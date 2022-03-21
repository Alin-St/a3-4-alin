#include "MaterialRepository.h"
#include "MaterialValidator.h"
#include <assert.h>
#include <string.h>

void test_material_repository()
{
	MaterialRepository* matRepo = matRepo_create(NULL);
	assert(matRepo != NULL);
	assert(matRepo->materials != NULL);
	assert(matRepo->materials->array == NULL);
	assert(matRepo->materials->capacity == 0);
	assert(matRepo->materials->length == 0);
	matRepo_destroy(matRepo);

	matRepo = matRepo_create(matValid_validate);
	assert(matRepo->validator == matValid_validate);

	Date expDate = { .year = 2022, .month = 12, .day = 1 };
	Material* mat = material_construct(0, "mat1", "sup1", 1.0f, expDate);
	assert(matRepo_save(matRepo, mat) == 0);
	material_destroy(mat);

	mat = material_construct(1, "mat2", "sup2", 2.0f, expDate);
	assert(matRepo_save(matRepo, mat) == 1);

	assert(matRepo_matCount(matRepo) == 2);
	assert(matRepo_getById(matRepo, 0) == matRepo_getByIndex(matRepo, 0));
	assert(matRepo_getById(matRepo, 1) == matRepo_getByIndex(matRepo, 1));
	assert(strcmp(material_name(matRepo_getByIndex(matRepo, 1)), "mat2") == 0);

	assert(matRepo_save(matRepo, mat) == -2);
	assert(matRepo_matCount(matRepo) == 2);
	
	material_id_set(mat, 10);
	material_quantity_set(mat, -1.0f);
	assert(matRepo_save(matRepo, mat) == -1);

	assert(matRepo_getFreeid(matRepo) == 2);

	assert(matRepo_deleteById(matRepo, 0) == 0);
	assert(matRepo_matCount(matRepo) == 1);
	assert(matRepo_getByIndex(matRepo, 0) == matRepo_getById(matRepo, 1));
	assert(strcmp(matRepo_getByIndex(matRepo, 0)->name, "mat2") == 0);

	material_supplier_set(mat, "sup2.1");
	material_quantity_set(mat, 100.0f);
	assert(matRepo_updateById(matRepo, mat) == -3);
	material_id_set(mat, 1);
	assert(matRepo_updateById(matRepo, mat) == 0);
	assert(matRepo_matCount(matRepo) == 1);
	assert(strcmp(matRepo_getById(matRepo, 1)->supplier, "sup2.1") == 0);
	assert(matRepo_getByIndex(matRepo, 0)->quantity == 100.0f);

	assert(matRepo_deleteById(matRepo, 1) == 0);
	assert(matRepo_matCount(matRepo) == 0);
	assert(matRepo_getByIndex(matRepo, 0) == NULL);

	material_destroy(mat);
	matRepo_destroy(matRepo);
}
