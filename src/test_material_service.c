#include "MaterialService.h"
#include "MaterialValidator.h"
#include <assert.h>
#include <string.h>

void test_material_service()
{
	// CRUD Operations.
	MaterialRepository* repo = matRepo_create(matValid_validate);
	MaterialService* serv = matServ_create(repo);

	assert(matServ_matCount(serv) == 0);
	assert(matServ_findById(serv, 0) == NULL);

	assert(matServ_add(serv, -1, "name1", "sup1", 10.0f, (Date) { 2030, 1, 1 }, 1) == 0);
	const Material* mat = matServ_findById(serv, 0);
	assert(mat != NULL);
	assert(mat == matServ_findById(serv, 0));
	assert(material_id(mat) == 0);
	assert(strcmp(material_name(mat), "name1") == 0);
	assert(material_quantity(mat) == 10.0f);

	assert(matServ_add(serv, -1, "name2", "sup1", -1.0f, (Date) { 2030, 1, 1 }, 1) == -1);
	assert(matServ_matCount(serv) == 1);
	assert(material_quantity(matServ_findById(serv, 0)) == 10.0f);

	assert(matServ_addOrUpdateByNSE(serv, -1, "name1", "sup1", 2.0f, (Date) { 2030, 1, 1 }, NULL) == 0);
	assert(matServ_matCount(serv) == 1);
	assert(material_quantity(matServ_findById(serv, 0)) == 10.0f + 2.0f);

	assert(matServ_removeById(serv, 1, NULL, 1) == -3);
	assert(matServ_removeById(serv, 0, NULL, 1) == 0);
	assert(matServ_matCount(serv) == 0);
	assert(matServ_findById(serv, 0) == NULL);
	assert(matServ_findById(serv, 0) == NULL);

	assert(matServ_updateById(serv, 0, "name1.1", "sup1.1", 10.0f, (Date) { 2025, 10, 10 }, NULL, 1) == -3);

	assert(matServ_add(serv, -1, "name1.2", "sup1.2", 110.0f, (Date) { 2022, 12, 12 }, 1) == 0);
	assert(matServ_updateById(serv, 0, "name1.3", "sup1.3", -10.0f, (Date) { 2023, 11, 11 }, NULL, 1) == -1);
	assert(strcmp(material_name(matServ_findById(serv, 0)), "name1.2") == 0);

	assert(matServ_updateById(serv, 0, "name1.3", "sup1.3", 10.0f, (Date) { 2023, 11, 11 }, NULL, 1) == 0);
	assert(strcmp(material_name(matServ_findById(serv, 0)), "name1.3") == 0);
	assert(material_quantity(matServ_findById(serv, 0)) == 10.0f);

	matServ_destroy(serv);
	matRepo_destroy(repo);
}
