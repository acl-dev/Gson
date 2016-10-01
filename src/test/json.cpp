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
	std::list<const user_t*> *user_ptr_list_ptr_;
	std::list<std::list<user_t>> user_list_list_;
	user_t user;
};
namespace acl
{
	namespace gson
	{

		template <class T>
		typename std::enable_if<std::is_integral<T>::value, bool>::type
		gson(acl::json_node &node, T &obj)
		{
			if(node.is_number() == false)
				return false;
			obj = static_cast<T>(*node.get_int64());
			return true;
		}

		template <class T>
		typename std::enable_if<std::is_integral<T>::value, bool>::type
			gson(acl::json_node &node, T *&obj)
		{
			if(node.is_number() == false)
				return false;
			obj = new T;
			*obj = static_cast<T>(*node.get_int64());
			return true;
		}
		bool gson(acl::json_node &node, char* &obj)
		{
			if(node.is_string() == false)
				return false;
			int len = strlen(node.get_string());
			obj = new char[len];
			memcpy(obj, node.get_string(), len);
			obj[len] = 0;
			return true;
		}

		bool gson(acl::json_node &node, std::string &obj)
		{
			if(node.is_string() == false)
				return false;
			obj.append(node.get_string());
			return true;
		}

		bool gson(acl::json_node &node, acl::string &obj)
		{
			if(node.is_string() == false)
				return false;
			obj.append(node.get_string());
			return true;
		}

		bool gson(acl::json_node &node, std::string *&obj)
		{
			if(node.is_string() == false)
				return false;
			obj = new std::string;
			obj->append(node.get_string());
			return true;
		}

		bool gson(acl::json_node &node, acl::string *&obj)
		{
			if(node.is_string() == false)
				return false;
			obj = new acl::string;
			obj->append(node.get_string());
			return true;
		}

		template<class T>
		bool gson(acl::json_node &node, T **obj)
		{
			*obj = new T();
			if(gson(node, *obj) == false)
			{
				delete *obj;
				*obj = NULL;
				return false;
			}
			return true;
		}
		template<class T>
		bool gson(acl::json_node *node, T**obj)
		{
			*obj = new T();
			if(gson(node, *obj) == false)
			{
				delete *obj;
				*obj = NULL;
				return false;
			}
			return true;
		}
		template<class T>
		bool gson(acl::json_node *node, std::list<T> *objs)
		{
			acl::json_node *itr = node->first_child();
			while(itr)
			{
				T obj;
				if(gson(itr, &obj))
					objs->push_back(obj);
				itr = node->next_child();
			}
			return !!!objs->empty();
		}
		template<class T>
		bool gson(acl::json_node &node, T *obj)
		{
			return gson(node, *obj);
		}
		template<class T>
		bool gson(acl::json_node *node, T *obj)
		{
			return gson(*node, *obj);
		}
		template<class T>
		bool gson(acl::json_node *node, T &obj)
		{
			return gson(*node, obj);
		}

		
		bool gson(acl::json_node &node, auth_t &obj)
		{
			acl::json_node *_id = node["id"];
			acl::json_node *_password = node["password"];
			if(_id && gson(*_id,obj.id))
				return false;
			if(_password && gson(*_password, obj.password))
				return false;
			return true;
		}
		
		

		bool gson(acl::json_node &node, user_t &obj)
		{
			acl::json_node *id = node["id"];
			acl::json_node *username_ = node["username_"];
			acl::json_node *c_str = node["c_str"];
			acl::json_node *auth = node["auth"];

			if(c_str)
				gson(*c_str, obj.c_str);

			if(username_ && gson(*username_, obj.username_))
				return false;

			if(auth &&auth->get_obj() && gson(auth->get_obj(), &obj.auth))
				return false;

			return true;
		}

		template<class T>
		acl::json_node &gson(acl::json &json, const T *user)
		{
			return gson(json, *user);
		}

		acl::json_node& gson(acl::json & json,acl::json_node &node,
							 const char* laber,const std::string &obj)
		{
			return node.add_text(laber, obj.c_str());
		}

		acl::json_node& gson(acl::json & json,acl::json_node &node,
							 const char* laber, std::string *obj)
		{
			return node.add_text(laber, obj->c_str());
		}
		acl::json_node& gson(acl::json & json, acl::json_node &node,
							 const char* laber, const std::string *obj)
		{
			return node.add_text(laber, obj->c_str());
		}
		acl::json_node& gson(acl::json & json,acl::json_node &node,
							 const char* laber,const char *obj)
		{
			return node.add_text(laber, obj);
		}
		acl::json_node& gson(acl::json & json,acl::json_node &node,
							 const char* laber, char *obj)
		{
			return node.add_text(laber, obj);
		}
		template <class T>
		typename std::enable_if<std::is_integral<T>::value, acl::json_node&>::type
			gson(acl::json & json, acl::json_node &node, const char* laber, const T *obj)
		{
			return node.add_number(laber, *obj);
		}

		template <class T>
		typename std::enable_if<std::is_integral<T>::value, acl::json_node&>::type
			gson(acl::json & json, acl::json_node &node, const char* laber, const T &obj)
		{
			return node.add_number(laber, obj);
		}

		template <class T>
		typename std::enable_if<std::is_integral<typename std::decay<T>::type>::value == false, acl::json_node&>::type
			gson(acl::json & json, acl::json_node &node, const char* laber, const std::list<T> *obj)
		{
			return node.add_child(laber, acl::gson::gson(json, obj));
		}

		template <class T>
		typename std::enable_if<std::is_integral<typename std::decay<T>::type>::value == false, acl::json_node&>::type
			gson(acl::json & json, acl::json_node &node, const char* laber, const std::list<T> &obj)
		{
			return node.add_child(laber, acl::gson::gson(json, obj));
		}

		template <class T>
		static inline acl::json_node&	gson(acl::json & json,
		acl::json_node &node, const char* laber, T &obj)
		{
			return node.add_child(laber, acl::gson::gson(json, obj));
		}
		acl::json_node &gson(acl::json &json, const auth_t &obj)
		{
			acl::json_node& node = json.create_node();
			gson(json, node, "password", obj.password);
			gson(json, node, "id", obj.id);
			return node;
		}
		
		acl::json_node &gson(acl::json &json, const user_t &user)
		{
			acl::json_node &node = json.create_node();
			gson(json, node, "username_", user.username_);
			gson(json, node, "id", user.id);
			gson(json, node, "c_str", user.c_str);
			gson(json, node, "auth", user.auth);

			return node;
		}
	



		acl::json_node& gson(acl::json & json, const group_t &obj)
		{
			acl::json_node &node = json.create_node();
			gson(json, node, "group_id_", obj.group_id_);
			gson(json, node, "user_list_", obj.user_list_);
			gson(json, node, "user_ptr_list_", obj.user_ptr_list_);
			gson(json, node, "user_ptr_list_ptr_", obj.user_ptr_list_ptr_);
			gson(json, node, "user_list_list_", obj.user_list_list_);
			gson(json, node, "user", obj.user);

			return node;
		}

	}

}
void test1()
{
	group_t group;
	group.group_id_ = "10100101";
	group.user_list_.push_back(user_t{ "hello1",1 });
	group.user_list_.push_back(user_t{ "hello2", 2 });
	group.user_list_.push_back(user_t{ "hello3", 3 });
	group.user_ptr_list_.push_back(new user_t{ "u_ptr1", 11 });
	group.user_ptr_list_.push_back(new user_t{ "u_ptr2", 12 });

	group.user_ptr_list_ptr_ = new std::list<const user_t*>;
	group.user_ptr_list_ptr_->push_back(new user_t{ "u_ptr3", 13 });
	group.user_ptr_list_ptr_->push_back(new user_t{ "u_ptr4", 14 });
	std::list<user_t> users;
	users.push_back(user_t{ "1",1 });
	users.push_back(user_t{ "2", 2 });
	users.push_back(user_t{ "3", 3 });
	group.user_list_list_.push_back(users);
	group.user_list_list_.push_back(users);

	acl::json json;
	acl::json_node &node = acl::gson::gson(json, group);

	printf(node.to_string().c_str());
	getchar();
}
void test02()
{
	acl::gson::gsoner gr;
	gr.read_file("struct.h");
	gr.parse_code();
}
void test01()
{
	acl::gson::gsoner er;
	er.test_pack();
	getchar();

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
	if(acl::gson::gson(j2.get_root(), u))
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
	test03();
	getchar();
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
