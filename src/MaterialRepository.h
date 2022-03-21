#ifndef MATERIAL_REPOSITORY
#define MATERIAL_REPOSITORY

#include "Material.h"
#include "Vector.h"
#include <stdlib.h>

// The internal data for a material repository.
// Do not use the struct members directly. Instead, use only the methods that start with 'matRepo_'.
// The repository must be initialized with 'matRepo_create' and destroyed with 'matRepo_destroy'.
// If not specified otherwise, material repository pointer cannot be NULL in material repository methods.
typedef struct {
	Vector* materials;
	int(*validator)(const Material* mat);
} MaterialRepository;

// CONSTRUCTOR / DESTRUCTOR.

// Initialize a repository along with its underlying container.
// The validator can be NULL (materials won't be validated), or a function returning 0 for failure.
MaterialRepository* matRepo_create(int(*validator)(const Material* mat));

// Release all repository resources and free the repository itself.
// If repository is NULL, nothing happens.
void matRepo_destroy(MaterialRepository* rep);

// PROPERTIES.

// Returns the number of materials in the repository.
size_t matRepo_matCount(MaterialRepository* rep);

// METHODS.

// Saves a copy of the given material to the repository and returns the index.
// If the operation fails, the return value is negative and the material is not saved.
// If validation fails or material is NULL, the return value is -1.
// If a material with the same id is found, returns -2.
size_t matRepo_save(MaterialRepository* rep, const Material* mat);

// Returns the material with the specified id, or NULL if the material is not found.
const Material* matRepo_getById(MaterialRepository* rep, int id);

// Returns the material on the specified index, or NULL if the index is invalid.
const Material* matRepo_getByIndex(MaterialRepository* rep, size_t index);

// Replaces the material with the same id with a copy of the new material and returns the index.
// If the operation fails, the return value is negative and the material is not updated.
// If validation fails or material is NULL, the return value is -1.
// If no material with the same id is found, returns -3.
size_t matRepo_updateById(MaterialRepository* rep, const Material* newMat);

// Deletes the material with the specified id.
// If the material is found, returns the old index, otherwise returns -3 and no deletion is performed.
size_t matRepo_deleteById(MaterialRepository* rep, int id);

// Returns an unused id.
int matRepo_getFreeid(MaterialRepository* rep);

#endif
