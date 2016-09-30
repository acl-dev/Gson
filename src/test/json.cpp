// json.cpp : �������̨Ӧ�ó������ڵ㡣
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

static void test(void)
{
	static const char* data = \
		"{ 'menu name': {\r\n"
		"    'id:file': 'file',\r\n"
		"    'value': 'File',\r\n"
		"    'popup': {\r\n"
		"        'menuitem1': [\r\n"
		"            {'value': 'New1', 'onclick': 'CreateNewDoc()'},\r\n"
		"            {'value': 'Open1', 'onclick': 'OpenDoc()'},\r\n"
		"            {'value': 'Close1', 'onclick': 'CloseDoc()'}\r\n"
		"        ],\r\n"
		"        'menuname': 'hello world',\r\n"
		"        'inner': { 'value' : 'new ', 'value' : 'open' },\r\n"
		"        'menuitem2': [\r\n"
		"            {'value': 'New2', 'onclick': 'CreateNewDoc()'},\r\n"
		"            {'value': 'Open2', 'onclick': 'OpenDoc()'},\r\n"
		"            {'value': 'Close2', 'onclick': 'CloseDoc()'},\r\n"
		"            {{'value': 'Help2', 'onclick': 'Help()'}}"
		"        ]\r\n"
		"    }\r\n"
		" }\r\n,"
		" 'help': 'hello world!',\r\n"
		" 'menuitem2': [\r\n"
		"   {'value': 'New3', 'onclick': 'CreateNewDoc()'},\r\n"
		"   {'value': 'Open3', 'onclick': 'OpenDoc()'},\r\n"
		"   {'value': 'Close3', 'onclick': 'CloseDoc()'},\r\n"
		"   [{'value': 'Save3', 'onclick': 'SaveDoc()'}]"
		" ]\r\n"
		"}\r\n";
	acl::json json;

	json.update(data);
	const vector<acl::json_node*>& elements = json.getElementsByTagName("value");

	if (!elements.empty())
	{
		vector<acl::json_node*>::const_iterator cit = elements.begin();
		for (; cit != elements.end(); ++cit)
		{
			acl::json_node *node = *cit;
			printf("tagname: %s, text: %s\n",
				node->tag_name() ? node->tag_name() : "",
				node->get_text() ? node->get_text() : "");
		}
	}

	//const vector<acl::json_node*>& menuitem1 = json.getElementsByTagName("menuitem1");
	const vector<acl::json_node*>& menuitem1 =
		json.getElementsByTags("menu name/popup/menuitem1");
	if (!menuitem1.empty())
	{
		vector<acl::json_node*>::const_iterator cit = menuitem1.begin();
		for (; cit != menuitem1.end(); ++cit)
		{
			acl::json_node* node = (*cit)->first_child();
			printf("tag: %s\r\n", (*cit)->tag_name());
			while (node)
			{
				acl::json_node* nv = node->first_child();
				while (nv)
				{
					printf("tag: %s, value: %s; ", nv->tag_name(),
						nv->get_text());
					nv = node->next_child();
				}
				printf("\r\n");
				node = (*cit)->next_child();
			}
		}
	}

	json.reset();

	//////////////////////////////////////////////////////////////////////////

	acl::json_node& root = json.get_root();
	acl::json_node *node0, *node1, *node2, *node3;

	node0 = &json.create_node();
	root.add_child(node0);

	node1 = &json.create_node("name1", "value1");
	node0->add_child(node1);

	node1 = &json.create_node("name2", "value2");
	node0->add_child(node1);

	node1 = &json.create_node();
	node2 = &json.create_node("name3", "value3");
	node1->add_child(node2);
	node2 = &json.create_node("name4", node1);
	node0->add_child(node2);

	////////////////////////////////////////////////////////////////////////////

	node1 = &json.create_node(true);  // node1 Ϊ������
	node2 = &json.create_node("name5", node1);
	node0->add_child(node2);

	node3 = &json.create_node("name6", "value6");
	node1->add_child(node3);

	node3 = &json.create_node("name7", "value7");
	node1->add_child(node3);

	node3 = &json.create_node();
	node1->add_child(node3);

	node1 = &json.create_node("name8", "value8");
	node2 = &json.create_node("name9", "value9");
	(*node3).add_child(node1).add_child(node2);

	//////////////////////////////////////////////////////////////////////////

	acl::json_node& node_a =
		json.create_node("name12",
			json.create_node()
			.add_child(json.create_node("name12_1_1", "value12_1_1"))
			.add_child(json.create_node("name12_1_2", "value12_1_2"))
			.add_child(json.create_node("name12_1_3", "value12_1_3"))
			.add_child(json.create_node("name12_1_4", "value12_1_4")));
	acl::json_node& node_b =
			json.create_node()
			.add_child(json.create_node("name13_1", "value13_1"))
			.add_child(json.create_node("name13_2", "value13_2"))
			.add_child(json.create_node("name13_3", "value13_3"));
	acl::json_node& node_c =
			json.create_node()
			.add_child(json.create_node("name14_1", "value14_1"));
	acl::json_node& node_d =
			json.create_node()
			.add_child(json.create_node("name15_1", "value15_1"));
	acl::json_node& node_e =
		json.create_node()
		.add_child(json.create_node("name15_2", "value15_2"));

	node0->add_child(node_a)
		.add_child(node_b)
		.add_child(node_c)
		.add_child(node_d)
		.add_child(node_e)
		.add_child(json.create_node("name16", "value16"))
		.add_child(json.create_node("name17", "value17"))
		.add_child(json.create_node("name18", "value18"));

	//////////////////////////////////////////////////////////////////////////
	node0->add_text("name19", "value19")
		.add_text("name20", "value20")
		.add_text("name21", "value21");
	(*node0).add_child("name23",
			json.create_node()
			.add_text("name24", "value24")
			.add_text("name24_1", "value24_1"))
			.add_number("name24_int", 1000)
			.add_bool("name24_bool", true)
		.add_child(true, true)
			.add_text("name25", "value25")
			.add_text("name26", "value26")
			.add_text("name27", "value27")
			.get_parent()
		.add_text("name28", "value28")
		.add_text("name29", "value29")
		.add_text("name30", "value30");
	//////////////////////////////////////////////////////////////////////////

	// ���������б�ǩ���Ľ��

	printf("----------------------------------------------------------\r\n");
	acl::json_node* iter = json.first_node();
	while (iter)
	{
		if (iter->tag_name())
		{
			printf("tag: %s", iter->tag_name());
			if (iter->get_text())
				printf(", value: %s\r\n", iter->get_text());
			else
				printf("\r\n");
		}
		iter = json.next_node();
	}

	printf("------------------root first level child---------\r\n");
	iter = node0->first_child();
	while (iter)
	{
		if (iter->tag_name())
			printf("tag: %s", iter->tag_name());
		if (iter->get_text())
			printf(", text: %s\r\n", iter->get_text());
		else
			printf("\r\n");
		iter = node0->next_child();
	}

	//////////////////////////////////////////////////////////////////////////

	printf("-------------------------------------------------\r\n");

	acl::string buf;
	json.build_json(buf);

	printf("-----------------json------------------------\r\n");
	printf("%s\r\n", buf.c_str());
}



struct user_t 
{
	std::string username_;
	int id;
};
struct group_t 
{
	std::string group_id_;
	std::list<user_t> user_list_;
	std::list<user_t*> user_ptr_list_;
	std::list<user_t*> *user_ptr_list_ptr_;
	std::list<std::list<user_t>> user_list_list_;
	user_t user;
};
namespace acl
{
	namespace gson
	{

	acl::json_node &gson (acl::json &json, const user_t &user)
	{
		return json.create_node ().
			add_text ("username_", acl::gson::get_value (user.username_)).
			add_number ("id", acl::gson::get_value (user.id));
	}
	acl::json_node &gson (acl::json &json, const user_t *user)
	{
		return gson (json, *user);
	}
// 	acl::json_node &gson (acl::json &json, const group_t &group)
// 	{
// 		return
// 			json.get_root ().
// 			add_text ("group_id_", acl::gson::get_value (group.group_id_)).
// 			add_child ("user_list_", acl::gson::gson (json, group.user_list_)).
// 			add_child ("user_ptr_list_", acl::gson::gson (json, group.user_ptr_list_)).
// 			add_child ("user_ptr_list_ptr_", acl::gson::gson (json, group.user_ptr_list_ptr_)).
// 			add_child ("user_list_list_", acl::gson::gson (json, group.user_list_list_)).
// 			add_child ("user", acl::gson::gson (json, group.user));
// 	}
	acl::json_node& gson (acl::json & json, const group_t &obj)
	{
		acl::json_node &node = json.create_node ();
		node.add_text ("group_id_", acl::gson::get_value (obj.group_id_));
		node.add_child ("user_list_", acl::gson::gson (json, obj.user_list_));
		node.add_child ("user_ptr_list_", acl::gson::gson (json, obj.user_ptr_list_));
		node.add_child ("user_ptr_list_ptr_", acl::gson::gson (json, obj.user_ptr_list_ptr_));
		node.add_child ("user_list_list_", acl::gson::gson (json, obj.user_list_list_));
		node.add_child ("user", acl::gson::gson (json, obj.user));

		return node;
	}

}

}
void test1 ()
{
	group_t group;
	group.group_id_ = "10100101";
	group.user_list_.push_back (user_t{"hello1",1});
	group.user_list_.push_back (user_t{"hello2", 2});
	group.user_list_.push_back (user_t{"hello3", 3});
	group.user_ptr_list_.push_back (new user_t{"u_ptr1", 11});
	group.user_ptr_list_.push_back (new user_t{"u_ptr2", 12});

	group.user_ptr_list_ptr_ = new std::list<user_t*>;
	group.user_ptr_list_ptr_->push_back (new user_t{"u_ptr3", 13});
	group.user_ptr_list_ptr_->push_back (new user_t{"u_ptr4", 14});
	std::list<user_t> users;
	users.push_back (user_t{"1",1});
	users.push_back (user_t{"2", 2});
	users.push_back (user_t{"3", 3});
	group.user_list_list_.push_back(users);
	group.user_list_list_.push_back (users);

	acl::json json;
	acl::json_node &node = acl::gson::gson (json, group);

	printf (node.to_string ().c_str ());
	getchar ();
}
void test02 ()
{
	acl::gson::gsoner gr;
	gr.read_file ("D://struct.h");
	gr.parse_code ();
}
void test01()
{
	acl::gson::gsoner er;
	er.test1 ();
	getchar ();

}
int main(void)
{

	test02 ();
	return 0;
	std::string str = "std::string";
	std::string sub = str.substr (str.find ("::"));
	printf (sub.c_str ());
	getchar ();
	test01 ();
	return 0;
	test();
	printf("enter any key to exit\r\n");
	return 0;
}

