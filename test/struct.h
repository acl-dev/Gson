#pragma once
#include <string>
#include <list>
#include <map>
#include <vector>


struct aab 
{
	int a;
	int b;
};
struct list_test
{
	std::list<aab> a;
 	std::list<aab> *a_ptr;
	std::list<aab*> *a_ptr_ptr;

	std::list<bool> bools_;
	std::list<bool> *bools_ptr;
};

struct vector_test
{
	std::vector<aab> a;
	std::vector<aab> *a_ptr;
	std::vector<aab*> *a_ptr_ptr;
};

struct map_test
{
	std::map<std::string, std::string> strig_map;
	std::map<std::string, std::string*> strig_map_ptr;
	std::map<std::string, std::string*> *strig_ptr_map_ptr;
	std::map<std::string, aab> aabs;
	std::map<std::string, aab*> aabs_ptr;
	std::map<std::string, aab*> *aabs_ptr_ptr;
};



