#include "domain.h"
#include "repository.h"
#include "service.h"
#include "ui.h"
#include "tests.h"
#include <crtdbg.h>

#define SCAN_BUFFER_LENGTH 0x1000

void add_some_materials(MaterialService* matServ);

int main()
{
	test_all();

	int(*materialValidator)(const Material* mat) = matValid_validate;
	MaterialRepository* materialRepository = matRepo_create(materialValidator);
	MaterialService* materialService = matServ_create(materialRepository);
	Console* console = console_create(materialService, SCAN_BUFFER_LENGTH);

	add_some_materials(materialService);
	console_run(console);

	console_destroy(console);
	matServ_destroy(materialService);
	matRepo_destroy(materialRepository);

	_CrtDumpMemoryLeaks();

	return 0;
}

void add_some_materials(MaterialService* matServ)
{
	matServ_addOrUpdateByNSE(matServ, -1, "Flour",		"Good Flour SRL",			11.0f, (Date) { 2022, 12, 13 }, NULL);
	matServ_addOrUpdateByNSE(matServ, -1, "Milk",		"Cow inc",					12.1f, (Date) { 2000, 10, 11 }, NULL);
	matServ_addOrUpdateByNSE(matServ, -1, "Raisins",	"Good Flour SRL",			13.4f, (Date) { 2000, 12, 12 }, NULL);
	matServ_addOrUpdateByNSE(matServ, -1, "Water",		"Average Flour SRL",		14.4f, (Date) { 2022, 10, 13 }, NULL);
	matServ_addOrUpdateByNSE(matServ, -1, "Sugar",		"Swweeeet inc",				15.0f, (Date) { 2022, 10, 14 }, NULL);
	matServ_addOrUpdateByNSE(matServ, -1, "Flour",		"Bad Flour SRL",			16.9f, (Date) { 2022, 12, 15 }, NULL);
	matServ_addOrUpdateByNSE(matServ, -1, "Milk",		"Best Cow inc",				17.0f, (Date) { 2022, 03, 13 }, NULL);
	matServ_addOrUpdateByNSE(matServ, -1, "Cake Stuff",	"Rnd Bakery Stuff SRL",		18.0f, (Date) { 2022, 12, 17 }, NULL);
	matServ_addOrUpdateByNSE(matServ, -1, "Flour",		"FLOOOOOOOOOOOOOOOOUR",		19.0f, (Date) { 2022, 10, 18 }, NULL);
	matServ_addOrUpdateByNSE(matServ, -1, "Things",		"We sell stuff SRL",		20.0f, (Date) { 2022, 12, 19 }, NULL);
}
