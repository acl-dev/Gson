#pragma once
#include <list>
#include <map>
#include <vector>
#include <assert.h>

namespace acl
{
	class jsoner
	{
	public:
		jsoner ()
		{

		}
		struct object_t
		{
			typedef std::map<std::string, object_t*> objects_t;
			enum type_t
			{
				e_bool,
				e_bool_ptr,
				e_number,
				e_number_ptr,
				e_double,
				e_double_ptr,

				e_std_string,
				e_std_string_ptr,
				e_acl_string,
				e_acl_string_ptr,

				e_char,
				e_char_string,

				e_std_list,
				e_std_list_ptr,

				e_std_map,
				e_std_map_ptr,
				
				e_std_vector,
				e_std_vector_ptr,
				
				e_std_pair,
				e_std_pair_ptr,
				
				e_struct,
				e_struct_ptr,
			};
			type_t type_;
			objects_t fields_;
			std::string struct_name_;

			void to_do ()
			{
				//funtion declare
			}
		};

		struct function_code_t
		{
			std::string declare;
			std::string definition;
		};

		/*
			return node.add_text({laber},obj.{laber}.c_str());
		*/
		std::string add_text (const std::string laber, 
							  const std::string prefix, 
							  const object_t *obj)
		{
			std::string codes;
			codes += "node.add_text(\"";
			codes += laber;
			codes +="\",";
			codes += prefix;
			codes += laber;

			if (obj->type_ == object_t::e_acl_string ||
				obj->type_ == object_t::e_std_string)
			{
				codes += ".c_str());\n";
			}
			else if (obj->type_ == object_t::e_acl_string_ptr ||
					 obj->type_ == object_t::e_std_string_ptr)
			{
				codes += "->c_str());\n";
			}
			else if (obj->type_ == object_t::e_char_string)
			{
				codes += ");\n";
			}
			else
			{
				assert (!"add_text error, obj.type is not string type");
			}
			return codes;
		}

		/*
			node.add_text({laber}, obj.{laber}));
			node.add_text({laber}, *(obj.{laber})));
		*/

		std::string add_number (const std::string &laber,
								const std::string &prefix,
								const object_t *obj)
		{
			
			std::string codes;
			codes += "node.add_number(\"";
			codes += laber;
			codes += "\",";

			if (obj->type_ == object_t::e_number)
			{
				
			}
			else if (obj->type_ == object_t::e_number_ptr)
			{
				codes += "*(";
			}
			else
			{
				assert (!"add_number error, obj.type is number type");
			}
			
			codes += prefix;
			codes += laber;
			codes += "));\n";

			return codes;
		}
	
		std::string add_bool(const std::string &laber,
							 const std::string &prefix,
							 const object_t *obj)
		{

			std::string codes;
			codes += "node.add_bool(\"";
			codes += laber;
			codes += "\",";

			if (obj->type_ == object_t::e_number)
			{

			}
			else if (obj->type_ == object_t::e_number_ptr)
			{
				codes += "*(";
			}
			else
			{
				assert (!"add_bool error, obj.type is bool type");
			}

			codes += prefix;
			codes += laber;
			codes += "));\n";

			return codes;
		}

		std::string add_double (const std::string & laber,
								const std::string & prefix, 
								const object_t * obj)
		{
			std::string codes;
			codes += "node.add_double(\"";
			codes += laber;
			codes += "\",";

			if (obj->type_ == object_t::e_number)
			{

			}
			else if (obj->type_ == object_t::e_number_ptr)
			{
				codes += "*(";
			}
			else
			{
				assert (!"add_double error, obj.type is double type");
			}

			codes += prefix;
			codes += laber;
			codes += "));\n";

			return codes;
		}
		/*
			add_child ("{laber}", to_json (json, group.bools_));
		*/
		std::string add_objject(const std::string & laber,
							   const std::string & prefix,
							   const object_t * obj)
		{
			std::string codes;
			codes += "node.add_child(\"";
			codes += laber;
			codes += "\", to_json(json, ";
			if (obj->type_ == object_t::e_std_list ||
				obj->type_ == object_t::e_std_vector)
			{
				codes += prefix;
				codes += laber;
			}
			else if (obj->type_ == object_t::e_std_list_ptr ||
				obj->type_ == object_t::e_std_vector_ptr)
			{
				codes += "*(";
				codes += prefix;
				codes += laber;
				codes += ")";
			}
			else
			{
				assert (!"add_array error, obj.type is array type");
			}
			codes += "));\n";

			return codes;
		}

		std::string add_code (const std::string &laber,
							  const std::string &prefix, 
							  object_t *obj)
		{
			switch (obj->type_)
			{
				case acl::jsoner::object_t::e_bool:
				case acl::jsoner::object_t::e_bool_ptr:
					return add_bool (laber, prefix, obj);
				case acl::jsoner::object_t::e_number:
				case acl::jsoner::object_t::e_number_ptr:
					return add_number (laber, prefix, obj);
				case acl::jsoner::object_t::e_double:
				case acl::jsoner::object_t::e_double_ptr:
					return add_double (laber, prefix, obj);
				case acl::jsoner::object_t::e_std_string:
				case acl::jsoner::object_t::e_acl_string:
				case acl::jsoner::object_t::e_std_string_ptr:
				case acl::jsoner::object_t::e_acl_string_ptr:
				case acl::jsoner::object_t::e_char_string:
					return add_text (laber, prefix, obj);

				case acl::jsoner::object_t::e_std_list:
				case acl::jsoner::object_t::e_std_list_ptr:
				case acl::jsoner::object_t::e_std_vector:
				case acl::jsoner::object_t::e_std_vector_ptr:
				case acl::jsoner::object_t::e_std_map:
				case acl::jsoner::object_t::e_std_map_ptr:
				case acl::jsoner::object_t::e_std_pair:
				case acl::jsoner::object_t::e_std_pair_ptr:
				case acl::jsoner::object_t::e_struct:
				case acl::jsoner::object_t::e_struct_ptr:
					return add_objject (laber, prefix, obj);
				case acl::jsoner::object_t::e_char:
				default:
					break;
			}
		}

			/*

		struct user_t
		{
			std::string name;
		}

		// generate_code:

		acl::json_node &to_json(acl::json & json, const user_t &obj)
		{
			acl::json_node &node =  json.create_node();
			node.add_text("name", obj.name.c_str());

			return node;
		}

		*/
		function_code_t generate_function_code (const object_t &obj)
		{

			function_code_t code;
			std::string str;
			str += "acl::json_node &to_json(acl::json & json, const ";
			str += obj.struct_name_;
			str += " &obj)";
			
			code.declare = str;
			code.declare += ";\n";

			str += "\n{\n";
			str += space;
			str += "acl::json_node &node =  json.create_node();\n";
			
			for (object_t::objects_t::const_iterator itr = obj.fields_.begin ();
				itr != obj.fields_.end(); ++itr)
			{
				str += space;
				str += add_code (itr->first,  "obj.", itr->second);
			}
			str += "\n";
			str += space;
			str += "return node;\n}\n\n";

			code.definition = str;
			return code;
		}

		std::string test1 ()
		{
			object_t *name = new object_t;
			name->type_ = object_t::e_std_list;

			object_t obj;
			obj.type_ = object_t::e_struct;
			obj.struct_name_ = "user_t";
			obj.fields_.insert (std::make_pair ("name",name));

			function_code_t code = generate_function_code (obj);

			printf (code.declare.c_str ());
			printf (code.definition.c_str ());

			return code.declare;
		}
		void generate_code (object_t obj)
		{
			std::string symbol = " ";
			switch (obj.type_)
			{
				case object_t::e_struct:
					{
						symbol = " *";
					}

			}
		}


		std::string space = "    ";
	};


	struct  user_t
	{
		user_t (const char *username_, int age_)
		{
			username = username_;
			age = age_;
		}
		std::string username;
		int age;
	};

	struct group_t
	{
		std::string group_id;
		std::vector<user_t> users;
		std::map<std::string, std::string> status;
		std::list<bool> bools_;
	};

	acl::json_node &to_json (acl::json &json, user_t user)
	{
		return json.create_node ().add_text ("username", user.username.c_str ()).
			add_number ("age", user.age);
	}
	template<class T>
	acl::json_node &to_json (acl::json &json, std::vector<T> objects)
	{

		acl::json_node &node = json.create_array ();
		for (auto itr : objects)
		{
			node.add_child (to_json (json, itr));
		}
		return node;
	}
	//template<class T>
	acl::json_node &to_json (acl::json &json, std::list<bool> objects)
	{
		acl::json_node &node = json.create_array ();
		for (auto itr : objects)
		{
			node.add_array_bool (itr);
		}
		return node;
	}

	acl::json_node &to_json (acl::json &json, std::pair<const std::string, std::string> &objects)
	{
		return json.create_node ().add_text (objects.first.c_str (), objects.second.c_str ());
	}
	acl::json_node &to_json (acl::json &json, std::map<std::string, std::string> objects)
	{

		acl::json_node &node = json.create_array ();
		for (auto itr : objects)
		{
			node.add_child (to_json (json, itr));
		}
		return node;
	}

	acl::json_node &to_json (acl::json &json, group_t group)
	{
		return json.create_node ()
			.add_text ("group_id", group.group_id.c_str ()).
			add_child ("users", to_json (json, group.users)).
			add_child ("status", to_json (json, group.status)).
			add_child ("bools_", to_json (json, group.bools_));
	}

	void test4 ()
	{
		acl::json json;
		group_t group;
		group.group_id = "hello world";
		group.users.emplace_back ("user1", 11);
		group.users.emplace_back ("user2", 12);
		group.users.emplace_back ("user3", 13);

		group.status.insert (std::make_pair ("user1", "ok"));
		group.status.insert (std::make_pair ("user2", "ok"));
		group.status.insert (std::make_pair ("user2", "error"));

		group.bools_.push_back (true);
		group.bools_.push_back (false);
		group.bools_.push_back (true);
		group.bools_.push_back (false);

		acl::json_node &node = to_json (json, group);

		printf (node.to_string ().c_str ());
		printf ("\n-----------------------------------------------------------------\n");

	}
}
