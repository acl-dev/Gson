#ifndef __GSON_HELPER__
#define __GSON_HELPER__

namespace acl
{
namespace gson
{
#define __GSON_DEFINE_ADD_ITEM__(TYPE,FUNC,P)\
static inline void add_item(acl::json &json,\
						acl::json_node &node, \
						TYPE value)\
{\
node.##FUNC( P value);\
}

static inline const char* get_value(const std::string &value)
{
	return value.c_str();
}
static inline const char* get_value(const acl::string &value)
{
	return value.c_str();
}
static inline const char* get_value(const std::string *value)
{
	return value->c_str();
}
static inline const char* get_value(const acl ::string *value)
{
	return value->c_str();
}
#define __GSON_DEFINE_GET_VALUE__(TYPE,PTR)\
static inline const TYPE get_value(const TYPE PTR value)\
{\
return  PTR##value;\
}

	//define_string_map
#define __GSON_DEFINE_STRING_MAP__(TYPE)\
template<class K>\
static inline acl::json_node &gson(acl::json &json,\
									std::map<K, TYPE> objects)\
{\
acl::json_node &node = json.create_array();\
for(auto &itr : objects)\
{\
	node.add_child(\
		json.create_node().add_text( \
		get_value(itr.first),\
		get_value(itr.second))\
	);\
}\
return node;\
}

//define number map
#define __GSON_DEFINE_NUMBER_MAP__(TYPE)\
template<class K>\
static inline acl::json_node &gson(acl::json &json,\
									std::map<K, TYPE> objects)\
{\
acl::json_node &node = json.create_array();\
for(auto &itr : objects)\
{\
	node.add_child(\
		json.create_node().add_number(\
		get_value(itr.first),\
		get_value(itr.second))\
	);\
}\
return node;\
}

//define bool map
#define __GSON_DEFINE_BOOL_MAP__(TYPE)\
template<class K>\
static inline acl::json_node &gson(acl::json &json,\
									std::map<K, TYPE> objects)\
{\
acl::json_node &node = json.create_array();\
for(auto &itr : objects)\
{\
	node.add_child(\
		json.create_node().add_bool(\
		get_value(itr.first),\
		get_value(itr.second))\
	);\
}\
return node;\
}

//define bool map
#define __GSON_DEFINE_DOUBLE_MAP__(TYPE)\
template<class K>\
static inline acl::json_node &gson(acl::json &json,\
									std::map<K, TYPE> objects)\
{\
acl::json_node &node = json.create_array();\
for(auto &itr : objects)\
{\
	node.add_child(\
		json.create_node().add_double(\
		get_value(itr.first),\
		get_value(itr.second))\
	);\
}\
return node;\
}


	template<class T>
	inline void add_item (acl::json &json, acl::json_node &node, const T &user)
	{
		node.add_child (gson (json, user));
	}
	template<class T>
	inline void add_item (acl::json &json, acl::json_node &node, const T *user)
	{
		return add_item (json, node, *user);
	}

	__GSON_DEFINE_ADD_ITEM__ (bool, add_array_bool, );
	__GSON_DEFINE_ADD_ITEM__ (int8_t, add_array_number, );
	__GSON_DEFINE_ADD_ITEM__ (uint8_t, add_array_number, );
	__GSON_DEFINE_ADD_ITEM__ (int16_t, add_array_number, );
	__GSON_DEFINE_ADD_ITEM__ (uint16_t, add_array_number, );
	__GSON_DEFINE_ADD_ITEM__ (int32_t, add_array_number, );
	__GSON_DEFINE_ADD_ITEM__ (uint32_t, add_array_number, );
	__GSON_DEFINE_ADD_ITEM__ (int64_t, add_array_number, );
	__GSON_DEFINE_ADD_ITEM__ (uint64_t, add_array_number, );
	__GSON_DEFINE_ADD_ITEM__ (float, add_array_double, );
	__GSON_DEFINE_ADD_ITEM__ (double, add_array_double, );

	__GSON_DEFINE_ADD_ITEM__ (bool*, add_array_bool, *);
	__GSON_DEFINE_ADD_ITEM__ (int8_t*, add_array_number, *);
	__GSON_DEFINE_ADD_ITEM__ (uint8_t*, add_array_number, *);
	__GSON_DEFINE_ADD_ITEM__ (int16_t*, add_array_number, *);
	__GSON_DEFINE_ADD_ITEM__ (uint16_t*, add_array_number, *);
	__GSON_DEFINE_ADD_ITEM__ (int32_t*, add_array_number, *);
	__GSON_DEFINE_ADD_ITEM__ (uint32_t*, add_array_number, *);
	__GSON_DEFINE_ADD_ITEM__ (int64_t*, add_array_number, *);
	__GSON_DEFINE_ADD_ITEM__ (uint64_t*, add_array_number, *);


	__GSON_DEFINE_ADD_ITEM__ (float*, add_array_double, *);
	__GSON_DEFINE_ADD_ITEM__ (double*, add_array_double, *);



	__GSON_DEFINE_GET_VALUE__ (bool, );
	__GSON_DEFINE_GET_VALUE__ (int8_t, );
	__GSON_DEFINE_GET_VALUE__ (uint8_t, );
	__GSON_DEFINE_GET_VALUE__ (int16_t, );
	__GSON_DEFINE_GET_VALUE__ (uint16_t, );
	__GSON_DEFINE_GET_VALUE__ (int32_t, );
	__GSON_DEFINE_GET_VALUE__ (uint32_t, );
	__GSON_DEFINE_GET_VALUE__ (int64_t, );
	__GSON_DEFINE_GET_VALUE__ (uint64_t, );
	__GSON_DEFINE_GET_VALUE__ (float, );
	__GSON_DEFINE_GET_VALUE__ (double, );

	__GSON_DEFINE_GET_VALUE__ (bool*, );
	__GSON_DEFINE_GET_VALUE__ (int8_t, *);
	__GSON_DEFINE_GET_VALUE__ (uint8_t, *);
	__GSON_DEFINE_GET_VALUE__ (int16_t, *);
	__GSON_DEFINE_GET_VALUE__ (uint16_t, *);
	__GSON_DEFINE_GET_VALUE__ (int32_t, *);
	__GSON_DEFINE_GET_VALUE__ (uint32_t, *);
	__GSON_DEFINE_GET_VALUE__ (int64_t, *);
	__GSON_DEFINE_GET_VALUE__ (uint64_t, *);
	__GSON_DEFINE_GET_VALUE__ (float*, );
	__GSON_DEFINE_GET_VALUE__ (double*, );


	__GSON_DEFINE_STRING_MAP__ (std::string);
	__GSON_DEFINE_STRING_MAP__ (acl::string);
	__GSON_DEFINE_STRING_MAP__ (char*);
	__GSON_DEFINE_STRING_MAP__ (const char*);
	__GSON_DEFINE_STRING_MAP__ (std::string*);
	__GSON_DEFINE_STRING_MAP__ (acl::string*);


	__GSON_DEFINE_BOOL_MAP__ (bool);
	__GSON_DEFINE_BOOL_MAP__ (bool*);
	__GSON_DEFINE_NUMBER_MAP__ (int8_t);
	__GSON_DEFINE_NUMBER_MAP__ (uint8_t);
	__GSON_DEFINE_NUMBER_MAP__ (int16_t);
	__GSON_DEFINE_NUMBER_MAP__ (uint16_t);
	__GSON_DEFINE_NUMBER_MAP__ (int32_t);
	__GSON_DEFINE_NUMBER_MAP__ (uint32_t);
	__GSON_DEFINE_NUMBER_MAP__ (int64_t);
	__GSON_DEFINE_NUMBER_MAP__ (uint64_t);
	__GSON_DEFINE_NUMBER_MAP__ (int8_t*);
	__GSON_DEFINE_NUMBER_MAP__ (uint8_t*);
	__GSON_DEFINE_NUMBER_MAP__ (int16_t*);
	__GSON_DEFINE_NUMBER_MAP__ (uint16_t*);
	__GSON_DEFINE_NUMBER_MAP__ (int32_t*);
	__GSON_DEFINE_NUMBER_MAP__ (uint32_t*);
	__GSON_DEFINE_NUMBER_MAP__ (int64_t*);
	__GSON_DEFINE_NUMBER_MAP__ (uint64_t*);
	__GSON_DEFINE_DOUBLE_MAP__ (float);
	__GSON_DEFINE_DOUBLE_MAP__ (double);
	__GSON_DEFINE_DOUBLE_MAP__ (float*);
	__GSON_DEFINE_DOUBLE_MAP__ (double*);

	static inline const char *get_value (const char *value)
	{
		return value;
	}

	template<typename V>
	static inline acl::json_node &gson (acl::json &json,
										const std::list<V> &objects)
	{
		acl::json_node &node = json.create_array ();
		for (std::list<V>::const_iterator
				itr = objects.begin (); itr != objects.end (); itr++)
		{
			add_item (json, node, *itr);
		}
		return node;
	}
	template<typename V>
	static inline acl::json_node &gson (acl::json &json,
										const std::list<V> *objects)
	{
		if (!objects)
			// {'a':[]} for empty list.
			return json.create_array ();
		return gson (json, *objects);
	}

	template<class V>
	static inline acl::json_node &gson (acl::json &json,
										const std::vector<V> &objects)
	{
		acl::json_node &node = json.create_array ();
		for (std::vector<V>::const_iterator
				itr = objects.begin (); itr != objects.end (); itr++)
		{
			add_item (json, node, *itr);
		}
		return node;
	}

	template<class T, class V>
	static inline acl::json_node &gson (acl::json &json,
										const std::map<T, V> &objects)
	{

		acl::json_node &node = json.create_array ();
		for (std::map<T, V>::const_iterator
				itr = objects.begin (); itr != objects.end (); itr++)
		{
			const char *tag = get_value(itr->first);
			acl::json_node &item= gson(json, itr->second);
			node.add_child (json.create_node ().add_child (tag, item));
		}
		return node;
	}
}
}
#endif

//////////////////////////////////////////////////////////////////////////