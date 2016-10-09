#pragma once
#include "bson.h"
#include <string>
#include <list>

struct destroy_bson_t
{
	destroy_bson_t (bson_t &bson)
		:bson_ (bson)
	{

	}
	~destroy_bson_t ()
	{
		bson_destroy (&bson_);
	}
	bson_t &bson_;
};

static inline bool gson (bson_iter_t &itr, int32_t &obj)
{
	bson_type_t type = bson_iter_type (&itr);
	if (type == BSON_TYPE_INT32 ||
		type == BSON_TYPE_INT64)
	{
		obj = bson_iter_int32 (&itr);
		return true;
	}
	return false;
}
static inline bool gson (bson_iter_t &itr, std::string &obj)
{
	bson_type_t type = bson_iter_type (&itr);
	if (type == BSON_TYPE_UTF8)
	{
		size_t len = 0;
		const char *buf = bson_iter_utf8 (&itr, &len);
		if (buf)
		{
			obj.append (buf, len);
			return true;
		}
	}
	return false;
}

static inline bool gson (const std::list<user_t> &users, bson_t &bson)
{
	int i = 0;
	for (auto &itr : users)
	{
		bson_t user;
		destroy_bson_t d_user (user);
		bson_init (&user);
		if (gson (itr, user) == false)
			return false;
		if (bson_append_document (&bson, std::to_string (i).c_str (), -1, &user) == false)
			return false;
		i++;
	}
	return true;
}
static inline bool gson (bson_iter_t &iter, std::list<user_t> &obj)
{
	while (bson_iter_next (&iter))
	{
		bson_iter_t user_iter;
		if (bson_iter_recurse (&iter, &user_iter) == false)
			return false;
		user_t user;
		if (gson (user_iter, user) == false)
			return false;
		obj.push_back (user);
	}
	return !!obj.size ();
}

static inline std::string to_text(const std::string &str)
{
	std::string result;
	int pos = 0;
	result.append ("\"");
	while (pos < str.size())
	{
		if(str[pos] == '\\r')
		{
			result.append ("\\r");
		}
		else if (str[pos] == '\\n')
		{
			result.append ("\\n");
		}
		else if (str[pos] == '\\t')
		{
			result.append ("\\t");
		}
		else if (str[pos] == '"')
		{
			result.append ("\\\"");
		}
		else
		{
			result.push_back(str[pos]);
		}
		pos++;
	}
	result.append ("\"");
	return result;
}