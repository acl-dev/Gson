#pragma once
#include <string>
#include <list>

struct user_t
{
	int id;
	std::string username;
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
	std::string group_id;
	user_t user;
	std::list<user_t> users;
};