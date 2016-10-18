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
	if(bson_iter_init(&iter, &bson) == false)
		return std::make_pair(false, "bson_iter_init failed");
	return gson(iter, obj);
}

bool gson (const user_t &obj, bson_t &bson)
{
	if(gson(bson, "id", obj.id) == false)
		return false;
	if(gson(bson, "username", obj.username) == false)
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

	if(result = bson_get_obj(iter, "list_users", &obj.list_users), !result.first)
		return result;

	if(result = bson_get_obj(iter, "vector_users", &obj.vector_users), !result.first)
		return result;

	if(result = bson_get_obj(iter, "map_users", &obj.map_users), !result.first)
		return result;

	if(result = bson_get_obj(iter, "map_list_users", &obj.map_list_users), !result.first)
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
result_t gson(bson_iter_t &iter, group_t *obj)
{
	if(obj == NULL)
		return std::make_pair(false, "group null");
	return gson(iter, *obj);
}

bool gson (const group_t& obj, bson_t &bson)
{

	if(gson(bson,"double_" ,obj.double_) == false)
		return false;

	if(gson(bson, "double_ptr_", obj.double_ptr_) == false)
		return false;

	if(gson(bson, "group_id", obj.group_id) == false)
		return false;

	if(gson(bson, "obj_id_", obj.obj_id_) == false)
		return false;

	if(gson(bson, "user", obj.user) == false)
		return false;

	if(gson(bson, "list_users", obj.list_users) == false)
		return false;

	if(gson(bson, "vector_users", obj.vector_users) == false)
		return false;

	if(gson(bson, "map_users", obj.map_users) == false)
		return false;

// 	if(gson(bson, "map_list_users", obj.map_list_users) == false)
// 		return false;

// 	if(bson_append_obj(bson, "list_map_users", obj.list_map_users) == false)
// 		return false;

	return true;
}

bool gson(const group_t *group, bson_t &bson)
{
	if(group == NULL)
		return false;
	return gson(*group, bson);
}


