#ifndef MATERIAL_VALIDATOR
#define MATERIAL_VALIDATOR

#include "Material.h"

// Validate a given material (can be NULL). Returns 1 if valid, 0 otherwise.
int matValid_validate(const Material* mat);

#endif
