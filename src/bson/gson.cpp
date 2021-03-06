#include "bson.h"
#include "struct.h"
#include "gson.h"
#include "gson_helper.ipp"
namespace acl
{
    bool gson (const group_t&$obj, bson_t &$bson)
    {

        // double_
        if(bson_append_double(&$bson, "double_", -1, get_value($obj.double_)) == false)
            return false;

        // double_ptr_
        if(bson_append_double(&$bson, "double_ptr_", -1, get_value($obj.double_ptr_)) == false)
            return false;

        // group_id
        if(bson_append_utf8(&$bson, "group_id", -1, get_value($obj.group_id), -1) == false)
            return false;

        // obj_id_
        if(bson_append_oid(&$bson,"obj_id_", -1, &$obj.obj_id_) == false)
            return false;

        // user
        bson_t user;
        bson_init(&user);
        destroy_bson_t destroy_user(user);
        if(gson($obj.user, user) == false)
            return false;
        if(bson_append_document(&$bson,"user", -1, &user) == false)
            return false;

        // list_users
        bson_t  list_users;
        bson_init(&list_users);
        destroy_bson_t destroy_list_users(list_users);
        if(gson($obj.list_users,list_users) == false)
            return false;
        if(bson_append_array(&$bson,"list_users", -1, &list_users) == false)
            return false;

        // vector_users
        bson_t  vector_users;
        bson_init(&vector_users);
        destroy_bson_t destroy_vector_users(vector_users);
        if(gson($obj.vector_users,vector_users) == false)
            return false;
        if(bson_append_array(&$bson,"vector_users", -1, &vector_users) == false)
            return false;

        // map_users
        bson_t map_users;
        bson_init(&map_users);
        destroy_bson_t destroy_map_users(map_users);
        if(gson($obj.map_users,map_users) == false)
            return false;
        if(bson_append_document(&$bson,"map_users", -1, &map_users) == false)
            return false;

        // map_list_users
        bson_t map_list_users;
        bson_init(&map_list_users);
        destroy_bson_t destroy_map_list_users(map_list_users);
        if(gson($obj.map_list_users,map_list_users) == false)
            return false;
        if(bson_append_document(&$bson,"map_list_users", -1, &map_list_users) == false)
            return false;

        // list_map_users
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
    result_t gson(bson_iter_t &$itr,group_t &$obj)
    {

        result_t result;
        // double_
        if(bson_iter_find(&$itr,"double_"))
              gson($itr, &$obj.double_);

        // double_ptr_
        if(bson_iter_find(&$itr,"double_ptr_"))
              gson($itr, &$obj.double_ptr_);

        // group_id
        if(bson_iter_find(&$itr,"group_id"))
              gson($itr, &$obj.group_id);

        // obj_id_
        if(bson_iter_find(&$itr,"obj_id_"))
              gson($itr, &$obj.obj_id_);

        // user
        if(bson_iter_find(&$itr,"user"))
        {
              bson_iter_t user_iter;
              if(bson_iter_recurse(&$itr, &user_iter))
                    gson(user_iter, &$obj.user);
         }
        // list_users
        if(bson_iter_find(&$itr,"list_users"))
        {
              bson_iter_t list_users_iter;
              if(bson_iter_recurse(&$itr, &list_users_iter))
                    gson(list_users_iter, &$obj.list_users);
         }
        // vector_users
        if(bson_iter_find(&$itr,"vector_users"))
        {
              bson_iter_t vector_users_iter;
              if(bson_iter_recurse(&$itr, &vector_users_iter))
                    gson(vector_users_iter, &$obj.vector_users);
         }
        // map_users
        if(bson_iter_find(&$itr,"map_users"))
        {
              bson_iter_t map_users_iter;
              if(bson_iter_recurse(&$itr, &map_users_iter))
                    gson(map_users_iter, &$obj.map_users);
         }
        // map_list_users
        if(bson_iter_find(&$itr,"map_list_users"))
        {
              bson_iter_t map_list_users_iter;
              if(bson_iter_recurse(&$itr, &map_list_users_iter))
                    gson(map_list_users_iter, &$obj.map_list_users);
         }
        // list_map_users
        if(bson_iter_find(&$itr,"list_map_users"))
        {
              bson_iter_t list_map_users_iter;
              if(bson_iter_recurse(&$itr, &list_map_users_iter))
                    gson(list_map_users_iter, &$obj.list_map_users);
         }
        return std::make_pair(true,"");
    }
    result_t gson(bson_iter_t &$itr,group_t *$obj)
    {

        if($obj == NULL)
            return std::make_pair(false,"group_t null");
        return gson($itr, *$obj);
    }
    result_t gson(bson_t &bson,group_t &obj)
    {
        bson_iter_t iter;
        if(!bson_iter_init(&iter, &bson))
             return std::make_pair(false,"bson_iter_init fail");
        return gson(iter,obj);
    }

    bool gson (const user_t&$obj, bson_t &$bson)
    {

        // id
        if(bson_append_int32(&$bson, "id", -1, get_value($obj.id)) == false)
            return false;

        // username
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
    result_t gson(bson_iter_t &$itr,user_t &$obj)
    {

        result_t result;
        // id
        if(bson_iter_find(&$itr,"id") == false)
            return std::make_pair(false, "bison_iter_find [id] failed");
        if(result = gson($itr, &$obj.id), !result.first)
            return std::make_pair(false, "gson[id] failed:["+result.second+"]");

        // username
        if(bson_iter_find(&$itr,"username") == false)
            return std::make_pair(false, "bison_iter_find [username] failed");
        if(result = gson($itr, &$obj.username), !result.first)
            return std::make_pair(false, "gson[username] failed:["+result.second+"]");

        return std::make_pair(true,"");
    }
    result_t gson(bson_iter_t &$itr,user_t *$obj)
    {

        if($obj == NULL)
            return std::make_pair(false,"user_t null");
        return gson($itr, *$obj);
    }
    result_t gson(bson_t &bson,user_t &obj)
    {
        bson_iter_t iter;
        if(!bson_iter_init(&iter, &bson))
             return std::make_pair(false,"bson_iter_init fail");
        return gson(iter,obj);
    }

}///end of acl.
