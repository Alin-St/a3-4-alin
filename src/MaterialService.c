#include "MaterialService.h"
#include <string.h>
#include <time.h>

// Constructor / Destructor.
MaterialService* matServ_create(MaterialRepository* repository)
{
	MaterialService* serv = calloc(1, sizeof(MaterialService));
	if (serv) {
		serv->repository = repository;
		serv->undoStack = vector_create(0);
		serv->redoStack = vector_create(0);
	}

	return serv;
}

void matServ_destroy(MaterialService* serv)
{
	if (serv == NULL)
		return;

	for (size_t i = 0; i < vector_length(serv->undoStack); ++i)
		matOp_destroy(vector_get(serv->undoStack, i));

	for (size_t i = 0; i < vector_length(serv->redoStack); ++i)
		matOp_destroy(vector_get(serv->redoStack, i));

	vector_destroy(serv->undoStack);
	vector_destroy(serv->redoStack);
	free(serv);
}

// Properties.
size_t matServ_matCount(MaterialService* serv)
{
	return matRepo_matCount(serv->repository);
}

// CRUD Operations.
int matServ_add(MaterialService* serv, int id, const char* name, const char* supplier, float quantity, Date exp_date, int undoable)
{
	if (matServ_findByNSE(serv, name, supplier, exp_date) != NULL)
		return -5;

	if (id == -1)
		id = matRepo_getFreeid(serv->repository);

	Material* mat = material_construct(id, name, supplier, quantity, exp_date);
	int res = (int)matRepo_save(serv->repository, mat);
	if (res >= 0 && undoable)
		matServ_addUndoOperation(serv, REMOVE, mat);
	material_destroy(mat);

	if (res < 0)
		return res;
	return id;
}

const Material* matServ_findById(MaterialService* serv, int id)
{
	return matRepo_getById(serv->repository, id);
}

int matServ_updateById(MaterialService* serv, int id, const char* name, const char* supplier, float quantity, Date exp_date, Material* oldMat, int undoable)
{
	const Material *curMat = matServ_findById(serv, id);
	if (curMat == NULL)
		return -3;
	
	Material* curMatCopy = material_duplicate(curMat);
	Material* newMat = material_construct(id, name, supplier, quantity, exp_date);

	int res = (int)matRepo_updateById(serv->repository, newMat);
	if (res > 0)
		res = 0;

	if (res >= 0 && undoable)
		matServ_addUndoOperation(serv, UPDATE, curMatCopy);

	if (res >= 0 && oldMat != NULL)
		material_set(oldMat, curMatCopy);

	material_destroy(curMatCopy);
	material_destroy(newMat);
	return res;
}

int matServ_removeById(MaterialService* serv, int id, Material* remMat, int undoable)
{
	const Material* mat = matServ_findById(serv, id);
	if (mat == NULL)
		return -3;

	Material* matCopy = material_duplicate(mat);

	int res = (int)matRepo_deleteById(serv->repository, id);
	if (res > 0)
		res = 0;

	if (res >= 0 && undoable)
		matServ_addUndoOperation(serv, ADD, matCopy);

	if (res >= 0 && remMat != NULL)
		material_set(remMat, matCopy);

	material_destroy(matCopy);
	return res;
}

void matServ_getAll(MaterialService* serv, Vector* v)
{
	for (size_t i = 0; i < matRepo_matCount(serv->repository); ++i) {
		const Material* mat = matRepo_getByIndex(serv->repository, i);
		vector_add(v, (void*)mat);
	}
}

// Undo / Redo.
void matServ_addUndoOperation(MaterialService* serv, OperationType type, const Material* mat)
{
	// Clear redo stack.
	for (size_t i = 0; i < vector_length(serv->redoStack); ++i)
		matOp_destroy(vector_get(serv->redoStack, i));
	vector_clear(serv->redoStack);

	// Add the undo operation.
	MaterialOperation* op = matOp_construct(type, mat);
	vector_add(serv->undoStack, op);
}

int matServ_performOperation(MaterialService* serv, const MaterialOperation* op, MaterialOperation* revOp)
{
	OperationType type = matOp_type(op);
	const Material* mat = matOp_material(op);

	MaterialOperation* result = matOp_create();
	Material* revOpMat = material_create();
	int exCode = 0;

	if (type == ADD) {
		exCode = matServ_add(serv, material_id(mat), material_name(mat), material_supplier(mat), material_quantity(mat), material_expDate(mat), 0);
		matOp_setTypeAndMaterial(result, REMOVE, mat);
	}
	else if (type == UPDATE) {
		exCode = matServ_updateById(serv, material_id(mat), material_name(mat), material_supplier(mat), material_quantity(mat), material_expDate(mat), revOpMat, 0);
		matOp_setTypeAndMaterial(result, UPDATE, revOpMat);
	}
	else if (type == REMOVE) {
		exCode = matServ_removeById(serv, material_id(mat), revOpMat, 0);
		matOp_setTypeAndMaterial(result, ADD, revOpMat);
	}

	if (revOp)
		matOp_setTypeAndMaterial(revOp, matOp_type(result), matOp_material(result));

	material_destroy(revOpMat);
	matOp_destroy(result);
	return exCode;
}

int matServ_undo(MaterialService* serv)
{
	if (vector_length(serv->undoStack) == 0)
		return 0;

	MaterialOperation* op = vector_get(serv->undoStack, vector_length(serv->undoStack) - 1);
	MaterialOperation* revOp = matOp_create();
	matServ_performOperation(serv, op, revOp);
	vector_add(serv->redoStack, revOp);
	vector_removeAt(serv->undoStack, vector_length(serv->undoStack) - 1);
	matOp_destroy(op);
	return 1;
}

int matServ_redo(MaterialService* serv)
{
	if (vector_length(serv->redoStack) == 0)
		return 0;

	MaterialOperation* op = vector_get(serv->redoStack, vector_length(serv->redoStack) - 1);
	MaterialOperation* revOp = matOp_create();
	matServ_performOperation(serv, op, revOp);
	vector_add(serv->undoStack, revOp);
	vector_removeAt(serv->redoStack, vector_length(serv->redoStack) - 1);
	matOp_destroy(op);
	return 1;
}

// Methods.
int matServ_addOrUpdateByNSE(MaterialService* serv, int id, const char* name, const char* supplier, float quantity, Date exp_date, Material* oldMat)
{
	const Material* curMat = matServ_findByNSE(serv, name, supplier, exp_date);

	if (curMat == NULL) {
		if (oldMat != NULL)
			material_id_set(oldMat, -1);
		return matServ_add(serv, id, name, supplier, quantity, exp_date, 1);
	}
	else
		return matServ_updateByNSE(serv, name, supplier, material_quantity(curMat) + quantity, exp_date, oldMat);
}

const Material* matServ_findByNSE(MaterialService* serv, const char* name, const char* supplier, Date exp_date)
{
	for (size_t i = 0; i < matRepo_matCount(serv->repository); ++i) {
		const Material* mat = matRepo_getByIndex(serv->repository, i);
		if (strcmp(material_name(mat), name) == 0 &&
			strcmp(material_supplier(mat), supplier) == 0 &&
			material_expDate(mat).year == exp_date.year &&
			material_expDate(mat).month == exp_date.month &&
			material_expDate(mat).day == exp_date.day)
		{
			return mat;
		}
	}

	return NULL;
}

int matServ_updateByNSE(MaterialService* serv, const char* name, const char* supplier, float quantity, Date exp_date, Material* oldMat)
{
	const Material* curMat = matServ_findByNSE(serv, name, supplier, exp_date);
	if (curMat == NULL)
		return -4;

	int oldId = material_id(curMat);
	int result = matServ_updateById(serv, material_id(curMat), name, supplier, quantity, exp_date, oldMat, 1);

	if (result < 0)
		return result;
	return oldId;
}

int matServ_removeByNSE(MaterialService* serv, const char* name, const char* supplier, Date exp_date, Material* remMat)
{
	const Material* curMat = matServ_findByNSE(serv, name, supplier, exp_date);
	if (curMat == NULL)
		return -4;

	int remId = material_id(curMat);
	int result = matServ_removeById(serv, material_id(curMat), remMat, 1);

	if (result < 0)
		return result;
	return remId;
}

void matServ_get_materials_past_exp(MaterialService* serv, Vector* v, const char* optStr)
{
	if (vector_length(v) > 0)
		return;

	time_t seconds = time(NULL);
	struct tm* current_time = localtime(&seconds);
	int curYear = current_time->tm_year + 1900;
	int curMonth = current_time->tm_mon + 1;
	int curDay = current_time->tm_mday;

	if (optStr != NULL && strlen(optStr) == 0)
		optStr = NULL;

	matServ_getAll(serv, v);

	for (size_t i = vector_length(v); i-- > 0; ) {
		const Material* mat = vector_get(v, i);
		int expYear = material_expDate(mat).year, expMonth = material_expDate(mat).month, expDay = material_expDate(mat).day;

		int expired = (expYear < curYear || (expYear == curYear && expMonth < curMonth) || (expYear == curYear && expMonth == curMonth && expDay < curDay));
		int containsString = (optStr == NULL || strstr(material_name(mat), optStr) != NULL);

		if (!expired || !containsString)
			vector_removeFastAt(v, i);
	}
}

void matServ_getMaterialsSortedByQuantity(MaterialService* serv, Vector* v)
{
	matServ_getAll(serv, v);

	for (size_t i = 1; i < vector_length(v); ++i) {
		for (size_t j = i; j >= 1; --j) {
			const Material* leftMat = vector_get(v, j - 1);
			const Material* rightMat = vector_get(v, j);

			if (material_quantity(leftMat) > material_quantity(rightMat)) {
				void* temp = vector_get(v, j);
				vector_set(v, j, vector_get(v, j - 1));
				vector_set(v, j - 1, temp);
			}
			else break;
		}
	}
}

void matServ_getMaterialsFromSupplierInShortSupply(MaterialService* serv, Vector* v, const char* supplier, float max_quantity)
{
	matServ_getAll(serv, v);

	for (size_t i = vector_length(v); i-- > 0; ) {
		const Material* mat = matRepo_getByIndex(serv->repository, i);
		if (strcmp(material_supplier(mat), supplier) != 0 || material_quantity(mat) > max_quantity)
			vector_removeFastAt(v, i);
	}

	for (size_t i = 1; i < vector_length(v); ++i) {
		for (size_t j = i; j >= 1; --j) {
			const Material* leftMat = vector_get(v, j - 1);
			const Material* rightMat = vector_get(v, j);

			if (material_quantity(leftMat) > material_quantity(rightMat)) {
				void* temp = vector_get(v, j);
				vector_set(v, j, vector_get(v, j - 1));
				vector_set(v, j - 1, temp);
			}
			else
				break;
		}
	}
}
