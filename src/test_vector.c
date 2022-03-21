#include "Vector.h"
#include <assert.h>

void test_vector()
{
	Vector* vec = vector_create(0);
	assert(vec != NULL);
	assert(vec->array == NULL);
	assert(vec->capacity == 0);
	assert(vec->length == 0);
	vector_destroy(vec);

	vec = vector_create(10);
	assert(vec != NULL);
	const Vector* cvec = vec;

	assert(cvec->array != NULL);
	assert(cvec->capacity == 10);
	assert(cvec->length == 0);

	assert(vector_length(cvec) == 0);
	assert(vector_capacity(cvec) == 10);

	vector_add(vec, (void*)100);
	assert(cvec->capacity == 10);
	assert(cvec->length == 1);
	assert(cvec->array[0] == (void*)100);

	assert(vector_get(cvec, 0) == (void*)100);
	assert(vector_get(cvec, -1) == NULL);
	assert(vector_get(cvec, 1) == NULL);

	vector_shrinkToFit(vec);

	assert(vector_length(cvec) == 1);
	assert(vector_capacity(cvec) == 1);
	assert(vector_get(cvec, 0) == (void*)100);

	vector_add(vec, (void*)200);
	vector_add(vec, (void*)300);
	vector_removeAt(vec, 0);

	assert(vector_length(cvec) == 2);
	assert(vector_capacity(cvec) == 4);
	assert(vector_get(cvec, 0) == (void*)200);
	assert(vector_get(cvec, 1) == (void*)300);

	vector_add(vec, (void*)400);
	vector_add(vec, (void*)500);
	vector_removeFastAt(vec, 1);

	assert(vector_length(cvec) == 3);
	assert(vector_get(cvec, 0) == (void*)200);
	assert(vector_get(cvec, 1) == (void*)500);
	assert(vector_get(cvec, 2) == (void*)400);

	vector_clear(vec);

	assert(vector_capacity(cvec) == 0);
	assert(vector_length(cvec) == 0);
	assert(cvec->array == NULL);

	vector_destroy(vec);
}
