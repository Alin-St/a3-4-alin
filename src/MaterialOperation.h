#ifndef MATERIAL_OPERATION
#define MATERIAL_OPERATION

#include "OperationType.h"
#include "Material.h"

// Internal data used for a CRUD operation on a material data set. Do not use directly, use only 'matOp_*' methods.
typedef struct {
	OperationType type;
	Material* mat;
} MaterialOperation;

// CONSTRUCTOR / DESTRUCTOR.

// Initialize an empty operation.
MaterialOperation* matOp_create();

// Initialize an operation with the given properties. A copy of the given material is saved.
MaterialOperation* matOp_construct(OperationType type, const Material* mat);

// Release all resources and free the operation itself.
void matOp_destroy(MaterialOperation* op);

// PROPERTIES.

// Get the operation type of the material operation.
OperationType matOp_type(const MaterialOperation* op);

// Get the material of the material operation.
const Material* matOp_material(const MaterialOperation* op);

// METHODS.

// The operation type will be none and the material destroyed.
void matOp_setToNone(MaterialOperation* op);

// Sets the operation type and the material if they are a valid combination.
void matOp_setTypeAndMaterial(MaterialOperation* op, OperationType type, const Material* mat);

#endif
