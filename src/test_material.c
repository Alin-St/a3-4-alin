#include "Material.h"
#include <assert.h>
#include <string.h>

void test_material()
{
	Material* mat = material_create();
	const Material* cmat = mat;

	assert(cmat != NULL);
	assert(cmat->id == 0);
	assert(cmat->name == NULL);
	assert(cmat->supplier == NULL);
	assert(cmat->quantity == 0.0f);
	assert(cmat->exp_date.year == 0);
	assert(cmat->exp_date.month == 0);
	assert(cmat->exp_date.day == 0);

	assert(material_id(cmat) == 0);
	assert(material_name(cmat) == NULL);
	assert(material_supplier(cmat) == NULL);
	assert(material_quantity(cmat) == 0.0f);
	assert(material_expDate(cmat).year == 0);
	assert(material_expDate(cmat).month == 0);
	assert(material_expDate(cmat).day == 0);

	material_id_set(mat, 68);
	assert(material_id(cmat) == 68);

	material_name_set(mat, "HELLO");
	assert(strcmp(material_name(cmat), "HELLO") == 0);

	material_supplier_set(mat, "sup1");
	assert(strcmp(material_supplier(cmat), "sup1") == 0);

	material_name_set(mat, "hi");
	assert(strcmp(material_name(cmat), "hi") == 0);

	material_quantity_set(mat, 15.6f);
	assert(material_quantity(cmat) == 15.6f);

	Date expDate = { .year = 1970, .month = 10, .day = 20 };
	material_expDate_set(mat, expDate);
	expDate = material_expDate(cmat);
	assert(expDate.year == 1970);
	assert(expDate.month == 10);
	assert(expDate.day == 20);

	material_supplier_set(mat, NULL);
	assert(material_supplier(cmat) == NULL);

	Material* dupeMat = material_duplicate(cmat);
	assert(material_id(dupeMat) == 68);
	assert(strcmp(material_name(dupeMat), "hi") == 0);
	assert(material_supplier(dupeMat) == NULL);
	assert(material_quantity(dupeMat) == 15.6f);
	assert(material_expDate(dupeMat).year == 1970);
	assert(material_expDate(dupeMat).month == 10);
	assert(material_expDate(dupeMat).day == 20);

	material_destroy(mat);
	material_destroy(dupeMat);
}
