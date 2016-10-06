#pragma once
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/stdlib/json.hpp"
#include "acl_cpp/stdlib/string.hpp"
#include "gson.h"
namespace acl
{
	namespace gson
	{
		///////////////////////////////////type_traits//////////////////////////
		// TEMPLATE CLASS _Is_number
		template<class T>
		struct _Is_number
			: std::false_type
		{	
		};
		template<>
		struct _Is_number<unsigned short>
			: std::true_type
		{	
		};

		template<>
		struct _Is_number<signed short>
			: std::true_type
		{	
		};

		template<>
		struct _Is_number<unsigned int>
			: std::true_type
		{	
		};

		template<>
		struct _Is_number<signed int>
			: std::true_type
		{	
		};

		template<>
		struct _Is_number<unsigned long>
			: std::true_type
		{	
		};

		template<>
		struct _Is_number<signed long>
			: std::true_type
		{	
		};

		template<>
		struct _Is_number<long long>
			: std::true_type
		{	
		};

		template<>
		struct _Is_number<unsigned long long>
			: std::true_type
		{	
		};

		template<class T>
		struct is_number :
			_Is_number<
			typename std::remove_cv<
					typename std::remove_pointer<T>::type>::type>
		{
		};

		//string
		template<class T>
		struct _Is_string :std::false_type
		{
		};
		template<>
		struct _Is_string<std::string> :std::true_type
		{
		};
		template<>
		struct _Is_string<acl::string> :std::true_type
		{
		};
		template<class T>
		struct is_string :
			_Is_string<
			typename std::remove_cv<
			typename std::remove_pointer<T>::type>::type>
		{
		};
		//double

		template<class T>
		struct _Is_double :std::false_type
		{
		};
		template<>
		struct _Is_double<float> :std::true_type
		{
		};
		template<>
		struct _Is_double<double> :std::true_type
		{
		};
		template<>
		struct _Is_double<long double> :std::true_type
		{
		};
		template <class T>
		struct is_double :
			_Is_double<
			typename std::remove_pointer<
			typename std::remove_pointer<T>::type>::type
			>
		{

		};
		//char ptr. c string in cpp
		template<class T>
		struct _Is_char_ptr :std::false_type
		{
		};
		template<>
		struct _Is_char_ptr<char*> :std::true_type
		{
		};
		template<class T>
		struct is_char_ptr :_Is_char_ptr<typename std::remove_cv<T>::type>
		{
		};

		template<class T>
		struct _Is_bool :std::false_type
		{
		};
		template<>
		struct _Is_bool<bool> :std::true_type
		{
		};
		template<class T>
		struct is_bool :
			_Is_bool<typename 
			std::remove_cv<typename std::remove_pointer<T>::type>::type>
		{
		};
		template<bool T>
		struct _Is_object
			:std::false_type
		{
		};

		template<>
		struct _Is_object<true> :std::true_type
		{
		};

		template<class T>
		struct is_object :_Is_object<			
			!is_string<T>::value &&
			!std::is_floating_point<
				typename std::remove_pointer<
				typename std::remove_pointer<T>::type>::type>::value &&
			!is_number<T>::value &&
			!is_bool<T>::value &&
			!is_char_ptr<T>::value
		>
		{
		};

		//acl::string ,std::string
		template<class T>
		typename std::enable_if<is_string<T>::value &&
			!std::is_pointer<T>::value, const char *>::type
			static inline get_value(const T &value)
		{
			return value.c_str();
		}
		template<class T>
		typename std::enable_if<is_string<T>::value &&
			std::is_pointer<T>::value, const char *>::type
			static inline get_value(const T &value)
		{
			return value->c_str();
		}
		//char *,const char *
		static inline const char *get_value(const char *value)
		{
			return value;
		}
		//bool 
		static inline bool get_value(const bool value)
		{
			return value;
		}

		static inline bool get_value(const bool* value)
		{
			return *value;
		}

		//number
		template <class T>
		typename std::enable_if<is_number<T>::value && 
			!std::is_pointer<T>::value, T>::type
			static inline  get_value(const T t)
		{
			return t;
		}
		// number pointor -> number
		// eg: int * -> int .
		template <class T>
		typename std::enable_if<is_number<T>::value &&
			std::is_pointer<T>::value,
			typename std::remove_pointer<T>::type>::type
			static inline  get_value(const T &t)
		{
			return *t;
		}
		template <class T>
		typename std::enable_if<std::is_floating_point<T>::value, T>::type
			static inline  get_value(const T &t)
		{
			return t;
		}
		template <class T>
		typename std::enable_if<std::is_floating_point<T>::value , T>::type
			static inline  get_value(const T *t)
		{
			return *t;
		}
		// obj
		template<class T>
		typename std::enable_if<is_object<T>::value , void>::type
			static inline add_item(acl::json &json,
								   acl::json_node &node,
								   const T &obj)
		{
			node.add_child(gson(json, obj));
		}

		template<class T>
		typename std::enable_if<is_object<T>::value , void>::type
			static inline add_item(acl::json &json,acl::json_node &node,
								   const T *obj)
		{
			return add_item(json, node, *obj);
		}
		// number
		template<class T>
		typename std::enable_if<is_number<T>::value, void>::type
			static inline add_item(acl::json &json, acl::json_node &node, T value)
		{
			node.add_array_number(get_value(value));
		}

		template<class T>
		typename std::enable_if<is_number<T>::value, void>::type
			static inline add_item(acl::json &json, acl::json_node &node, T *value)
		{
			node.add_array_number(get_value(value));
		}

		template<class T>
		typename std::enable_if<std::is_floating_point<
			typename std::remove_pointer<T>::type>::value, void>::type
			static inline add_item(acl::json &json, acl::json_node &node, T value)
		{
			node.add_array_double(get_value(value));
		}
		//bool 
		template<class T>
		typename std::enable_if<is_bool<T>::value, void>::type
			static inline add_item(acl::json &json, acl::json_node &node, T value)
		{
			node.add_array_bool(get_value(value));
		}

		template<class V>
		static inline 
			acl::json_node &gson(acl::json &json, const std::list<V> &objects)
		{
			acl::json_node &node = json.create_array();
			for(typename std::list<V>::const_iterator
				itr = objects.begin(); itr != objects.end(); itr++)
			{
				add_item(json, node, *itr);
			}
			return node;
		}
		template<class V>
		static inline 
			acl::json_node &gson(acl::json &json, const std::list<V> *objects)
		{
			acl::json_node &node = json.create_array();
			for(typename std::list<V>::const_iterator
				itr = objects->begin(); itr != objects->end(); itr++)
			{
				add_item(json, node, *itr);
			}
			return node;
		}

		template<class V>
		static inline acl::json_node &gson(acl::json &json,
										   const std::vector<V> &objects)
		{
			acl::json_node &node = json.create_array();
			for(typename std::vector<V>::const_iterator
				itr = objects.begin(); itr != objects.end(); itr++)
			{
				add_item(json, node, *itr);
			}
			return node;
		}
		template<class V>
		static inline acl::json_node &gson(acl::json &json,
										   const std::vector<V> *objects)
		{
			if(!objects)
				// {'a':[]} for empty vector.
				return json.create_array();
			return gson(json, *objects);
		}


		//define number map
		template<class K, class V>
		typename std::enable_if< is_number<V>::value, acl::json_node &>::type
			static inline gson(acl::json &json, const std::map<K, V> &objects)
		{
			acl::json_node &node = json.create_array();
			for(typename std::map<K, V>::const_iterator itr = objects.begin();
				itr != objects.end(); ++itr)
			{
				node.add_child(
					json.create_node().add_number(
					get_value(itr->first),
					get_value(itr->second))
				);
			}
			return node; \
		}
		//define number map
		template<class K, class V>
		typename std::enable_if< is_number<V>::value, acl::json_node &>::type
			static inline gson(acl::json &json, const std::map<K, V> *objects)
		{
			acl::json_node &node = json.create_array();
			for(typename std::map<K, V>::const_iterator itr = objects->begin();
				itr != objects->end(); ++itr)
			{
				node.add_child(
					json.create_node().add_number(
					get_value(itr->first),
					get_value(itr->second))
				);
			}
			return node; \
		}
		//define floating map
		template<class K, class V>
		typename std::enable_if<is_double<V>::value, acl::json_node &>::type
			static inline gson(acl::json &json, const std::map<K, V> &objects)
		{
			acl::json_node &node = json.create_array();
			for(typename std::map<K, V>::const_iterator itr = objects.begin();
				itr != objects.end(); ++itr)
			{
				node.add_child(
					json.create_node().add_double(
					get_value(itr->first),
					get_value(itr->second))
				);
			}
			return node;
		}
		template<class K, class V>
		typename std::enable_if<is_double<V>::value, acl::json_node &>::type
			static inline gson(acl::json &json, const std::map<K, V> *objects)
		{
			acl::json_node &node = json.create_array();
			for(typename std::map<K, V>::const_iterator itr = objects->begin();
				itr != objects->end(); ++itr)
			{
				node.add_child(
					json.create_node().add_double(
					get_value(itr->first),
					get_value(itr->second))
				);
			}
			return node;
		}
		//define bool map
		template<class K, class V>
		typename std::enable_if<is_bool<V>::value, acl::json_node &>::type
			static inline gson(acl::json &json, const std::map<K, V> &objects)
		{
			acl::json_node &node = json.create_array();
			for(typename std::map<K, V>::const_iterator itr = objects;
				itr != objects.end(); ++itr)
			{
				node.add_child(
					json.create_node().add_bool(
					get_value(itr->first),
					get_value(itr->second))
				);
			}
			return node;
		}


		template<class K, class V>
		typename std::enable_if<is_string<V>::value ||
			is_char_ptr<V>::value, acl::json_node &>::type
			static inline gson(acl::json &json, std::map<K, V> objects)
		{
			acl::json_node &node = json.create_array();
			for(typename std::map<K, V>::iterator itr = objects.begin();
				itr != objects.end(); itr++)
			{
				node.add_child(
					json.create_node().add_text(
					get_value(itr->first),
					get_value(itr->second))
				);
			}
			return node;
		}

		template<class T, class V>
		typename std::enable_if<is_object<V>::value, acl::json_node &>::type
		static inline gson(acl::json &json,const std::map<T, V> &objects)
		{

			acl::json_node &node = json.create_array();
			for(typename std::map<T, V>::const_iterator
				itr = objects.begin(); itr != objects.end(); itr++)
			{
				const char *tag = get_value(itr->first);
				acl::json_node &item = gson(json, itr->second);
				node.add_child(json.create_node().add_child(tag, item));
			}
			return node;
		}

		template<class T, class V>
		typename std::enable_if<is_object<V>::value, acl::json_node &>::type
			static inline gson(acl::json &json, const std::map<T, V> *objects)
		{

			acl::json_node &node = json.create_array();
			for(typename std::map<T, V>::const_iterator
				itr = objects->begin(); itr != objects->end(); itr++)
			{
				const char *tag = get_value(itr->first);
				acl::json_node &item = gson(json, itr->second);
				node.add_child(json.create_node().add_child(tag, item));
			}
			return node;
		}

		//////////////////////////////////////////////////////////////////////
		template <class T>
		typename std::enable_if<std::is_class<T>::value,
			std::pair<bool, std::string>>::type
			static inline	gson(acl::json_node &node, T **obj);

		template<class T>
		static inline void del(T **obj)
		{
			delete *obj;
			*obj = NULL;
		}
		template<class T>
		static inline void del(T *obj)
		{
			;
		}
		//bool
		static inline std::pair<bool, std::string>	
			gson(acl::json_node &node, bool *obj)
		{
			if(node.is_bool() == false)
				return std::make_pair(false, "get bool failed");
			*obj = *node.get_bool();
			return std::make_pair(true, "");
		}

		static inline std::pair<bool, std::string>
			gson(acl::json_node &node, bool **obj)
		{
			if(node.is_bool() == false)
				return std::make_pair(false, "get bool failed");
			*obj = new bool;
			**obj = *node.get_bool();
			return std::make_pair(true, "");
		}

		//double
		template <class T>
		typename std::enable_if<std::is_floating_point<T>::value,
			std::pair<bool, std::string>>::type
		static inline	gson(acl::json_node &node, T *obj)
		{
			if(node.is_double() == false)
				return std::make_pair(false, "get double failed");
			*obj = static_cast<T>(*node.get_double());
			return std::make_pair(true, "");
		}

		template <class T>
		typename std::enable_if<std::is_floating_point<T>::value,
			std::pair<bool, std::string>>::type 
			static inline gson(acl::json_node &node, T **obj)
		{
			if(node.is_double() == false)
				return std::make_pair(false, "get double failed");;
			*obj = new T;
			**obj = static_cast<T>(*node.get_double());
			return std::make_pair(true, "");
		}
		//intergral
		template <class T>
		typename std::enable_if<is_number<T>::value,
			std::pair<bool, std::string>>::type
			static inline gson(acl::json_node &node, T *obj)
		{
			if(node.is_number() == false)
				return std::make_pair(false, "get number failed");
			*obj = static_cast<T>(*node.get_int64());
			return std::make_pair(true, "");
		}

		template <class T>
		typename std::enable_if<is_number<T>::value, 
			std::pair<bool, std::string>>::type
			static inline gson(acl::json_node &node, T **obj)
		{
			if(node.is_number() == false)
				return std::make_pair(false, "get number failed");;
			*obj = new T;
			**obj = static_cast<T>(*node.get_int64());
			return std::make_pair(true, "");
		}
		//string
		template<class T>
		typename std::enable_if<std::is_same<T,char>::value, 
			std::pair<bool, std::string>>::type
		static inline gson(acl::json_node &node, T **obj)
		{
			if(node.is_string() == false)
				return std::make_pair(false, "get char * string failed");
			int len = strlen(node.get_string());
			*obj = new T[len + 1];
			memcpy(*obj, node.get_string(), len);
			(*obj)[len] = 0;
			return std::make_pair(true, "");
		}

		template<class T> 
		typename std::enable_if<
			is_string<T>::value && 
			!std::is_pointer<T>::value, std::pair<bool, std::string>>::type
		static inline gson(acl::json_node &node, T *obj)
		{
			if(node.is_string() == false)
				return std::make_pair(false, "get string failed");
			obj->append(node.get_string());
			return std::make_pair(true, "");
		}
		
		template<class T>
		typename std::enable_if<is_string<T>::value,
			std::pair<bool, std::string>>::type
			static inline gson(acl::json_node &node, std::string **obj)
		{
			if(node.is_string() == false)
				return std::make_pair(false, "get string failed");
			*obj = new std::string;
			(*obj)->append(node.get_string());
			return std::make_pair(true, "");
		}

		template<class T>
		static inline std::pair<bool, std::string> 
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

		//vector
		template<class T>
		std::pair<bool, std::string>
			static inline	gson(acl::json_node &node, std::vector<T> *objs)
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

		template <class T>
		typename std::enable_if<std::is_class<T>::value,
			std::pair<bool, std::string>>::type
			static inline	gson(acl::json_node &node, T **obj)
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
		/////////////////////////////////////map///////////////////////////////
		
		//int map
		
		template<class K,class T>
		typename std::enable_if< 
			is_string<T>::value ||
			is_bool<T>::value || 
			is_number<T>::value ||
			is_double<T>::value ||
			is_char_ptr<T>::value, std::pair<bool, std::string>>::type
		static inline 
			expand(acl::json_node &node, std::map<K, T> *objs)
		{
			std::pair<bool, std::string> result;
			acl::json_node *itr = node.first_child();
			while(itr)
			{
				T obj;
				result = gson(*itr, &obj);
				if(result.first)
					objs->insert(
					std::make_pair(K(itr->tag_name()), obj));
				else
					break;
				itr = node.next_child();
			}
			if(result.first == false)
			{
				for(typename std::map<K, T>::iterator it = objs->begin();
					it != objs->end(); ++it)
				{
					del(&it->second);
				}
				objs->clear();
				return result;
			}
			return std::make_pair(true, "");
		}

		template<class K,class T> 
		typename std::enable_if<
			std::is_class<typename std::remove_pointer<T>::type>::value &&
			!is_string<T>::value, std::pair<bool, std::string>>::type
			static inline 
			expand(acl::json_node &node, std::map<K, T> *objs)
		{
			std::pair<bool, std::string> result;
			acl::json_node *itr = node.first_child();
			while(itr && itr->get_obj())
			{
				T obj;
				result = gson(*(itr->get_obj()), &obj);
				if(result.first)
					objs->insert(
					std::make_pair(K(itr->tag_name()), obj));
				else
					break;
				itr = node.next_child();
			}
			if(result.first == false)
			{
				for(typename std::map<K, T>::iterator itr2 = objs->begin();
					itr2 != objs->end(); ++itr2)
				{
					del(&itr2->second);
				}
				objs->clear();
				return result;
			}
			return std::make_pair(true, "");
		}
		//map
		template<class K,class V>
		std::pair<bool, std::string>
			static inline 
			gson(acl::json_node &node, std::map<K, V> *objs)
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

	}
}

//////////////////////////////////////////////////////////////////////////