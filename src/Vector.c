// Minimum capacity = Length
// Normal capacity = Length * 2
// Maximum capacity = Length * 4
// If capacity out of bounds, reset to normal

#include "Vector.h"
#include "string.h"

// Constructor / Destructor.
Vector* vector_create(size_t capacity)
{
	Vector* v = calloc(1, sizeof(Vector));
	if (v)
		vector_reserve(v, capacity);

	return v;
}

void vector_destroy(Vector* v)
{
	if (v == NULL)
		return;

	vector_clear(v);
	free(v);
}

// Properties.
const void** vector_array(const Vector* v)
{
	return v->array;
}

size_t vector_length(const Vector* v)
{
	return v->length;
}

size_t vector_capacity(const Vector* v)
{
	return v->capacity;
}

void* vector_get(const Vector* v, size_t index)
{
	if (index < 0 || index >= v->length)
		return NULL;

	return v->array[index];
}

void vector_set(Vector* v, size_t index, void* newItem)
{
	if (index < 0 || index >= v->length)
		return;

	v->array[index] = newItem;
}

// Methods.
void vector_add(Vector* v, void* item)
{
	// If Capacity less than Minimum capacity for the new Length, reset to normal.
	size_t newLen = v->length + 1;
	if (v->capacity < newLen) {
		void** newArr = realloc(v->array, newLen * 2 * sizeof(void*));
		if (newArr) {
			v->array = newArr;
			v->capacity = newLen * 2;
		}
	}

	// If capacity is big enough, perform the addition.
	if (v->capacity >= newLen) {
		v->array[v->length] = item;
		v->length = newLen;
	}
}

void vector_removeAt(Vector* v, size_t index)
{
	if (index < 0 || index >= v->length)
		return;

	// Shift all the items left of the index to the left.
	if (index < v->length - 1)
		memmove(v->array + index, v->array + index + 1, (v->length - index - 1) * sizeof(void*));
	--v->length;

	// If Capacity greater than Maximum capacity for new Length, reset to normal.
	if (v->capacity > v->length * 4)
		vector_reserve(v, v->length * 2);
}

void vector_removeFastAt(Vector* v, size_t index)
{
	if (index < 0 || index >= v->length)
		return;

	// Replace the removed item with the last item.
	if (index < v->length - 1)
		v->array[index] = v->array[v->length - 1];
	--v->length;

	// If Capacity greater than Maximum capacity for new Length, reset to normal.
	if (v->capacity > v->length * 4)
		vector_reserve(v, v->length * 2);
}

void vector_clear(Vector* v)
{
	free(v->array);
	v->array = NULL;
	v->length = v->capacity = 0;
}

void vector_shrinkToFit(Vector* v)
{
	if (v->capacity == v->length)
		return;

	if (v->length == 0) {
		free(v->array);
		v->array = NULL;
		v->capacity = 0;
	}
	else {
		void** newArr = realloc(v->array, v->length * sizeof(void*));
		if (newArr) {
			v->array = newArr;
			v->capacity = v->length;
		}
	}
}

void vector_reserve(Vector* v, size_t capacity)
{
	if (capacity < v->length || capacity == v->capacity)
		return;

	if (capacity == 0) {
		free(v->array);
		v->array = NULL;
		v->capacity = 0;
	}
	else {
		void* newArr = realloc(v->array, capacity * sizeof(void*));
		if (newArr) {
			v->array = newArr;
			v->capacity = capacity;
		}
	}
}
