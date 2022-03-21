#include "material.h"
#include <stdlib.h>
#include <string.h>

// Constructor / Destructor
Material* material_create()
{
	return calloc(1, sizeof(Material));
}

Material* material_construct(int id, const char* name, const char* supplier, float quantity, Date exp_date)
{
	Material* mat = material_create();
	if (mat) {
		material_id_set(mat, id);
		material_name_set(mat, name);
		material_supplier_set(mat, supplier);
		material_quantity_set(mat, quantity);
		material_expDate_set(mat, exp_date);
	}

	return mat;
}

Material* material_duplicate(const Material* oldMat)
{
	Material* newMat = material_create();
	if (newMat)
		material_set(newMat, oldMat);
	return newMat;
}

void material_destroy(Material* mat)
{
	if (mat) {
		free(mat->name);
		free(mat->supplier);
		free(mat);
	}
}

// Set operator.
void material_set(Material* mat, const Material* other)
{
	material_id_set(mat, material_id(other));
	material_name_set(mat, material_name(other));
	material_supplier_set(mat, material_supplier(other));
	material_quantity_set(mat, material_quantity(other));
	material_expDate_set(mat, material_expDate(other));
}

// Properties.
int material_id(const Material* mat)
{
	return mat->id;
}

void material_id_set(Material* mat, int newId)
{
	mat->id = newId;
}

const char* material_name(const Material* mat)
{
	return mat->name;
}

void material_name_set(Material* mat, const char* newName)
{
	free(mat->name);
	mat->name = NULL;

	if (newName) {
		size_t nameLen = strlen(newName);
		if (mat->name = calloc(nameLen + 1, sizeof(char)))
			strcpy(mat->name, newName);
	}
}

const char* material_supplier(const Material* mat)
{
	return mat->supplier;
}

void material_supplier_set(Material* mat, const char* newSupplier)
{
	free(mat->supplier);
	mat->supplier = NULL;

	if (newSupplier) {
		size_t supplierLen = strlen(newSupplier);
		if (mat->supplier = calloc(supplierLen + 1, sizeof(char)))
			strcpy(mat->supplier, newSupplier);
	}
}

float material_quantity(const Material* mat)
{
	return mat->quantity;
}

void material_quantity_set(Material* mat, float newQuantity)
{
	mat->quantity = newQuantity;
}

Date material_expDate(const Material* mat)
{
	return mat->exp_date;
}

void material_expDate_set(Material* mat, Date newExpDate)
{
	mat->exp_date = newExpDate;
}
