#include "MaterialRepository.h"

// Constructor / Destructor.
MaterialRepository* matRepo_create(int(*validator)(const Material* mat))
{
	MaterialRepository* rep = calloc(1, sizeof(MaterialRepository));
	if (rep) {
		rep->materials = vector_create(0);
		rep->validator = validator;
	}

	return rep;
}

void matRepo_destroy(MaterialRepository* rep)
{
	if (rep == NULL)
		return;

	for (size_t i = 0; i < vector_length(rep->materials); ++i)
		material_destroy(vector_get(rep->materials, i));

	vector_destroy(rep->materials);
	free(rep);
}

// Properties.
size_t matRepo_matCount(MaterialRepository* rep)
{
	return vector_length(rep->materials);
}

// Methods.
size_t matRepo_save(MaterialRepository* rep, const Material* mat)
{
	if (mat == NULL || (rep->validator != NULL && !rep->validator(mat)))
		return -1;

	for (size_t i = 0; i < vector_length(rep->materials); ++i) {
		const Material* curMat = vector_get(rep->materials, i);
		if (material_id(curMat) == material_id(mat))
			return -2;
	}

	vector_add(rep->materials, material_duplicate(mat));
	return vector_length(rep->materials) - 1;
}

const Material* matRepo_getById(MaterialRepository* rep, int id)
{
	for (size_t i = 0; i < vector_length(rep->materials); ++i) {
		const Material* curMat = vector_get(rep->materials, i);
		if (material_id(curMat) == id)
			return curMat;
	}

	return NULL;
}

const Material* matRepo_getByIndex(MaterialRepository* rep, size_t index)
{
	return vector_get(rep->materials, index);
}

size_t matRepo_updateById(MaterialRepository* rep, const Material* newMat)
{
	if (newMat == NULL || (rep->validator != NULL && !rep->validator(newMat)))
		return -1;

	for (size_t i = 0; i < vector_length(rep->materials); ++i) {
		Material* curMat = vector_get(rep->materials, i);
		if (material_id(curMat) == material_id(newMat)) {
			material_destroy(curMat);
			vector_set(rep->materials, i, material_duplicate(newMat));
			return i;
		}
	}

	return -3;
}

size_t matRepo_deleteById(MaterialRepository* rep, int id)
{
	for (size_t i = 0; i < vector_length(rep->materials); ++i) {
		Material* curMat = vector_get(rep->materials, i);
		if (material_id(curMat) == id) {
			material_destroy(curMat);
			vector_removeFastAt(rep->materials, i);
			return i;
		}
	}

	return -3;
}

int matRepo_getFreeid(MaterialRepository* rep)
{
	int id = 0;
	for (size_t i = 0; i < vector_length(rep->materials); ++i) {
		const Material* curMat = vector_get(rep->materials, i);
		if (material_id(curMat) >= id)
			id = material_id(curMat) + 1;
	}

	return id;
}
