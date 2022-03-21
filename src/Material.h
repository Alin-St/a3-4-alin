#ifndef MATERIAL
#define MATERIAL

#include "Date.h"

// The internal data used to represent a material in the bakery.
// Do not use struct members directly. Use only the methods that start with 'material_'.
// You need to initialize the material with 'material_create' or 'material_construct', and destroy it with 'material_destroy'.
// If not specified otherwise, material pointer cannot be NULL in the methods that start with 'material_'
typedef struct {
	int id;
	char* name;
	char* supplier;
	float quantity;
	Date exp_date;
} Material;

// CONSTRUCTOR / DESTRUCTOR.

// Initialize a material with NULL name and supplier, quantity 0 and expiration date 0000/00/00.
Material* material_create();

// Initialize a material with the given properties.
Material* material_construct(int id, const char* name, const char* supplier, float quantity, Date exp_date);

// Create a material identical with the given material.
Material* material_duplicate(const Material* mat);

// Destroy a material. If pointer is NULL nothing happens.
void material_destroy(Material* mat);

// SET OPERATOR.

// Set all the properties of the first material as the properties of the second one.
void material_set(Material* mat, const Material* other);

// PROPERTIES.

// Get the id of the material.
int material_id(const Material* mat);

// Set the id of the material.
void material_id_set(Material* mat, int newId);

// Get the name of the material.
const char* material_name(const Material* mat);

// Set the name of the material (name can be NULL).
void material_name_set(Material* mat, const char* newName);

// Get the name of the supplier.
const char* material_supplier(const Material* mat);

// Set the name of the supplier (supplier can be NULL).
void material_supplier_set(Material* mat, const char* newSupplier);

// Get the quantity of the material.
float material_quantity(const Material* mat);

// Set the quantity of the material.
void material_quantity_set(Material* mat, float newQuantity);

// Get the expiration date of the material.
Date material_expDate(const Material* mat);

// Set the expiration date of the material.
void material_expDate_set(Material* mat, Date newExpDate);

#endif
