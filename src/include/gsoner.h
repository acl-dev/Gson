#pragma once
#include <list>
#include <map>
#include <vector>
#include <assert.h>
#include <xutility>

namespace acl
{
	namespace gson
	{

	class gsoner
	{
	public:
		gsoner ()
		{

		}
#define GSON_EXCEPTION(E)\
		struct E##:std::exception\
		{\
			E(){}\
			E(const char *msg):exception(msg){}\
		};

		GSON_EXCEPTION(syntax_error);
		GSON_EXCEPTION(unsupported_type);
		struct field_t 
		{
			enum type_t
			{
				e_bool,
				e_bool_ptr,
				e_number,
				e_double,
				e_cstr,//char *
				e_ccstr,//const char *
				e_string,
				e_list,
				e_vector,
				e_map,
				e_object,
			};
			type_t type_;
			std::string name_;
			field_t()
			{

			}
			field_t(type_t t, const std::string &name)
				:type_(t),
				name_(name)
			{

			}
			virtual ~field_t()
			{

			}
		};
		struct list_t :field_t
		{
			field_t inner_type_;
		};
		struct object_t
		{
			typedef std::list<field_t> fields_t;
			fields_t fields_;
			std::string name_;
			void reset ()
			{
				fields_.clear ();
				name_.clear ();
			}
		};

		struct function_code_t
		{
			std::string declare_;
			std::string declare_ptr_;
			std::string declare2_;
			std::string definition_;
			std::string definition_ptr_;
			std::string definition2_;
		};
		std::string get_node_func (const field_t &field)
		{
			acl::string code;
			switch (field.type_)
			{
				case gsoner::field_t::e_bool:
					return "add_bool";
				case gsoner::field_t::e_number:
					return "add_number";
				case gsoner::field_t::e_double:
					return "add_double";
				case gsoner::field_t::e_string:
					return "add_text";
				case gsoner::field_t::e_list:
				case gsoner::field_t::e_vector:
				case gsoner::field_t::e_map:
				case gsoner::field_t::e_object:
					return "add_child";

				default:
					break;
			}
			return "error_type";
		}
		std::string get_gson_func_laber (const field_t &field)
		{
			acl::string code;
			switch (field.type_)
			{
				case gsoner::field_t::e_bool:
				case gsoner::field_t::e_number:
				case gsoner::field_t::e_double:
				case gsoner::field_t::e_string:
					return "acl::gson::get_value(";
				case gsoner::field_t::e_list:
				case gsoner::field_t::e_vector:
				case gsoner::field_t::e_map:
				case gsoner::field_t::e_object:
					return "acl::gson::gson(json,";
				default:
					break;
			}
			return "error_type";
		}

		function_code_t generate_function_code (const object_t &obj)
		{

			function_code_t code;
			std::string str;
			str += "acl::json_node& gson (acl::json & json, const ";
			str += obj.name_;
			
			code.declare_ptr_ = str;
			code.declare_ptr_ += " *obj);\r\n";
			code.declare2_ = "acl::string gson(const ";
			code.declare2_ += obj.name_;
			code.declare2_ += " &obj);\r\n";
			code.definition2_ = code.declare2_.substr (0, code.declare2_.find (";"));
			code.definition2_ += "\n{\n"
				"    acl::json json;\n"
				"    acl::json_node &node = acl::gson::gson (json, obj);\n"
				"    return node.to_string ();\n}\r\n";

			code.definition_ptr_ = str;
			code.definition_ptr_ += "*obj)\n"
				"{\n"
				"    return gson (json, *obj);\n"
				"}\r\n";
			str += " &obj)";
			
			code.declare_ = str;
			code.declare_ += ";\n";

			str += "\n{\n";
			str += space_;
			str += "acl::json_node &node =  json.create_node();\n";
			
			for (object_t::fields_t::const_iterator itr = obj.fields_.begin ();
				itr != obj.fields_.end(); ++itr)
			{
				str += space_;
				str += "node.";
				str += get_node_func (*itr);
				str += "(\"";
				str += itr->name_;
				str += "\", ";
				str += get_gson_func_laber (*itr);
				str += "obj.";
				str += itr->name_;
				str += "));\n";
			}
			str += "\n";
			str += space_;
			str += "return node;\n}\n\n";

			code.definition_ = str;
			return code;
		}

		std::string test1 ()
		{
			object_t obj;
			obj.name_ = "user_t";
			obj.fields_.push_back (field_t{field_t::e_bool,"is_stop"});

			function_code_t code = generate_function_code (obj);

			printf (code.declare_.c_str ());
			printf (code.definition_.c_str ());
			return code.declare_;
		}

		enum code_parser_status_t
		{
			e_uninit,
			e_comment,
			e_struct_begin,
			e_struct_name,
			e_struct_menber_type,
			e_struct_end,
		};
		/*
			struct user_t
			{
			
			}
		*/
		bool check_struct_begin ()
		{
			if (status_ != e_uninit)
				return false;

			//struct user_t
			if (codes_[pos_] == 's' &&
				codes_[pos_+1] == 't' &&
				codes_[pos_+2] == 'r' &&
				codes_[pos_+3] == 'u' &&
				codes_[pos_+4] == 'c' &&
				codes_[pos_+5] == 't' )
			{
				pos_ += 6;
				std::string name;
				skip_space ();
				while (codes_[pos_] != ' ' &&
					   codes_[pos_] != '{' &&
					   codes_[pos_] != '\r'  &&
					   codes_[pos_] != '\n' && 
					   codes_[pos_] != '\t'&&
					   codes_[pos_] != '/') //
				{
					name.push_back (codes_[pos_]);
					pos_++;
				}
			check_again:
				skip_space ();
				if(codes_[pos_] == '/')
				{
					if(check_comment() == false)
						throw syntax_error();
					goto check_again;
				}
				if (codes_[pos_] == '{')
				{
					pos_++;
					status_ = e_struct_menber_type;
					current_obj_.name_ = name;
					return true;
				}
				if (codes_[pos_] == ';')
				{
					pos_++;
					//struct user_t ; end of struct;
					status_ = e_uninit;
					return true;
				}
			}
			//not struct block
			return false;
		}
		bool check_struct_end ()
		{
			if (status_ =  e_struct_menber_type)
			{
				pos_++;
			again:
				skip_space ();
				if (codes_[pos_] == '/')
				{
					if(check_comment() == false)
						throw syntax_error();
					goto again;
				}
				if (codes_[pos_] == ';')
				{
					pos_++;
					if (current_obj_.name_.size())
					{
						objs_.push_back (current_obj_);
						current_obj_.reset ();
					}
					status_ = e_uninit;
				}
				return true;
			}
			return false;
		}
		bool check_comment ()
		{
			if (codes_[pos_] == '/' &&
				codes_[pos_ + 1] == '/')
			{
				pos_++;
				pos_++;
				//skip a line
				while (codes_[pos_] != '\n')
					pos_++;
				return true;
			}
			else if (codes_[pos_] == '/' &&
					 codes_[pos_ + 1] == '*')
			{
				//skip /**/comment
				pos_++;
				pos_++;
				while (codes_[pos_] != '*' || 
					   codes_[pos_+1] !='/')
					pos_++;
				pos_++;
				pos_++;
				return true;
			}
			return false;
		}
		void skip_space ()
		{
			while (codes_[pos_] == ' '||
				   codes_[pos_] == '\r' ||
				   codes_[pos_] == '\n' ||
				   codes_[pos_] == '\t')
				pos_++;
		}

		bool check_member()
		{
			//struct user_t{int id;  
			
			if (status_ == e_struct_menber_type)
			{
				std::string lines;
				skip_space ();
				while (true)
				{
					if(codes_[pos_] == '/')
					{
						if(check_comment() == false)
							throw syntax_error();
						continue;
					}
					if(codes_[pos_] == ';')
						break;
					lines.push_back (codes_[pos_]);
					pos_++;
				}
				//skip ;
				pos_++;
				std::string name;
				std::string types;
				//remove back spacce. lilke "int a    ; "
				int e = lines.size () - 1;
				while (lines[e] == ' ' ||
					   lines[e] == '\r' ||
					   lines[e] == '\n' ||
					   lines[e] == '\t')
					e--;

				while (lines[e] != ' ' &&
					   lines[e] != '\r' &&
					   lines[e] != '\n' &&
					   lines[e] != '\t' &&
					   lines[e] != '*' &&
					   lines[e] != '&')
				{
					name.push_back (lines[e]);
					e--;
				}
				//get name
				std::reverse (name.begin (), name.end ());

				types = lines.substr (0,e);
				std::list<std::string> tokens;
				std::string token;
				for (std::string::iterator itr = types.begin ();
					itr!= types.end();++itr)
				{
					if (*itr == ' '||
						*itr == '\r'||
						*itr == '\n' ||
						*itr == '\t' )
					{
						if (token.size ())
						{
							tokens.push_back (token);
							token.clear ();
						}
					}
					else
					{
						token.push_back (*itr);
					}
				}
				if (token.size ())
					tokens.push_back (token);
				if (tokens.size() == 0)
				{
					printf ("\"%s\"[syntax error]", name.c_str ());
					assert (false);
				}

				//std    :: list <int> a;
				std::string first = tokens.front();
				if(first == "const")
				{
					tokens.pop_front();
					first = tokens.front();
					std::size_t pos = first.find("char");
					if (pos != std::string::npos )
					{
						if (first.find_first_of('*', pos) != std::string::npos)
						{
							field_t f;
							f.name_ = name;
							f.type_ = field_t::e_ccstr;
							current_obj_.fields_.push_back(f);
							return true;
						}
						tokens.pop_front();
						if (tokens.size())
						{
							first = tokens.front();
							if (first.find('*') != std::string::npos)
							{
								field_t f;
								f.name_ = name;
								f.type_ = field_t::e_ccstr;
								current_obj_.fields_.push_back(f);
								return true;
							}
						}
					}
					throw unsupported_type(("unsupported type:'"+ types +"'")
										   .c_str());
				}
				if (first.find("char") != std::string::npos)
				{
					if(first == "char*")
					{
						field_t f;
						f.name_ = name;
						f.type_ = field_t::e_cstr;
						current_obj_.fields_.push_back(f);
						return true;
					}
					tokens.pop_front();
					if (tokens.size())
					{
						first = tokens.front();
						if (first == "*")
						{
							field_t f;
							f.name_ = name;
							f.type_ = field_t::e_cstr;
							current_obj_.fields_.push_back(f);
							return true;
						}
					}
					throw unsupported_type("unsupported 'char' type");
				}
				if (first.find("std") != std::string::npos)
				{
					for (std::list<std::string>::iterator itr = tokens.begin ();
						itr != tokens.end();++itr)
					{
						if (itr->find ("string") != std::string::npos)
						{
							field_t f;
							f.name_ = name;
							f.type_ = field_t::e_string;
							current_obj_.fields_.push_back (f);
							return true;
						}
						else if (itr->find ("list") != std::string::npos )
						{
							field_t f;
							f.name_ = name;
							f.type_ = field_t::e_list;
							current_obj_.fields_.push_back (f);
							return true;;
						}
						else if(itr->find("vector") != std::string::npos )
						{
							field_t f;
							f.name_ = name;
							f.type_ = field_t::e_vector;
							current_obj_.fields_.push_back(f);
							return true;;
						}
						else if(itr->find("map") != std::string::npos)
						{
							field_t f;
							f.name_ = name;
							f.type_ = field_t::e_map;
							current_obj_.fields_.push_back(f);
							return true;;
						}
					}
				}
				else if (first.find("acl")!= std::string::npos)
				{
					for(std::list<std::string>::iterator itr = tokens.begin();
						itr != tokens.end(); ++itr)
					{
						if(itr->find("string") != std::string::npos)
						{
							field_t f;
							f.name_ = name;
							f.type_ = field_t::e_string;
							current_obj_.fields_.push_back(f);
							return true;
						}
					}
					throw syntax_error();
				}
				else if (first == "singned" ||
						  first == "int" ||
						  first == "int" ||
						  first == "uint32_t" ||
						  first == "int32_t" ||
						  first == "int64_t" ||
						  first == "int64_t")
				{
					field_t f;
					f.type_ = field_t::e_number;
					f.name_ = name;
					current_obj_.fields_.push_back (f);
					return true;
				}
				else if (first == "bool")
				{
					field_t f;
					f.type_ = field_t::e_bool;
					f.name_ = name;
					current_obj_.fields_.push_back (f);
					return true;

				}
				else if (first == "float" ||
						 first == "double")
				{
					field_t f;
					f.type_ = field_t::e_double;
					f.name_ = name;
					current_obj_.fields_.push_back (f);
					return true;
				}
				else
				{
					// user define class ,struct.
					field_t f;
					f.name_ = name;
					f.type_ = field_t::e_object;
					current_obj_.fields_.push_back (f);
					return true;
				}
				return true;

			}
			return false;
		}
		void read_file (const char *file_path)
		{
			std::ifstream is (file_path, std::ifstream::binary);
			std::string str ((std::istreambuf_iterator<char> (is)),
							 std::istreambuf_iterator<char> ());
			codes_ = str;
			printf (codes_.c_str ());
		}
		void parse_code()
		{
			char c = '\n';
			max_pos_ = codes_.size ();
			try
			{
				do
				{
					skip_space();
					if(pos_ == max_pos_)
						break;
					char ch = codes_[pos_];
					switch(ch)
					{
					case '/':
						if(check_comment())
							continue;
					case '}':
						if(check_struct_end())
							continue;
					default:
					{
						if(check_struct_begin())
							continue;
						if(check_member())
							continue;
					}
					}

				} while(pos_ < max_pos_);
			}
			catch(syntax_error &e)
			{
				printf(e.what());
				return;
			}
			catch (std::exception & e)
			{
				printf(e.what());
				return;
			}
			

			std::ofstream header("gson_gen.h");
			std::ofstream define("gson_gen.cpp");
			const char *namespace_start = "namespace acl{\r\nnamespace gson{\r\n";
			const char *namespace_end = "\r\n}///end of acl.\r\n}///end of gson.";
			const char *include = "#include \"stdafx.h\"\n"
								  "#include \"gson_helper.ipp\"\r\n";
			header.write (namespace_start,strlen(namespace_start));

			define.write (include,strlen(include));
			define.write (namespace_start, strlen (namespace_start));
			for (std::list<object_t>::iterator itr = objs_.begin ();
				itr!= objs_.end();++itr)
			{
				function_code_t code =  generate_function_code (*itr);
				header.write (code.declare_.c_str (), code.declare_.size ());
				header.write (code.declare_ptr_.c_str (), code.declare_ptr_.size ());
				header.write (code.declare2_.c_str (), code.declare2_.size ());
				define.write (code.definition_.c_str (), code.definition_.size ());
				define.write (code.definition_ptr_.c_str (), code.definition_ptr_.size ());
				define.write (code.definition2_.c_str (), code.definition2_.size ());
				printf ("%s\n%s",code.declare_.c_str(),code.definition_.c_str());
			}
			header.write (namespace_end, strlen (namespace_end));
			define.write (namespace_end, strlen (namespace_end));

			header.flush ();
			define.flush ();
			header.close ();
			define.close ();
		}
		char cc;
		int pos_ = 0;
		int max_pos_;
		std::string comment_begin_;
		std::string comment_end_;
		std::string codes_;
		code_parser_status_t status_ = e_uninit;
		std::string space_ = "    ";

		object_t current_obj_;
		std::list<object_t> objs_;
	};

}//end of gson
}//end of acl
