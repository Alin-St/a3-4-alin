#include "Console.h"
#include <stdio.h>
#include <string.h>

// Constructor / Destructor.
Console* console_create(MaterialService* matServ, size_t scanBufferSize)
{
	Console* c = NULL;
	if (scanBufferSize > 0 && matServ && (c = calloc(1, sizeof(Console)))) {
		c->matServ = matServ;
		if (c->ScanBuffer = calloc(scanBufferSize, sizeof(char)))
			c->scanBuffSize = scanBufferSize;
	}
	return c;
}

void console_destroy(Console* c)
{
	if (c) {
		free(c->ScanBuffer);
		free(c);
	}
}

// Console functions.
void console_run(Console* c)
{
	for (;;) {
		printf("\n\n"
			"1. Add material.\n"
			"2. Print all materials.\n"
			"3. Update material.\n"
			"4. Remove material.\n"
			"5. See all materials past their expiration date containing a given string in their name.\n"
			"6. Print all materials sorted by quantity.\n"
			"7. Get materials from a given supplier with quanitty less than a given value.\n"
			"8. Undo.\n"
			"9. Redo.\n"
			"10. Exit.\n"
		);

		int command = console_read_int(c, "Enter a number for a command: ");

		if (command == 1)
			console_add_material(c);
		else if (command == 2)
			console_print_all_materials(c);
		else if (command == 3)
			console_update_material(c);
		else if (command == 4)
			console_remove_material(c);
		else if (command == 5)
			console_print_materials_past_exp_with_str(c);
		else if (command == 6)
			console_print_all_materials_sorted_by_quantity(c);
		else if (command == 7)
			console_print_materials_from_supplier_in_short_supply(c);
		else if (command == 8)
			console_undo(c);
		else if (command == 9)
			console_redo(c);
		else if (command == 10)
			break;
		else
			printf("Command unknown.\n");
	}

	printf("Program finished.\n");
}

void console_add_material(Console* c)
{
	Material* mat = material_create();
	console_scan_material(c, mat, 1);

	if (matServ_addOrUpdateByNSE(c->matServ, -1, material_name(mat), material_supplier(mat), material_quantity(mat), material_expDate(mat), NULL) >= 0)
		printf("Material added successfully.\n");
	else
		printf("Material cannot be added. Validation probably failed.\n");

	material_destroy(mat);
}

void console_print_all_materials(Console* c)
{
	Vector* v = vector_create(0);
	matServ_getAll(c->matServ, v);
	console_print_materials(c, v, "Materials are:");
	vector_destroy(v);
}

void console_update_material(Console* c)
{
	Material* mat = material_create();
	console_scan_material(c, mat, 1);

	int result = matServ_updateByNSE(c->matServ, material_name(mat), material_supplier(mat), material_quantity(mat), material_expDate(mat), NULL);
	if (result >= 0)
		printf("Material updated successfully.\n");
	else {
		if (result == -1)
			printf("Error: Validation failed.\n");
		else if (result == -4)
			printf("Error: Name, supplier and expiration date not found.\n");
	}

	material_destroy(mat);
}

void console_remove_material(Console* c)
{
	Material* mat = material_create();
	console_scan_material(c, mat, 0);

	if (matServ_removeByNSE(c->matServ, material_name(mat), material_supplier(mat), material_expDate(mat), NULL) >= 0)
		printf("Material removed successfully.\n");
	else
		printf("Material can't be removed. Name, supplier and expiration date probably not found.\n");

	material_destroy(mat);
}

void console_print_materials_past_exp_with_str(Console* c)
{
	console_read_line(c, "Containing string (leave empty to see all): ");
	
	Vector* v = vector_create(0);
	matServ_get_materials_past_exp(c->matServ, v, c->ScanBuffer);
	console_print_materials(c, v, "Requested materials are:");
	vector_destroy(v);
}

void console_print_all_materials_sorted_by_quantity(Console* c)
{
	Vector* v = vector_create(0);
	matServ_getMaterialsSortedByQuantity(c->matServ, v);
	console_print_materials(c, v, "Materials are (sorted by quantity):");
	vector_destroy(v);
}

void console_print_materials_from_supplier_in_short_supply(Console* c)
{
	console_read_line(c, "Supplier: ");

	char* supplier = calloc(strlen(c->ScanBuffer) + 1, sizeof(char));
	if (supplier != NULL)
		strcpy(supplier, c->ScanBuffer);

	float max_quantity = console_read_float(c, "Maximum quantity: ");

	Vector* v = vector_create(0);
	matServ_getMaterialsFromSupplierInShortSupply(c->matServ, v, supplier, max_quantity);
	console_print_materials(c, v, "Materials in short supply are:");
	vector_destroy(v);

	free(supplier);
}

void console_undo(Console* c)
{
	if (matServ_undo(c->matServ))
		printf("Undo completed successfully.\n");
	else
		printf("Undo failed. Probably no more undos left.\n");
}

void console_redo(Console* c)
{
	if (matServ_redo(c->matServ))
		printf("Redo completed successfully.\n");
	else
		printf("Redo failed. Probably no more redos left.\n");
}

// Helper functions.
void console_read_line(Console* c, const char* prompt)
{
	if (prompt)
		printf("%s", prompt);

	char ch = '\0';
	size_t index = 0;
	while (scanf("%c", &ch) && ch != '\n' && ch != '\0' && index < c->scanBuffSize - 1)
		c->ScanBuffer[index++] = ch;

	if (index < c->scanBuffSize)
		c->ScanBuffer[index] = '\0';
}

float console_read_float(Console* c, const char* prompt)
{
	float result = 0;
	do {
		console_read_line(c, prompt);
	} while (!sscanf(c->ScanBuffer, "%f", &result));

	return result;
}

int console_read_int(Console* c, const char* prompt)
{
	return (int) console_read_float(c, prompt);
}

void console_scan_material(Console* c, Material* mat, int scanQuantity)
{
	console_read_line(c, "Name: ");
	material_name_set(mat, c->ScanBuffer);

	console_read_line(c, "Supplier: ");
	material_supplier_set(mat, c->ScanBuffer);

	if (scanQuantity) {
		float quantity = console_read_float(c, "Quantity: ");
		material_quantity_set(mat, quantity);
	}

	Date expDate = { 0 };
	expDate.year = console_read_int(c, "Expiration date year: ");
	expDate.month = console_read_int(c, "Expiration date month: ");
	expDate.day = console_read_int(c, "Expiration date day: ");
	material_expDate_set(mat, expDate);
}

void console_print_material(Console* c, const Material* mat, int index)
{
	if (index >= 0)
		printf("%2d) ", index);

	// printf("Id: %2d, ", material_id(mat));

	printf("Name: \"%20s\", Supplier: \"%20s\", Quantity: %4.2f, Expiration date: %02d/%02d/%04d.\n",
		material_name(mat), material_supplier(mat), material_quantity(mat),
		material_expDate(mat).month, material_expDate(mat).day, material_expDate(mat).year);
}

void console_print_materials(Console* c, const Vector* materials, const char* prompt)
{
	if (prompt)
		printf("%s\n", prompt);

	for (size_t i = 0; i < vector_length(materials); ++i) {
		const Material* mat = vector_get(materials, i);
		console_print_material(c, mat, i + 1);
	}

	if (vector_length(materials) == 0)
		printf("No materials.\n");
}
