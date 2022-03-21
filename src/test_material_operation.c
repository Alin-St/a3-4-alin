#include "MaterialOperation.h"
#include <assert.h>

void test_material_operation()
{
	MaterialOperation* op = matOp_create();
	assert(op != NULL);
	assert(op->type == NONE);
	assert(op->mat == NULL);
	assert(matOp_type(op) == NONE);
	assert(matOp_material(op) == NULL);
	matOp_destroy(op);


	assert(matOp_construct(ADD, NULL) == NULL);
	assert(matOp_construct(NONE, (void*)1) == NULL);


	Material* mat = material_create();
	material_id_set(mat, 10);
	assert(op = matOp_construct(ADD, mat));
	assert(op->type == ADD);
	assert(op->mat != mat);
	assert(matOp_type(op) == ADD);
	assert(matOp_material(op) != mat);
	material_destroy(mat);
	assert(material_id(matOp_material(op)) == 10);
	matOp_destroy(op);
}
