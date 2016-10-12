#include "gson.h"
#include "gson_helper.ipp"


bool gson (bson_iter_t &iter, user_t &obj)
{
	if (bson_iter_find (&iter, "id") == false ||
		gson (iter, &obj.id) == false)
		return false;

	if (bson_iter_find (&iter, "username") == false ||
		gson (iter, &obj.username) == false)
		return false;

	return true;
}
bool gson(bson_iter_t &iter, user_t *obj)
{
	return gson(iter, *obj);
}
bool gson(bson_t &bson, user_t &obj)
{
	bson_iter_t iter;
	if(bson_iter_init(&iter, &bson) == false)
		return false;
	return gson(iter, obj);
}

bool gson (const user_t &obj, bson_t &bson)
{
	if (check_nullptr(obj.id) == false ||
		bson_append_int32 (&bson, "id", -1, obj.id) == false)
		return false;

	if (bson_append_utf8 (&bson, "username", -1,
		get_value(obj.username), 
		strlen(get_value(obj.username))) == false)

		return false;
	return true;
}
bool gson(const user_t *user, bson_t &bson)
{
	if(user == NULL)
		return false;
	return gson(*user, bson);
}

bool gson (bson_iter_t &iter, group_t &obj)
{
	bson_iter_t user;
	bson_iter_t users;

	if (bson_iter_find (&iter, "group_id") == false ||
		gson (iter, &obj.group_id) == false)
		return false;

	if (bson_iter_find (&iter, "user") == false ||
		bson_iter_recurse (&iter, &user) == false ||
		gson (user, obj.user) == false)
		return false;

	if (bson_iter_find (&iter, "users") == false ||
		bson_iter_recurse (&iter, &users) == false ||
		gson (users, &obj.users) == false)
		return false;

	return true;
}
bool gson(bson_t &bson, group_t &obj)
{
	bson_iter_t iter;
	if(bson_iter_init(&iter, &bson) == false)
		return false;
	return gson(iter, obj);
}
bool gson (const group_t& obj, bson_t &bson)
{
	bson_t user;
	bson_t users;
	bson_init (&user);
	bson_init (&users);
	destroy_bson_t d_user (user);
	destroy_bson_t d_users (users);

	if(check_nullptr(obj.double_) == false ||
	   bson_append_double(&bson, "double_", -1, get_value(obj.double_)) == false)
		return false;

	if (bson_append_utf8 (&bson, "group_id", -1,get_value(obj.group_id),
		strlen(get_value(obj.group_id))) == false)
		return false;

	if (gson (obj.user, user) == false)
		return false;
	if (bson_append_document (&bson, "user", -1, &user) == false)
		return false;

	if (gson (obj.users, users) == false)
		return false;
	if (bson_append_array (&bson, "users", -1, &users) == false)
		return false;

	return true;
}

bool gson(const group_t *group, bson_t &bson)
{
	if(group == NULL)
		return false;
	return gson(*group, bson);
}
