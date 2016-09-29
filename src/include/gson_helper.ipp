#ifndef __GSON_HELPER__
#define __GSON_HELPER__

#define __GSON__NOTHING__ 

#define __GSON_DEFINE_ADD_ITEM__(TYPE,FUNC,P)\
static inline void add_item(acl::json &json,\
							acl::json_node &node, \
							TYPE value)\
{\
	node.##FUNC( P value);\
}

#define __GSON_DEFINE_TO_STR__(TYPE,P)\
static inline const char* to_str(const TYPE &value)\
{\
	return value##P##c_str();\
}

#define __GSON_DEFINE_GET_VALUE__(TYPE,PTR)\
static inline TYPE get_value(TYPE PTR value)\
{\
	return  PTR##value;\
}

//define_string_map
#define __GSON_DEFINE_STRING_MAP__(TYPE)\
template<class K>\
static inline acl::json_node &to_json(acl::json &json,\
									 std::map<K, TYPE> objects)\
{\
	acl::json_node &node = json.create_array();\
	for(auto &itr : objects)\
	{\
		node.add_child(\
			json.create_node().add_text( \
			to_str(itr.first),\
			to_str(itr.second))\
		);\
	}\
	return node;\
}

//define number map
#define __GSON_DEFINE_NUMBER_MAP__(TYPE)\
template<class K>\
static inline acl::json_node &to_json(acl::json &json,\
									  std::map<K, TYPE> objects)\
{\
	acl::json_node &node = json.create_array();\
	for(auto &itr : objects)\
	{\
		node.add_child(\
			json.create_node().add_number(\
			to_str(itr.first),\
			get_value(itr.second))\
		);\
	}\
	return node;\
}

//define bool map
#define __GSON_DEFINE_BOOL_MAP__(TYPE)\
template<class K>\
static inline acl::json_node &to_json(acl::json &json,\
									  std::map<K, TYPE> objects)\
{\
	acl::json_node &node = json.create_array();\
	for(auto &itr : objects)\
	{\
		node.add_child(\
			json.create_node().add_bool(\
			to_str(itr.first),\
			get_value(itr.second))\
		);\
	}\
	return node;\
}

//define bool map
#define __GSON_DEFINE_DOUBLE_MAP__(TYPE)\
template<class K>\
static inline acl::json_node &to_json(acl::json &json,\
									  std::map<K, TYPE> objects)\
{\
	acl::json_node &node = json.create_array();\
	for(auto &itr : objects)\
	{\
		node.add_child(\
			json.create_node().add_double(\
			to_str(itr.first),\
			get_value(itr.second))\
		);\
	}\
	return node;\
}


template<class T>
inline void add_item(acl::json &json, acl::json_node &node, const T &user)
{
	node.add_child(to_json(json, user));
}
template<class T>
inline void add_item(acl::json &json, acl::json_node &node, const T *user)
{
	return add_item(json, node, *user);
}

__GSON_DEFINE_ADD_ITEM__(bool, add_array_bool,);
__GSON_DEFINE_ADD_ITEM__(int8_t, add_array_number,);
__GSON_DEFINE_ADD_ITEM__(uint8_t, add_array_number,);
__GSON_DEFINE_ADD_ITEM__(int16_t, add_array_number,);
__GSON_DEFINE_ADD_ITEM__(uint16_t, add_array_number,);
__GSON_DEFINE_ADD_ITEM__(int32_t, add_array_number,);
__GSON_DEFINE_ADD_ITEM__(uint32_t, add_array_number,);
__GSON_DEFINE_ADD_ITEM__(int64_t, add_array_number,);
__GSON_DEFINE_ADD_ITEM__(uint64_t, add_array_number,);
__GSON_DEFINE_ADD_ITEM__(float, add_array_double,);
__GSON_DEFINE_ADD_ITEM__(double, add_array_double,);

__GSON_DEFINE_ADD_ITEM__(bool*, add_array_bool, *);
__GSON_DEFINE_ADD_ITEM__(int8_t*, add_array_number,*);
__GSON_DEFINE_ADD_ITEM__(uint8_t*, add_array_number,*);
__GSON_DEFINE_ADD_ITEM__(int16_t*, add_array_number,*);
__GSON_DEFINE_ADD_ITEM__(uint16_t*, add_array_number,*);
__GSON_DEFINE_ADD_ITEM__(int32_t*, add_array_number,*);
__GSON_DEFINE_ADD_ITEM__(uint32_t*, add_array_number,*);
__GSON_DEFINE_ADD_ITEM__(int64_t*, add_array_number,*); 
__GSON_DEFINE_ADD_ITEM__(uint64_t*, add_array_number,*);


__GSON_DEFINE_ADD_ITEM__(float*, add_array_double,*);
__GSON_DEFINE_ADD_ITEM__(double*, add_array_double,*);



__GSON_DEFINE_TO_STR__(acl::string,.);
__GSON_DEFINE_TO_STR__(std::string,.);
__GSON_DEFINE_TO_STR__(acl::string*,->);
__GSON_DEFINE_TO_STR__(std::string*,->);


__GSON_DEFINE_GET_VALUE__(bool,);
__GSON_DEFINE_GET_VALUE__(int8_t,);
__GSON_DEFINE_GET_VALUE__(uint8_t,);
__GSON_DEFINE_GET_VALUE__(int16_t,);
__GSON_DEFINE_GET_VALUE__(uint16_t,);
__GSON_DEFINE_GET_VALUE__(int32_t,);
__GSON_DEFINE_GET_VALUE__(uint32_t,);
__GSON_DEFINE_GET_VALUE__(int64_t,);
__GSON_DEFINE_GET_VALUE__(uint64_t,);
__GSON_DEFINE_GET_VALUE__(float,);
__GSON_DEFINE_GET_VALUE__(double,);

__GSON_DEFINE_GET_VALUE__(bool*, );
__GSON_DEFINE_GET_VALUE__(int8_t,*);
__GSON_DEFINE_GET_VALUE__(uint8_t,*);
__GSON_DEFINE_GET_VALUE__(int16_t,*);
__GSON_DEFINE_GET_VALUE__(uint16_t,*);
__GSON_DEFINE_GET_VALUE__(int32_t,*);
__GSON_DEFINE_GET_VALUE__(uint32_t,*);
__GSON_DEFINE_GET_VALUE__(int64_t,*);
__GSON_DEFINE_GET_VALUE__(uint64_t,*);
__GSON_DEFINE_GET_VALUE__(float*, );
__GSON_DEFINE_GET_VALUE__(double*, );


__GSON_DEFINE_STRING_MAP__(std::string);
__GSON_DEFINE_STRING_MAP__(acl::string);
__GSON_DEFINE_STRING_MAP__(char*);
__GSON_DEFINE_STRING_MAP__(const char*);
__GSON_DEFINE_STRING_MAP__(std::string*);
__GSON_DEFINE_STRING_MAP__(acl::string*);


__GSON_DEFINE_BOOL_MAP__(bool);
__GSON_DEFINE_BOOL_MAP__(bool*);
__GSON_DEFINE_NUMBER_MAP__(int8_t);
__GSON_DEFINE_NUMBER_MAP__(uint8_t);
__GSON_DEFINE_NUMBER_MAP__(int16_t);
__GSON_DEFINE_NUMBER_MAP__(uint16_t);
__GSON_DEFINE_NUMBER_MAP__(int32_t);
__GSON_DEFINE_NUMBER_MAP__(uint32_t);
__GSON_DEFINE_NUMBER_MAP__(int64_t);
__GSON_DEFINE_NUMBER_MAP__(uint64_t);
__GSON_DEFINE_NUMBER_MAP__(int8_t*);
__GSON_DEFINE_NUMBER_MAP__(uint8_t*);
__GSON_DEFINE_NUMBER_MAP__(int16_t*);
__GSON_DEFINE_NUMBER_MAP__(uint16_t*);
__GSON_DEFINE_NUMBER_MAP__(int32_t*);
__GSON_DEFINE_NUMBER_MAP__(uint32_t*);
__GSON_DEFINE_NUMBER_MAP__(int64_t*);
__GSON_DEFINE_NUMBER_MAP__(uint64_t*);
__GSON_DEFINE_DOUBLE_MAP__(float);
__GSON_DEFINE_DOUBLE_MAP__(double);
__GSON_DEFINE_DOUBLE_MAP__(float*);
__GSON_DEFINE_DOUBLE_MAP__(double*);

static inline const char *to_str(const char *value)
{
	return value;
}

template<typename V>
static inline acl::json_node &to_json(acl::json &json, const std::list<V> &objects)
{
	acl::json_node &node = json.create_array();
	for(auto itr : objects)
	{
		add_item(json, node, itr);
	}
	return node;
}

template<class V>
static inline acl::json_node &to_json(acl::json &json,const std::vector<V> &objects)
{

	acl::json_node &node = json.create_array();
	for(auto itr : objects)
	{
		add_item(json, node, itr);
	}
	return node;
}

template<class T, class V>
static inline acl::json_node &to_json(acl::json &json,const std::map<T, V> &objects)
{

	acl::json_node &node = json.create_array();
	for(auto itr : objects)
	{
		node.add_child(
			json.create_node().add_child(
			to_str(itr.first),
			to_json(json, itr.second))
		);
	}
	return node;
}
#endif

//////////////////////////////////////////////////////////////////////////