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

bool gson (const user_t &user, bson_t &bson, result_t *result)
{
	if (bson_append_int32 (&bson, "id", -1, user.id) == false)
		return false;
	if (bson_append_utf8 (&bson, "username", -1, user.username.c_str (), user.username.size ()) == false)
		return false;
	return true;
}
bool gson(const user_t *user, bson_t &bson, result_t *result)
{
	if(user == NULL)
		return false;
	return gson(*user, bson, result);
}

bool gson (bson_iter_t &bson_iter, group_t &obj)
{
	bson_iter_t user;
	bson_iter_t users;

	if (bson_iter_find (&bson_iter, "group_id") == false ||
		gson (bson_iter, &obj.group_id) == false)
		return false;

	if (bson_iter_find (&bson_iter, "user") == false ||
		bson_iter_recurse (&bson_iter, &user) == false ||
		gson (user, obj.user) == false)
		return false;

	if (bson_iter_find (&bson_iter, "users") == false ||
		bson_iter_recurse (&bson_iter, &users) == false ||
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
bool gson (const group_t& obj, bson_t &bson, result_t *result)
{
	bson_t user;
	bson_t users;
	bson_init (&user);
	bson_init (&users);
	destroy_bson_t d_user (user);
	destroy_bson_t d_users (users);

	if(check_value(obj.double_, result, "group_t.double_") == false ||
	   bson_append_double(&bson, "double_", -1, get_value(obj.double_)) == false)
		return false;

	if (bson_append_utf8 (&bson, "group_id", -1, obj.group_id.c_str (), obj.group_id.size ()) == false)
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

bool gson(const group_t *group, bson_t &bson, result_t *result)
{
	if(group == NULL)
		return false;
	return gson(*group, bson, result);
}
