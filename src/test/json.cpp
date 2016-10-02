// json.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <list>
#include <vector>
#include <map>
#include <fstream>
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/stdlib/json.hpp"
#include "gson_helper.ipp"
#include "gsoner.h"
#include <stdio.h>
#include <iostream>

using namespace std;


struct auth_t
{
	int id;
	std::string *password;
};

struct user_t
{
	std::string username_;
	int id;
	std::list<auth_t*> *auth;
	char *c_str;
};

struct group_t
{
	std::string group_id_;
	std::list<user_t> user_list_;
	std::list<user_t*> user_ptr_list_;
	std::list<user_t*> *user_ptr_list_ptr_;
	std::list<std::list<user_t>> user_list_list_;
	std::vector<user_t> vector_user_;
	std::vector<user_t*> vector_user_ptr_;
	std::map<std::string ,user_t*> map_user_ptr_;
	user_t user;
};
namespace acl
{
	namespace gson
	{
		template<class T>
		void del(T **obj)
		{
			delete *obj;
			*obj = NULL;
		}
		template<class T>
		void del(T *obj)
		{
			;
		}
		//double
		std::pair<bool, std::string> gson(acl::json_node &node, auth_t *obj);

		template <class T>
		typename std::enable_if<std::is_floating_point<T>::value,
			std::pair<bool, std::string>>::type
			gson(acl::json_node &node, T *&obj)
		{
			if(node.is_double() == false)
				return std::make_pair(false, "get double failed");
			obj = static_cast<T>(*node.get_double());
			return std::make_pair(true, "");
		}

		template <class T>
		typename std::enable_if<std::is_floating_point<T>::value,
			std::pair<bool, std::string>>::type
			gson(acl::json_node &node, T **obj)
		{
			if(node.is_double() == false)
				return std::make_pair(false, "get double failed");;
			obj = new T;
			*obj = static_cast<T>(*node.get_double());
			return std::make_pair(true, "");
		}
		//intergral
		template <class T>
		typename std::enable_if<std::is_integral<T>::value, 
			std::pair<bool, std::string>>::type
		gson(acl::json_node &node, T *obj)
		{
			if(node.is_number() == false)
				return std::make_pair(false, "get number failed");
			*obj = static_cast<T>(*node.get_int64());
			return std::make_pair(true, "");
		}

		template <class T>
		typename std::enable_if<std::is_integral<T>::value,
			std::pair<bool, std::string>>::type
			gson(acl::json_node &node, T **obj)
		{
			if(node.is_number() == false)
				return std::make_pair(false, "get number failed");;
			*obj = new T;
			*obj = static_cast<T>(*node.get_int64());
			return std::make_pair(true, "");
		}
		//string
		std::pair<bool, std::string>
			gson(acl::json_node &node, char **obj)
		{
			if(node.is_string() == false)
				return std::make_pair(false, "get char * string failed");
			int len = strlen(node.get_string());
			*obj = new char[len+1];
			memcpy(*obj, node.get_string(), len);
			(*obj)[len] = 0;
			return std::make_pair(true, "");
		}

		std::pair<bool,std::string>
			gson(acl::json_node &node, std::string *obj)
		{
			if(node.is_string() == false)
				return std::make_pair(false,"get string failed");
			obj->append(node.get_string());
			return std::make_pair(true, "");
		}

		std::pair<bool, std::string>
			gson(acl::json_node &node, acl::string *obj)
		{
			if(node.is_string() == false)
				return std::make_pair(false, "get string failed");
			obj->append(node.get_string());
			return std::make_pair(true, "");
		}

		std::pair<bool, std::string>
			gson(acl::json_node &node, std::string **obj)
		{
			if(node.is_string() == false)
				return std::make_pair(false, "get string failed");
			*obj = new std::string;
			(*obj)->append(node.get_string());
			return std::make_pair(true, "");
		}

		std::pair<bool, std::string>
			gson(acl::json_node &node, acl::string **obj)
		{
			if(node.is_string() == false)
				return std::make_pair(false,"get string failed");
			*obj = new acl::string;
			(*obj)->append(node.get_string());
			return std::make_pair(true, "");
		}
		
		template<class T>
		std::pair<bool, std::string>
			gson(acl::json_node &node, std::list<T> *objs);
		template<class T>
		std::pair<bool, std::string>
			gson(acl::json_node *node, std::list<const T *> &objs);

		template <class T>
		typename std::enable_if<std::is_class<T>::value, std::pair<bool, std::string>>::type
			gson(acl::json_node &node, T **obj)
		{
			*obj = new T();
			std::pair<bool, std::string> result = gson(node, *obj);
			if(result.first == false)
			{
				delete *obj;
				*obj = NULL;
			}
			return result;
		}

		
		template<class T>
		std::pair<bool, std::string>
			gson(acl::json_node &node, std::list<T> *objs)
		{
			std::pair<bool, std::string> result;
			std::string error_string;
			acl::json_node *itr = node.first_child();
			while(itr)
			{
				T obj;
				result = gson(*itr, &obj);
				if(result.first)
					objs->push_back(obj);
				else
					error_string.append(result.second);
				itr = node.next_child();
			}
			return std::make_pair(!!!objs->empty(), error_string);
		}

		template<class T>
		std::pair<bool, std::string>
			gson(acl::json_node *node, std::list<const T *> &objs)
		{
			std::pair<bool, std::string> result;
			acl::json_node *itr = node->first_child();
			std::string error_string;
			while(itr)
			{
				T *obj = new T;
				result = gson(itr, obj);
				if(result.first)
					objs.push_back(obj);
				else
				{
					error_string.append(result.second);
					delete obj;
				}
				itr = node->next_child();
			}
			return std::make_pair(!!!objs.empty(),error_string);
		}

		
		//vector
		template<class T>
		std::pair<bool, std::string>
			gson(acl::json_node &node, std::vector<T> *objs)
		{
			std::pair<bool, std::string> result;
			acl::json_node *itr = node.first_child();
			std::string error_string;
			while(itr)
			{
				T obj;
				result = gson(*itr, &obj);
				if(result.first)
					objs->push_back(obj);
				else
					error_string.append(result.second);
				itr = node.next_child();
				//todo delete obj when failed
			}
			return std::make_pair(!!!objs->empty(), error_string);
		}
		

		template<class T>
		std::pair<bool, std::string>
			expand(acl::json_node &node, std::map<std::string, T> *objs)
		{
			std::pair<bool, std::string> result;
			acl::json_node *itr = node.first_child();
			while (itr && itr->get_obj())
			{
				T obj;
				result = gson(*(itr->get_obj()), &obj);
				if(result.first)
					objs->insert(
					std::make_pair(std::string(itr->tag_name()), obj));
				else
					break;
				itr = node.next_child();
			}
			if(result.first == false)
			{
				for (std::map<std::string, T>::iterator itr = objs->begin();
					 itr != objs->end() ;++itr)
				{
					del(&itr->second);
				}
				objs->clear();
				return result;
			}
			return std::make_pair(true, "");
		}
		//map
		template<class T>
		std::pair<bool, std::string>
			gson(acl::json_node &node, std::map<std::string, T> *objs)
		{
			std::pair<bool, std::string> result;
			acl::json_node *itr = node.first_child();
			std::string error_string;
			while(itr)
			{
				result = expand(*itr, objs);
				if(result.first == false)
					error_string.append(result.second);
				itr = node.next_child();
			}
			return std::make_pair(!!!objs->empty(), error_string);
		}

		//////////////////////////////////////////////////////////////////////////
		acl::json_node& gson(acl::json & json, const auth_t &obj)
		{
			acl::json_node &node = json.create_node();
			node.add_number("id", acl::gson::get_value(obj.id));
			node.add_text("password", acl::gson::get_value(obj.password));

			return node;
		}
		acl::json_node& gson(acl::json & json, const auth_t*obj)
		{
			return gson(json, *obj);
		}


		acl::string gson(const auth_t &obj)
		{
			acl::json json;
			acl::json_node &node = acl::gson::gson(json, obj);
			return node.to_string();
		}


		std::pair<bool, std::string> gson(acl::json_node &node, auth_t &obj)
		{
			acl::json_node *id = node["id"];
			acl::json_node *password = node["password"];
			std::pair<bool, std::string> result;

			if(!id || !(result = gson(*id, &obj.id), result.first))
				return std::make_pair(false, "required [auth_t.id] failed:{" + result.second + "}");

			if(!password || !(result = gson(*password, &obj.password), result.first))
				return std::make_pair(false, "required [auth_t.password] failed:{" + result.second + "}");

			return std::make_pair(true, "");
		}


		std::pair<bool, std::string> gson(acl::json_node &node, auth_t *obj)
		{
			return gson(node, *obj);
		}


		acl::json_node& gson(acl::json & json, const user_t &obj)
		{
			acl::json_node &node = json.create_node();
			node.add_text("username_", acl::gson::get_value(obj.username_));
			node.add_number("id", acl::gson::get_value(obj.id));
			node.add_child("auth", acl::gson::gson(json, obj.auth));
			node.add_text("c_str", acl::gson::get_value(obj.c_str));

			return node;
		}
		acl::json_node& gson(acl::json & json, const user_t*obj)
		{
			return gson(json, *obj);
		}


		acl::string gson(const user_t &obj)
		{
			acl::json json;
			acl::json_node &node = acl::gson::gson(json, obj);
			return node.to_string();
		}


		std::pair<bool, std::string> gson(acl::json_node &node, user_t &obj)
		{
			acl::json_node *username_ = node["username_"];
			acl::json_node *id = node["id"];
			acl::json_node *auth = node["auth"];
			acl::json_node *c_str = node["c_str"];
			std::pair<bool, std::string> result;

			if(!username_ || !(result = gson(*username_, &obj.username_), result.first))
				return std::make_pair(false, "required [user_t.username_] failed:{" + result.second + "}");

			if(!id || !(result = gson(*id, &obj.id), result.first))
				return std::make_pair(false, "required [user_t.id] failed:{" + result.second + "}");

			if(!auth || !auth->get_obj() || !(result = gson(*auth->get_obj(), &obj.auth), result.first))
				return std::make_pair(false, "required [user_t.auth] failed:{" + result.second + "}");

			if(!c_str || !(result = gson(*c_str, &obj.c_str), result.first))
				return std::make_pair(false, "required [user_t.c_str] failed:{" + result.second + "}");

			return std::make_pair(true, "");
		}


		std::pair<bool, std::string> gson(acl::json_node &node, user_t *obj)
		{
			return gson(node, *obj);
		}


		acl::json_node& gson(acl::json & json, const group_t &obj)
		{
			acl::json_node &node = json.create_node();
			node.add_text("group_id_", acl::gson::get_value(obj.group_id_));
			node.add_child("user_list_", acl::gson::gson(json, obj.user_list_));
			node.add_child("user_ptr_list_", acl::gson::gson(json, obj.user_ptr_list_));
			node.add_child("user_ptr_list_ptr_", acl::gson::gson(json, obj.user_ptr_list_ptr_));
			node.add_child("user_list_list_", acl::gson::gson(json, obj.user_list_list_));
			node.add_child("vector_user_", acl::gson::gson(json, obj.vector_user_));
			node.add_child("vector_user_ptr_", acl::gson::gson(json, obj.vector_user_ptr_));
			node.add_child("map_user_ptr_", acl::gson::gson(json, obj.map_user_ptr_));
			node.add_child("user", acl::gson::gson(json, obj.user));

			return node;
		}
		acl::json_node& gson(acl::json & json, const group_t*obj)
		{
			return gson(json, *obj);
		}


		acl::string gson(const group_t &obj)
		{
			acl::json json;
			acl::json_node &node = acl::gson::gson(json, obj);
			return node.to_string();
		}


		std::pair<bool, std::string> gson(acl::json_node &node, group_t &obj)
		{
			acl::json_node *group_id_ = node["group_id_"];
			acl::json_node *user_list_ = node["user_list_"];
			acl::json_node *user_ptr_list_ = node["user_ptr_list_"];
			acl::json_node *user_ptr_list_ptr_ = node["user_ptr_list_ptr_"];
			acl::json_node *user_list_list_ = node["user_list_list_"];
			acl::json_node *vector_user_ = node["vector_user_"];
			acl::json_node *vector_user_ptr_ = node["vector_user_ptr_"];
			acl::json_node *map_user_ptr_ = node["map_user_ptr_"];
			acl::json_node *user = node["user"];
			std::pair<bool, std::string> result;

			if(!group_id_ || !(result = gson(*group_id_, &obj.group_id_), result.first))
				return std::make_pair(false, "required [group_t.group_id_] failed:{" + result.second + "}");

			if(!user_list_ || !user_list_->get_obj() || !(result = gson(*user_list_->get_obj(), &obj.user_list_), result.first))
				return std::make_pair(false, "required [group_t.user_list_] failed:{" + result.second + "}");

			if(!user_ptr_list_ || !user_ptr_list_->get_obj() || !(result = gson(*user_ptr_list_->get_obj(), &obj.user_ptr_list_), result.first))
				return std::make_pair(false, "required [group_t.user_ptr_list_] failed:{" + result.second + "}");

			if(!user_ptr_list_ptr_ || !user_ptr_list_ptr_->get_obj() || !(result = gson(*user_ptr_list_ptr_->get_obj(), &obj.user_ptr_list_ptr_), result.first))
				return std::make_pair(false, "required [group_t.user_ptr_list_ptr_] failed:{" + result.second + "}");

			if(!user_list_list_ || !user_list_list_->get_obj() || !(result = gson(*user_list_list_->get_obj(), &obj.user_list_list_), result.first))
				return std::make_pair(false, "required [group_t.user_list_list_] failed:{" + result.second + "}");

			if(!vector_user_ || !vector_user_->get_obj() || !(result = gson(*vector_user_->get_obj(), &obj.vector_user_), result.first))
				return std::make_pair(false, "required [group_t.vector_user_] failed:{" + result.second + "}");

			if(!vector_user_ptr_ || !vector_user_ptr_->get_obj() || !(result = gson(*vector_user_ptr_->get_obj(), &obj.vector_user_ptr_), result.first))
				return std::make_pair(false, "required [group_t.vector_user_ptr_] failed:{" + result.second + "}");

			if(!map_user_ptr_ || !map_user_ptr_->get_obj() || !(result = gson(*map_user_ptr_->get_obj(), &obj.map_user_ptr_), result.first))
				return std::make_pair(false, "required [group_t.map_user_ptr_] failed:{" + result.second + "}");

			if(!user || !user->get_obj() || !(result = gson(*user->get_obj(), &obj.user), result.first))
				return std::make_pair(false, "required [group_t.user] failed:{" + result.second + "}");

			return std::make_pair(true, "");
		}


		std::pair<bool, std::string> gson(acl::json_node &node, group_t *obj)
		{
			return gson(node, *obj);
		}

	}

}
void test01()
{
	group_t g1;
	g1.group_id_ = "10100101";
	user_t u1,u2,u3;
	auth_t au1;
	au1.id = 1;
	au1.password = new std::string("auth");
	u1.auth = new std::list<auth_t*>;
	u1.auth->push_back(new auth_t{ au1});
	u1.c_str = new char[6]{ "hahah" };
	u1.id = 100l;
	u1.username_ = "u1";
	
	u2.auth = new std::list<auth_t*>;
	u2.auth->push_back(new auth_t{ au1 });
	u2.c_str = new char[6]{ "hahah" };
	u2.id = 1002;
	u2.username_ = "u2";

	u3.auth = new std::list<auth_t*>;
	u3.auth->push_back(new auth_t{ au1 });
	u3.c_str = new char[6]{ "hahah" };
	u3.id = 1003;
	u3.username_ = "u3";

	g1.user_list_.push_back(u1);
	g1.user_list_.push_back(u2);
	g1.user_list_.push_back(u3);

	g1.user_ptr_list_.push_back(new user_t{u1});
	g1.user_ptr_list_.push_back(new user_t{ u2 });

	g1.user_ptr_list_ptr_ = new std::list< user_t*>;
	g1.user_ptr_list_ptr_->push_back(new user_t{u1});
	g1.user_ptr_list_ptr_->push_back(new user_t{u2});
	std::list<user_t> users;
	users.push_back(u1);
	users.push_back(u2);
	users.push_back(u3);
	g1.user_list_list_.push_back(users);
	g1.user_list_list_.push_back(users);
	g1.user = u1;

	g1.vector_user_.push_back(u1);
	g1.vector_user_.push_back(u2);
	g1.vector_user_.push_back(u3);

	g1.vector_user_ptr_.push_back(new user_t{ u1 });
	g1.vector_user_ptr_.push_back(new user_t{ u2 });
	g1.vector_user_ptr_.push_back(new user_t{ u2 });

	g1.map_user_ptr_.insert(std::make_pair("1",  &u1 ));
	g1.map_user_ptr_.insert(std::make_pair("2", &u2));
	g1.map_user_ptr_.insert(std::make_pair("3", &u3));

	acl::json json;
	acl::json_node &node = acl::gson::gson(json, g1);

	std::cout<<node.to_string().c_str();
	acl::string str1 = node.to_string();
	acl::json j2;
	j2.update(node.to_string().c_str());
	group_t g2;

	acl::gson::gson(j2.get_root(), g2);

	acl::json j3;
	acl::json_node &node3 = acl::gson::gson(j3, g2);
	acl::string str3 = node3.to_string();

	if(str3 == str1)
	{
		printf("ok!");
	}
	getchar();
}
void test02()
{
	acl::gson::gsoner gr;
	gr.read_file("struct2.h");
	gr.parse_code();
}


void test03()
{
	acl::json json;
	user_t user{ "haha",  int(1) };
	user.auth = new std::list<auth_t*>;
	user.auth->push_back(new auth_t{ 111, new std::string("aaaa") });
	user.c_str = "hello world";
	acl::json_node &node = acl::gson::gson(json, user);



	acl::json j2;
	j2.update(node.to_string().c_str());
	j2.get_root();
	user_t u;
	if(acl::gson::gson(j2.get_root(), u).first)
	{
		acl::json jj;
		printf(acl::gson::gson(jj, u).to_string().c_str());
	}
}
#define test_1() printf("1");
#define test_2(a,b) printf("2");
#define test_3(a,b,c) printf("3");
#define test_4(a,b,c,d) printf("4");

int main(void)
{
	typedef const user_t const_user_t;
	//std::cout << std::is_const<std::remove_cv<const_user_t>::type>::value;
	test01();
	//test03();
	//test02();
	return 0;
	std::string str = "std::string";
	std::string sub = str.substr(str.find("::"));
	printf(sub.c_str());
	getchar();
	test01();
	return 0;
	printf("enter any key to exit\r\n");
	return 0;
}
