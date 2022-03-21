#include "MaterialValidator.h"
#include <string.h>
#include <math.h>

const char* VALID_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `~!@#$%^&*()-_=+[{]}\\|;:'\",<.>/?";

int matValid_validate(const Material* mat)
{
	if (mat == NULL)
		return 0;

	// Id.
	if (material_id(mat) < 0)
		return 0;

	// Name.
	const char* name = material_name(mat);
	if (name == NULL || strlen(name) < 1 || strlen(name) > 100)
		return 0;

	for (size_t i = 0; name[i] != '\0'; ++i) {
		if (!strchr(VALID_CHARS, name[i]))
			return 0;
	}

	// Supplier.
	const char* supplier = material_supplier(mat);
	if (supplier == NULL || strlen(supplier) < 1 || strlen(supplier) > 100)
		return 0;

	for (size_t i = 0; supplier[i] != '\0'; ++i) {
		if (!strchr(VALID_CHARS, supplier[i]))
			return 0;
	}

	// Quantity.
	float quantity = material_quantity(mat);
	if (isnan(quantity) || quantity <= 0.0f || isinf(quantity))
		return 0;

	// Expiration date.
	Date exp_date = material_expDate(mat);
	int year = exp_date.year, month = exp_date.month, day = exp_date.day;
	if (year < 1 || month < 1 || month > 12 || day < 1 || day > 31)
		return 0;

	return 1;
}
