#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "gson.h"
#include "gson_helper.ipp"

void test1 ()
{
	const char *json = "{ \"group_id\" : \"12345\", \"user\" : { \"id\" : 1, \"username\" : \"akzi\" }, \"users\" : [ { \"id\" : 1, \"username\" : \"akzi\" }, { \"id\" : 2, \"username\" : \"akzi2\" } ] }";

	bson_t bson;
	assert (bson_init_from_json (&bson, json, -1, NULL));
	bson_iter_t iter;
	assert (bson_iter_init (&iter, &bson));
	group_t group;
	gson (iter, group);
	bson_t bson2;
	bson_init (&bson2);
	gson (group, bson2);
	char *str = bson_as_json (&bson2, 0);

	std::string text = to_text (str);

	bson_free (str);
	bson_destroy (&bson);
	bson_destroy (&bson2);
	return ;
}


int main ()
{
	for (int i = 0; i < 10000000; i ++)
	{
		test1 ();
	}
	return 0;
}