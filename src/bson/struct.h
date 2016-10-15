#pragma once
#include <string>
#include <list>
#include "bson-types.h"

struct user_t
{
	int id;
	std::string *username;
	user_t ()
	{

	}
	user_t (int, std::string &&username)
		:id (int{((0))})
	{

	}
};

struct group_t
{
	double double_;
	double *double_ptr_;
	std::string group_id;
	bson_oid_t obj_id_;
	user_t user;
	std::list<user_t> users;
};