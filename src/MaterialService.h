#ifndef MATERIAL_SERVICE
#define MATERIAL_SERVICE

#include "MaterialRepository.h"
#include "Material.h"
#include "MaterialOperation.h"

// The internal data for a material service.
// Do not use struct members directly. Use only methods that start with 'matServ_'.
// The service must be initialized with 'matServ_create' and destroyed with 'matServ_destroy'.
// If not specified otherwise, material service pointer cannot be NULL in material service methods.
typedef struct {
	MaterialRepository* repository;
	Vector* undoStack;
	Vector* redoStack;
} MaterialService;

// CONSTRUCTOR / DESTRUCTOR.

// Initialize a material service that will use a given repository.
MaterialService* matServ_create(MaterialRepository* repository);

// Release all service resources and free the service itself.
// If service is NULL, nothing happens.
void matServ_destroy(MaterialService* serv);

// PROPERTIES.

// Returns the number of materials in the underlying repository.
size_t matServ_matCount(MaterialService* serv);

// CRUD OPERATIONS.

// Add a material to the repository and return its id. Use id -1 to find an unused id.
// Set 'undoable' to 0 if you don't want to undo this operation (this is dangerous). Recommended to keep it 1.
// If material failed validation, returns -1.
// If a material with the same id exists, returns -2.
// If a material with the same name, supplier and expiration date exists, returns -5.
int matServ_add(MaterialService* serv, int id, const char* name, const char* supplier, float quantity, Date exp_date, int undoable);

// Returns the material with the specified id if any, otherwise NULL.
const Material* matServ_findById(MaterialService* serv, int id);

// Updates the material with the specified id and returns 0 on success.
// If 'oldMat' is not null saves the old material there.
// Set 'undoable' to 0 if you don't want to undo this operation (this is dangerous). Recommended to keep it 1.
// If validation fails returns -1.
// If no material with the specified id is found, returns -3.
int matServ_updateById(MaterialService* serv, int id, const char* name, const char* supplier, float quantity, Date exp_date, Material* oldMat, int undoable);

// Deletes the material with the specified id and returns 0 on success.
// If 'remMat' is not null saves the removed material there.
// Set 'undoable' to 0 if you don't want to undo this operation (this is dangerous). Recommended to keep it 1.
// If no material with the specified id is found, returns -3.
int matServ_removeById(MaterialService* serv, int id, Material* remMat, int undoable);

// Saves in the given vector all materials. Do not destroy or modify the materials.
void matServ_getAll(MaterialService* serv, Vector* v);

// UNDO / REDO.

// Adds an undoable operation to the undo stack and clears the redos. Not recommended to use outside the service.
void matServ_addUndoOperation(MaterialService* serv, OperationType type, const Material* mat);

// Perform a given operation and return the return value for the specific operation. Not recommended to use outside the service.
// If 'revOp' is not null it saves the reverse operation there.
int matServ_performOperation(MaterialService* serv, const MaterialOperation* op, MaterialOperation* revOp);

// Undo the last operation. Returns 0 for failure, 1 for success.
int matServ_undo(MaterialService* serv);

// Redo the last operation. Returns 0 for failure, 1 for success.
int matServ_redo(MaterialService* serv);

// METHODS.

// Add a material to the repository and returns its id.
// If a material with the same name, supplier and expiration date already exists add only the quantity.
// The specified id is used only if the material ends up added, not updated. Use id -1 to assign an unused id.
// If 'oldMat' is not null: If the material is being updated, saves the old material in 'oldMat', if it is added, sets its id to -1.
// If the material fails validation, returns -1.
// If the material is being added, but the id already exists, returns -2.
int matServ_addOrUpdateByNSE(MaterialService* serv, int id, const char* name, const char* supplier, float quantity, Date exp_date, Material* oldMat);

// Returns the material with the specified name, supplier and expiration date or NULL if not found.
const Material* matServ_findByNSE(MaterialService* serv, const char* name, const char* supplier, Date exp_date);

// Updates the material with the specified name, supplier and expiration date and returns its id on success.
// If 'oldMat' is not null saves the old material there.
// If the new material fails validation returns -1.
// If the NSE is not found returns -4.
int matServ_updateByNSE(MaterialService* serv, const char* name, const char* supplier, float quantity, Date exp_date, Material* oldMat);

// Deletes the material with the specified name, supplier and expiration date and returns its id on success.
// If 'remMat' is not null, saves the removed material there.
// If the specified NSE is not found returns -4.
int matServ_removeByNSE(MaterialService* serv, const char* name, const char* supplier, Date exp_date, Material* remMat);

// Saves in the given vector all materials past their expiration date that contain a given optional string. Don't modify the materials.
// The given vector must be empty.
void matServ_get_materials_past_exp(MaterialService* serv, Vector* v, const char* optStr);

// Saves in the given vector all materials sorted by quantity. Don't modify the materials.
// The given vector must be empty.
void matServ_getMaterialsSortedByQuantity(MaterialService* serv, Vector* v);

// Saves in the given vector all materials which have the quantity less than a given number. Don't modify the materials.
// The given vector must be empty.
void matServ_getMaterialsFromSupplierInShortSupply(MaterialService* serv, Vector* v, const char* supplier, float max_quantity);

#endif
