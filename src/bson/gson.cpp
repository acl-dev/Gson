#include "gson.h"
#include "gson_helper.ipp"


bool gson (bson_iter_t &iter, user_t &obj)
{
	if (bson_get_obj(iter, "id",&obj.id) == false)
		return false;

	if(bson_get_obj(iter, "username", &obj.username) == false)
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
	if(bson_append_obj(bson, "id", obj.id) == false)
		return false;
	if(bson_append_obj(bson, "username", obj.username) == false)
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
	if(bson_get_obj(iter,"double_", &obj.double_) == false)
		return false;

	if(bson_get_obj(iter, "double_ptr_", &obj.double_ptr_) == false)
		return false;

	if (bson_get_obj(iter,"group_id", &obj.group_id) == false)
		return false;

 	if(bson_get_obj(iter, "obj_id_", &obj.obj_id_) == false)
 		return false;

	if (bson_get_obj(iter,"user", &obj.user) == false)
		return false;

	if(bson_get_obj(iter, "users", &obj.users) == false)
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

	if(bson_append_obj(bson,"double_" ,obj.double_) == false)
		return false;

	if(bson_append_obj(bson, "double_ptr_", obj.double_ptr_) == false)
		return false;

	if(bson_append_obj(bson, "group_id", obj.group_id) == false)
		return false;

	if(bson_append_obj(bson, "obj_id_", obj.obj_id_) == false)
		return false;

	if(bson_append_obj(bson, "user", obj.user) == false)
		return false;

	if(bson_append_obj(bson, "users", obj.users) == false)
		return false;

	return true;
}

bool gson(const group_t *group, bson_t &bson)
{
	if(group == NULL)
		return false;
	return gson(*group, bson);
}
