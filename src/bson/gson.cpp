#include "bson.h"
#include "struct.h"
#include "gson.h"
#include "gson_helper.ipp"
namespace acl
{
    bool gson (const group_t&$obj, bson_t &$bson)
    {

        if(bson_append_double(&$bson, "double_", -1, get_value($obj.double_)) == false)
            return false;

        if(bson_append_double(&$bson, "double_ptr_", -1, get_value($obj.double_ptr_)) == false)
            return false;

        if(bson_append_utf8(&$bson, "group_id", -1, get_value($obj.group_id), -1) == false)
            return false;

        if(bson_append_oid(&$bson,"obj_id_", -1, &$obj.obj_id_) == false)
            return false;

        bson_t user;
        bson_init(&user);
        destroy_bson_t destroy_user(user);
        if(gson($obj.user, user) == false)
            return false;
        if(bson_append_document(&$bson,"user", -1, &user) == false)
            return false;

        bson_t  list_users;
        bson_init(&list_users);
        destroy_bson_t destroy_list_users(list_users);
        if(gson($obj.list_users,list_users) == false)
            return false;
        if(bson_append_array(&$bson,"list_users", -1, &list_users) == false)
            return false;

        bson_t  vector_users;
        bson_init(&vector_users);
        destroy_bson_t destroy_vector_users(vector_users);
        if(gson($obj.vector_users,vector_users) == false)
            return false;
        if(bson_append_array(&$bson,"vector_users", -1, &vector_users) == false)
            return false;

        bson_t map_users;
        bson_init(&map_users);
        destroy_bson_t destroy_map_users(map_users);
        if(gson($obj.map_users,map_users) == false)
            return false;
        if(bson_append_document(&$bson,"map_users", -1, &map_users) == false)
            return false;

        bson_t map_list_users;
        bson_init(&map_list_users);
        destroy_bson_t destroy_map_list_users(map_list_users);
        if(gson($obj.map_list_users,map_list_users) == false)
            return false;
        if(bson_append_document(&$bson,"map_list_users", -1, &map_list_users) == false)
            return false;

        bson_t  list_map_users;
        bson_init(&list_map_users);
        destroy_bson_t destroy_list_map_users(list_map_users);
        if(gson($obj.list_map_users,list_map_users) == false)
            return false;
        if(bson_append_array(&$bson,"list_map_users", -1, &list_map_users) == false)
            return false;

        return true;
    }
    bool gson (const group_t*$obj, bson_t &$bson)
    {
        if($obj == NULL)
            return false;
        return gson(*$obj,$bson);
    }
    
    result_t gson(bson_iter_t &$itr,group_t $obj)
    {

        result_t result;
        if(bson_iter_find(&$itr,"double_") == false)
            return std::make_pair(false, "bison_iter_find [double_] failed");
        if(result = gson($itr, &$obj.double_), !result.first)
            return std::make_pair(false, "gson[double_] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"double_ptr_") == false)
            return std::make_pair(false, "bison_iter_find [double_ptr_] failed");
        if(result = gson($itr, &$obj.double_ptr_), !result.first)
            return std::make_pair(false, "gson[double_ptr_] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"group_id") == false)
            return std::make_pair(false, "bison_iter_find [group_id] failed");
        if(result = gson($itr, &$obj.group_id), !result.first)
            return std::make_pair(false, "gson[group_id] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"obj_id_") == false)
            return std::make_pair(false, "bison_iter_find [obj_id_] failed");
        if(result = gson($itr, &$obj.obj_id_), !result.first)
            return std::make_pair(false, "gson[obj_id_] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"user") == false)
            return std::make_pair(false, "bison_iter_find [user] failed.");
        bson_iter_t user_iter;
        if(bson_iter_recurse(&$itr, &user_iter) == false)
            return std::make_pair(false, "bson_iter_recurse [user] failed.");
        if(result = gson(user_iter, &$obj.user), !result.first)
            return std::make_pair(false, "gson[user] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"list_users") == false)
            return std::make_pair(false, "bison_iter_find [list_users] failed.");
        bson_iter_t list_users_iter;
        if(bson_iter_recurse(&$itr, &list_users_iter) == false)
            return std::make_pair(false, "bson_iter_recurse [list_users] failed.");
        if(result = gson(list_users_iter, &$obj.list_users), !result.first)
            return std::make_pair(false, "gson[list_users] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"vector_users") == false)
            return std::make_pair(false, "bison_iter_find [vector_users] failed");
        if(result = gson($itr, &$obj.vector_users), !result.first)
            return std::make_pair(false, "gson[vector_users] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"map_users") == false)
            return std::make_pair(false, "bison_iter_find [map_users] failed.");
        bson_iter_t map_users_iter;
        if(bson_iter_recurse(&$itr, &map_users_iter) == false)
            return std::make_pair(false, "bson_iter_recurse [map_users] failed.");
        if(result = gson(map_users_iter, &$obj.map_users), !result.first)
            return std::make_pair(false, "gson[map_users] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"map_list_users") == false)
            return std::make_pair(false, "bison_iter_find [map_list_users] failed.");
        bson_iter_t map_list_users_iter;
        if(bson_iter_recurse(&$itr, &map_list_users_iter) == false)
            return std::make_pair(false, "bson_iter_recurse [map_list_users] failed.");
        if(result = gson(map_list_users_iter, &$obj.map_list_users), !result.first)
            return std::make_pair(false, "gson[map_list_users] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"list_map_users") == false)
            return std::make_pair(false, "bison_iter_find [list_map_users] failed.");
        bson_iter_t list_map_users_iter;
        if(bson_iter_recurse(&$itr, &list_map_users_iter) == false)
            return std::make_pair(false, "bson_iter_recurse [list_map_users] failed.");
        if(result = gson(list_map_users_iter, &$obj.list_map_users), !result.first)
            return std::make_pair(false, "gson[list_map_users] failed:["+result.second+"]");

        return std::make_pair(true,"");
    }
    
    bool gson (const user_t&$obj, bson_t &$bson)
    {

        if(bson_append_int32(&$bson, "id", -1, get_value($obj.id)) == false)
            return false;

        if(bson_append_utf8(&$bson, "username", -1, get_value($obj.username), -1) == false)
            return false;

        return true;
    }
    bool gson (const user_t*$obj, bson_t &$bson)
    {
        if($obj == NULL)
            return false;
        return gson(*$obj,$bson);
    }
    
    result_t gson(bson_iter_t &$itr,user_t $obj)
    {

        result_t result;
        if(bson_iter_find(&$itr,"id") == false)
            return std::make_pair(false, "bison_iter_find [id] failed");
        if(result = gson($itr, &$obj.id), !result.first)
            return std::make_pair(false, "gson[id] failed:["+result.second+"]");

        if(bson_iter_find(&$itr,"username") == false)
            return std::make_pair(false, "bison_iter_find [username] failed");
        if(result = gson($itr, &$obj.username), !result.first)
            return std::make_pair(false, "gson[username] failed:["+result.second+"]");

        return std::make_pair(true,"");
    }
    
}///end of acl.
