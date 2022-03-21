#ifndef CONSOLE
#define CONSOLE

#include "MaterialService.h"

// The internal data for a console that provides ui for given services.
// Do not use struct members directly. Instead, use only methods that start with 'console_'.
// The console object must be initialized with 'console_create', started with 'console_run' and destroyed with 'console_destroy'.
// If not specified otherwise, console pointer cannot be NULL in console methods.
typedef struct {
	MaterialService* matServ;
	char* ScanBuffer;
	size_t scanBuffSize;
} Console;

// CONSTRUCTOR / DESTRUCTOR.

// Initialize a console object that uses the given services.
// The console uses a buffer for reading input line by line; Make sure the size of this buffer is big enough.
// Make sure the services are valid and stay valid while you use the console.
Console* console_create(MaterialService* matServ, size_t scanBufferSize);

// Release all console resources and free the console itself.
void console_destroy(Console* c);

// CONSOLE FUNCTIONS.

// Starts the console application.
void console_run(Console* c);

// Read a material from keyboard and add it.
void console_add_material(Console* c);

// Prints all the materials from the service.
void console_print_all_materials(Console* c);

// Reads a material from the keyboard and updates it if its identifiers exists.
void console_update_material(Console* c);

// Reads the material identifiers from keyboard and removes the material if it exists.
void console_remove_material(Console* c);

// Reads an expiration date and prints the materials past it.
void console_print_materials_past_exp_with_str(Console* c);

// Prints all materials sorted by quantity.
void console_print_all_materials_sorted_by_quantity(Console* c);

// Reads a supplier and a maximum quantity and prints all materials from that supplier with the specified max quantity.
void console_print_materials_from_supplier_in_short_supply(Console* c);

// Performs an undo operation.
void console_undo(Console* c);

// Performs a redo operations.
void console_redo(Console* c);

// HELPER FUNCTIONS.

// Scans a line character by character and saves it in the internal buffer (without '\n' at the end).
// If prompt is not NULL, it first prints the prompt.
void console_read_line(Console* c, const char* prompt);

// Print the prompt if it is not NULL, scan a line and parse it into a float. If input is invalid, repeat the operation.
float console_read_float(Console* c, const char* prompt);

// Print the prompt if it is not NULL, scan a line and parse it into an int. If input is invalid, repeat the operation.
int console_read_int(Console* c, const char* prompt);

// Scans the properties of a material (except id) line by line with prompts and stores them in the given material object.
// If 'scanQuantity' is 0 the quantity is not read.
// Given material cannot be NULL.
void console_scan_material(Console* c, Material* mat, int scanQuantity);

// Prints the given material (can't be NULL) using padding, on a single line.
// If index is greater or equal to 0 prints the index at the beginning.
void console_print_material(Console* c, const Material* mat, int index);

// Prints all the given materials (cannot be NULL and must be a vector of materials).
// If the prompt is not NULL, it first prints the prompt with a new line character at the end.
void console_print_materials(Console* c, const Vector* materials, const char* prompt);

#endif
