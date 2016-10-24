#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <string.h>
#include "bson.h"
#include "struct.h"
#include "gson.h"

#include "gsoner.hpp"
#include "gson_helper.ipp"

void test1 ()
{
	const char *json = "{ \"double_\" : 0.11111, \"double_ptr_\" : 0.2222, \"group_id\" : \"12345\", \"obj_id_\" : { \"$oid\" : \"353830333063306239316630\" }, \"user\" : { \"id\" : 1, \"username\" : \"akzi\" }, \"list_users\" : [ { \"id\" : 1, \"username\" : \"akzi\" }, { \"id\" : 2, \"username\" : \"akzi2\" } ], "
		"\"vector_users\" : [ { \"id\" : 1, \"username\" : \"akzi\" }, { \"id\" : 2, \"username\" : \"akzi2\" } ], "
		"\"map_users\" : { \"user1\":{ \"id\" : 1, \"username\" : \"akzi\" }, \"user2\" : { \"id\" : 2, \"username\" : \"akzi2\" } }, \"map_list_users\":{\"list1\" : [ { \"id\" : 1, \"username\" : \"akzi\" }, { \"id\" : 2, \"username\" : \"akzi2\" } ] }, "
		"\"list_map_users\":[{ \"user1\":{ \"id\" : 1, \"username\" : \"akzi\" }, \"user2\" : { \"id\" : 2, \"username\" : \"akzi2\" } } ] }";
	bson_t bson;

	assert (bson_init_from_json (&bson, json, -1, NULL));

	group_t group;
	acl:: result_t res = acl::gson(bson, group);
	assert(res.first);
	
	bson_t bson2;
	bson_init (&bson2);
	assert(acl::gson (group, bson2));
	char *str1 = bson_as_json (&bson2, 0);
	char *str2 = bson_as_json(&bson, 0);

	std::string text = acl::to_text(str1);
	std::cout << (strcmp(str2, str1) == 0 ? "OK" : "failed");

	bson_free (str1);
	bson_free(str2);
	bson_destroy (&bson);
	bson_destroy (&bson2);
	return ;
}

void tt(int)
{
	acl::gsoner gr;
	gr.read_file("struct.h");
	gr.parse_code();
	gr.gen_bson();
}
int main ()
{
	//tt(0);
	//return 0;
 	for (int i = 0; i < 10000000; i ++)
 	{
 		test1 ();
 	}
	return 0;
}