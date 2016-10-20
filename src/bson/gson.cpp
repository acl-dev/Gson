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
    
}///end of acl.
