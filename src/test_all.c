#include "tests.h"

void test_all()
{
	test_material();
	test_vector();
	test_material_validator();
	test_material_operation();

	test_material_repository();

	test_material_service();
}
