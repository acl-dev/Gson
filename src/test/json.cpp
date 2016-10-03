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

#include "struct2.h"
#include "gson_gen.h"

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

	g1.int_map_["1"] = 1;
	g1.int_map_["2"] = 2;
	g1.int_map_["3"] = 3;

	g1.double_map_["1"] = 0.1;
	g1.double_map_["2"] = 0.2;
	g1.double_map_["3"] = 0.3;

	g1.string_map_["1"] = "1";
	g1.string_map_["2"] = "2";
	g1.string_map_["3"] = "3";

	g1.int_map_ptr_["1"] = new int (1);
	g1.int_map_ptr_["2"] = new int(2);
	g1.int_map_ptr_["3"] = new int(3);

	g1.double_map_ptr_["1"] = new double(0.1);
	g1.double_map_ptr_["2"] = new double(0.1);

	g1.string_map_ptr_["1"] = new std::string("1");
	g1.string_map_ptr_["2"] = new std::string("2");
	g1.string_map_ptr_["3"] = new std::string("3");

	acl::json json;
	acl::json_node &node = acl::gson::gson(json, g1);

	std::cout<<node.to_string().c_str();
	acl::string str1 = node.to_string();
	acl::json j2;
	j2.update(node.to_string().c_str());
	group_t g2;

	std::pair<bool,std::string> res = 
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
	//std::cout << std::is_floating_point<std::remove_pointer<const float*>::type>::value;
	//std::cout << acl::gson::is_string_type<const std::string*>::value;
	//std::cout << std::is_class</*typename std::remove_pointer<*/const std::string/*>::type*/>::value;
	std::cout << std::is_object<int>::value;
	getchar();
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
