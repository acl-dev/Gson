#include "gson.h"
#include "gson_helper.ipp"


std::pair<bool, std::string> 
gson (bson_iter_t &iter, user_t &obj)
{
	std::pair<bool, std::string> result;
	result.first = true;
	result.second = "";

	if ( result = bson_get_obj(iter, "id",&obj.id), !result.first)
		return result;

	if(result = bson_get_obj(iter, "username", &obj.username), !result.first)
		return result;

	return result;
}
std::pair<bool, std::string> gson(bson_iter_t &iter, user_t *obj)
{
	return gson(iter, *obj);
}
std::pair<bool, std::string> gson(bson_t &bson, user_t &obj)
{
	bson_iter_t iter;
	std::pair<bool, std::string> result;
	result.first = true;
	result.second = "";
	if(bson_iter_init(&iter, &bson) == false)
		return std::make_pair(false, "bson_iter_init failed");
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

result_t gson (bson_iter_t &iter, group_t &obj)
{
	std::pair<bool, std::string> result(true,"");

	if(result = bson_get_obj(iter,"double_", &obj.double_), !result.first)
		return result;

	if(result = bson_get_obj(iter, "double_ptr_", &obj.double_ptr_), !result.first)
		return result;

	if (result = bson_get_obj(iter,"group_id", &obj.group_id), !result.first)
		return result;

 	if(result = bson_get_obj(iter, "obj_id_", &obj.obj_id_), !result.first)
 		return result;

	if (result = bson_get_obj(iter,"user", &obj.user), !result.first)
		return result;

	if(result = bson_get_obj(iter, "users", &obj.users), !result.first)
		return result;

	return result;
}
result_t gson(bson_t &bson, group_t &obj)
{
	bson_iter_t iter;
	if(bson_iter_init(&iter, &bson) == false)
		return std::make_pair(false, "bson_iter_init");
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
