// json.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <list>
#include <vector>
#include <map>
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/stdlib/json.hpp"
#include <stdio.h>
#include <iostream>
#include "struct.h"
#include "gson.h"
#include "gsoner.h"

void test_list()
{
	std::cout << "-----------------test_list--------------------" << std::endl;
	list_test obj;
	obj.a.push_back(aab{ 1,2 });
	obj.a_ptr_ptr = new std::list<aab*>;
	obj.a_ptr_ptr->push_back(new aab{ 1,2 });
	obj.a_ptr = new std::list<aab>;
	obj.a_ptr->push_back(aab{ 1,2 });
	obj.bools_.push_back(false);
	obj.bools_ptr = new std::list<bool>;
	obj.bools_ptr->push_back(false);

	acl::json json;
	acl::json_node &node = acl::gson::gson(json, obj);
	printf("%s\n", node.to_string().c_str());

	list_test obj2;
	acl::json json2;
	json2.update(node.to_string().c_str());
	std::pair<bool,std::string> ret =  acl::gson::gson(json2.get_root(), obj2);
	if (ret.first == false)
	{
		printf("%s\n\n", ret.second.c_str());
		return;
	}
	acl::json json3;
	acl::json_node &node3 = acl::gson::gson(json3, obj2);
	printf("%s\n\n", node3.to_string().c_str());
}


void test_vector()
{
	std::cout << "-----------------test_vector--------------------" << std::endl;
	vector_test obj;

	obj.a.push_back(aab{ 1,3 });
	obj.a_ptr_ptr = new std::vector<aab*>;
	obj.a_ptr_ptr->push_back(new aab{ 1,2 });
	obj.a_ptr = new std::vector<aab>;
	obj.a_ptr->push_back(aab{ 1,2 });

	acl::json json;
	acl::json_node &node = acl::gson::gson(json, obj);
	printf("%s\n", node.to_string().c_str());

	vector_test obj2;
	acl::json json2;
	json2.update(node.to_string().c_str());
	std::pair<bool, std::string> ret = acl::gson::gson(json2.get_root(), obj2);
	if(ret.first == false)
	{
		printf("%s\n", ret.second.c_str());
		return;
	}
	acl::json json3;
	acl::json_node &node3 = acl::gson::gson(json3, obj2);
	printf("%s\n", node3.to_string().c_str());
}

void test_map()
{
	std::cout << "-----------------test_map--------------------" << std::endl;
	map_test obj;
	obj.strig_map.insert({ "1","2" });
	obj.strig_map_ptr.insert({ "1",new std::string("2") });
	obj.strig_ptr_map_ptr = new std::map<std::string, std::string*>;
	obj.strig_ptr_map_ptr->insert({ "1",new std::string("2") });

	obj.aabs.insert({ "1",{1,3} });
	obj.aabs_ptr.insert({ "1" ,new aab{1,3} });
	obj.aabs_ptr_ptr = new std::map<std::string, aab*>;
	obj.aabs_ptr_ptr->insert({ "1" ,new aab{ 1,3 } });

	acl::json json;
	acl::json_node &node = acl::gson::gson(json, obj);
	printf("%s\n", node.to_string().c_str());
	map_test obj2;
	acl::json json2;
	json2.update(node.to_string().c_str());
	std::pair<bool, std::string> ret = acl::gson::gson(json2.get_root(), obj2);
	if(ret.first == false)
	{
		printf("%s\n", ret.second.c_str());
		return;
	}
	acl::json json3;
	acl::json_node &node3 = acl::gson::gson(json3, obj2);
	printf("%s\n", node3.to_string().c_str());
}


void test02()
{
	acl::gson::gsoner gr;
	gr.read_file("struct2.h");
	gr.parse_code();
}

void test04()
{
	acl::gson::gsoner gr;

	gr.read_multi_file({"struct.h" });
	gr.parse_code();
	gr.gen_gson();
}

int main(void)
{
	test_vector();
	test_list();
	test_map();
	return 0;
}
