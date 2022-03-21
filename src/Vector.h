#ifndef VECTOR
#define VECTOR

#include <stdlib.h>

// The internal data used to represent a vector of pointers.
// Do not use struct members directly. Use only methods that start with 'vector_'.
// You need to initialize the vector with 'vector_create', and destroy it with 'vector_destroy' when you are done.
// If not specified otherwise, vector pointer cannot be NULL in vector methods.
typedef struct {
	void** array;
	size_t length;
	size_t capacity;
} Vector;

// CONSTRUCTOR / DESTRUCTOR.

// Initialize an empty vector with the given capacity.
Vector* vector_create(size_t capacity);

// Destroy the vector. If pointer is NULL nothing happens.
void vector_destroy(Vector* v);

// PROPERTIES.

const void** vector_array(const Vector* v);

// Get the number of elements in the vector.
size_t vector_length(const Vector* v);

// Get the capacity of the vector (the number of elements its container could store.
size_t vector_capacity(const Vector* v);

// Get the modifiable element on the specified index. If index is invalid returns NULL.
void* vector_get(const Vector* v, size_t index);

// Set the element on the specified index. If index is invalid nothing happens.
void vector_set(Vector* v, size_t index, void* newItem);

// METHODS.

// Add the item to the vector as the last element.
void vector_add(Vector* v, void* item);

// Remove the element on the specified index and shift all items behind the removed element with one position to the left.
void vector_removeAt(Vector* v, size_t index);

// Remove the element on the specified index. If it was not the last element, then the last element takes its place.
void vector_removeFastAt(Vector* v, size_t index);

// Removes all the elements.
void vector_clear(Vector* v);

// If the capacity is greater than the number of elements, the container is shrinked to the minimum size.
void vector_shrinkToFit(Vector* v);

// Reserve some extra space or shrink the container.
// Beware that any add operation might increase the capacity if needed and remove operations might reduce it.
// If the capacity is less than the size, nothing happens.
void vector_reserve(Vector* v, size_t capacity);

#endif
