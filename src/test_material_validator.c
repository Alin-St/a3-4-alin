#include "MaterialValidator.h"
#include <assert.h>

void test_material_validator()
{
	Material* mat = material_create();
	assert(!matValid_validate(mat));

	Date expDate = { .year = 2001, .month = 12, .day = 1 };
	material_name_set(mat, "matty");
	material_supplier_set(mat, "suppi");
	material_expDate_set(mat, expDate);
	material_quantity_set(mat, 1.0f);
	assert(matValid_validate(mat));

	material_id_set(mat, -1);
	assert(!matValid_validate(mat));

	material_id_set(mat, 10);
	material_quantity_set(mat, -1.0f);
	assert(!matValid_validate(mat));

	material_quantity_set(mat, 10.0f);
	material_supplier_set(mat, "Bn 1");
	assert(matValid_validate(mat));

	assert(!matValid_validate(NULL));

	material_destroy(mat);
}
