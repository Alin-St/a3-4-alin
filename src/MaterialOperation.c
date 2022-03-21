#include "MaterialOperation.h"
#include <stdlib.h>

// Constructor / Destructor.
MaterialOperation* matOp_create()
{
	return calloc(1, sizeof(MaterialOperation));
}

MaterialOperation* matOp_construct(OperationType type, const Material* mat)
{
	// Check for invalid type of operations.
	if ((type == NONE && mat) || (type == ADD && !mat) || (type == UPDATE && !mat) || (type == REMOVE && !mat))
		return NULL;

	MaterialOperation* op = calloc(1, sizeof(MaterialOperation));
	if (op) {
		op->type = type;
		op->mat = material_duplicate(mat);
	}
	return op;
}

void matOp_destroy(MaterialOperation* op)
{
	if (op) {
		material_destroy(op->mat);
		free(op);
	}
}

// Properties.
OperationType matOp_type(const MaterialOperation* op)
{
	return op->type;
}

const Material* matOp_material(const MaterialOperation* op)
{
	return op->mat;
}

void matOp_setToNone(MaterialOperation* op)
{
	op->type = NONE;
	material_destroy(op->mat);
	op->mat = NULL;
}

void matOp_setTypeAndMaterial(MaterialOperation* op, OperationType type, const Material* mat)
{
	if ((type == NONE && mat) || (type == ADD && !mat) || (type == UPDATE && !mat) || (type == REMOVE && !mat))
		return;

	material_destroy(op->mat);
	op->mat = material_duplicate(mat);
	op->type = type;
}
