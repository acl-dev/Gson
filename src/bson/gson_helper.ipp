#pragma once
#include "bson.h"
#include <string>
#include <list>


struct true_type
{
	const static bool value = true;
};
struct false_type
{
	const static bool value = false;
};

template<class T>
struct _Is_number
	: false_type
{
};
template<>
struct _Is_number<unsigned short>
	: true_type
{
};

template<>
struct _Is_number<signed short>
	: true_type
{
};

template<>
struct _Is_number<unsigned int>
	: true_type
{
};

template<>
struct _Is_number<signed int>
	: true_type
{
};

template<>
struct _Is_number<unsigned long>
	: true_type
{
};

template<>
struct _Is_number<signed long>
	: true_type
{
};

template<>
struct _Is_number<long long>
	: true_type
{
};

template<>
struct _Is_number<unsigned long long>
	: true_type
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
struct _Is_string :false_type
{
};
template<>
struct _Is_string<std::string> :true_type
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
struct _Is_double :false_type
{
};
template<>
struct _Is_double<float> :true_type
{
};
template<>
struct _Is_double<double> :true_type
{
};
template<>
struct _Is_double<long double> :true_type
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
struct _Is_char_ptr :false_type
{
};
template<>
struct _Is_char_ptr<char*> :true_type
{
};
template<class T>
struct is_char_ptr :_Is_char_ptr<typename std::remove_cv<T>::type>
{
};

template<class T>
struct _Is_bool :false_type
{
};
template<>
struct _Is_bool<bool> :true_type
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
	:false_type
{
};

template<>
struct _Is_object<true> :true_type
{
};

template<class T>
struct is_object :_Is_object<
	!is_string<T>::value &&
	!is_double<T>::value &&
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
typename std::enable_if<std::is_floating_point<T>::value, T>::type
static inline  get_value(const T *t)
{
	return *t;
}
template<class T>
typename std::enable_if<!std::is_pointer<T>::value, bool>::type
static inline check_nullptr(const T &, result_t *,const char *)
{
	return true;
}

template<class T>
typename std::enable_if<!std::is_pointer<T>::value, bool>::type
static inline check_nullptr(const T *t, result_t *result, 
							const char *error_point)
{
	if(t == NULL)
	{
		if(result)
		{
			result->first = false;
			result->second.append("{[");
			result->second.append(error_point);
			result->second.append("] null }");
		}
		return false;
	}
	return true;
}

template<class T>
typename std::enable_if<!std::is_pointer<T>::value, bool>::type
static inline check_nullptr(const T &)
{
	return true;
}

template<class T>
typename std::enable_if<!std::is_pointer<T>::value, bool>::type
static inline check_nullptr(const T *t)
{
	if(t == NULL)
	{
		return false;
	}
	return true;
}

struct destroy_bson_t
{
	destroy_bson_t(bson_t &bson)
		:bson_(bson)
	{

	}
	~destroy_bson_t()
	{
		bson_destroy(&bson_);
	}
	bson_t &bson_;
};
//bool *
static inline bool gson(bson_iter_t &itr, bson_oid_t *obj)
{
	bson_type_t type = bson_iter_type(&itr);
	if(type == BSON_TYPE_OID)
	{
		*obj = *bson_iter_oid(&itr);
		return true;
	}
	return false;
}

//bool *
static inline bool gson(bson_iter_t &itr, bool *obj)
{
	bson_type_t type = bson_iter_type(&itr);
	if(type == BSON_TYPE_INT32)
	{
		*obj = bson_iter_as_bool(&itr);
		return true;
	}
	return false;
}

//int32_t *
static inline bool gson(bson_iter_t &itr, int32_t *obj)
{
	bson_type_t type = bson_iter_type(&itr);
	if(type == BSON_TYPE_INT32 )
	{
		*obj = bson_iter_int32(&itr);
		return true;
	}
	return false;
}
//int32_t *
static inline bool gson(bson_iter_t &itr, int64_t *obj)
{
	bson_type_t type = bson_iter_type(&itr);
	if(type == BSON_TYPE_INT64)
	{
		*obj = bson_iter_int64(&itr);
		return true;
	}
	return false;
}

//double *
static inline bool gson(bson_iter_t &itr, double *obj)
{
	bson_type_t type = bson_iter_type(&itr);
	if(type == BSON_TYPE_DOUBLE)
	{
		*obj = bson_iter_double(&itr);
		return true;
	}
	return false;
}
//string *
template<class T>
typename std::enable_if<is_string<T>::value,bool>::type
static inline gson(bson_iter_t &itr, T *obj)
{
	bson_type_t type = bson_iter_type(&itr);
	if(type == BSON_TYPE_UTF8)
	{
		size_t len = 0;
		const char *buf = bson_iter_utf8(&itr, &len);
		if(buf)
		{
			obj->append(buf, len);
			return true;
		}
	}
	return false;
}
//char **
static inline bool gson(bson_iter_t &itr, char **obj)
{
	bson_type_t type = bson_iter_type(&itr);
	if(type == BSON_TYPE_UTF8)
	{
		size_t len = 0;
		const char *buf = bson_iter_utf8(&itr, &len);
		if(buf)
		{
			*obj = new char[len+1];
			memcpy(*obj, buf, len);
			(*obj)[len] = 0;
			return true;
		}
	}
	return false;
}

static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const double *obj)
{
	if( obj == NULL || 
	   bson_append_double(&bson, obj_name, strlen(obj_name), *obj) == false)
		return false;
	return true;
}

static inline bool bson_append_obj(bson_t &bson, const char *obj_name, double obj)
{
	if(bson_append_double(&bson, obj_name, strlen(obj_name), obj) == false)
		return false;
	return true;
}

static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const char *obj)
{
	if(obj == NULL ||
	   bson_append_utf8(&bson, obj_name,strlen(obj_name),obj, strlen(obj)) == false)
		return false;
	return true;
}

static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const std::string &obj)
{
	if(bson_append_utf8(&bson, obj_name, strlen(obj_name), obj.c_str(), obj.size()) == false)
		return false;
	return true;
}

static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const std::string *obj)
{
	if(obj == NULL ||
		bson_append_utf8(&bson, obj_name, strlen(obj_name), obj->c_str(), obj->size()) == false)
		return false;
	return true;
}
#if 0

static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const acl::string &obj)
{
	if(bson_append_utf8(&bson, obj_name, strlen(obj_name), obj.c_str(), obj.size()) == false)
		return false;
	return true;
}

static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const acl::string *obj)
{
	if(obj == NULL ||
	   bson_append_utf8(&bson, obj_name, strlen(obj_name), obj->c_str(), obj->size()) == false)
		return false;
	return true;
}

#endif

static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const  bson_oid_t *obj)
{
	if(obj == NULL ||
	   bson_append_oid(&bson, obj_name, strlen(obj_name), obj) == false)
		return false;
	return true;
}

static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const bson_oid_t &obj)
{
	if(bson_append_oid(&bson, obj_name, strlen(obj_name), &obj) == false)
		return false;
	return true;
}

template<class T>
static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const T &obj)
{
	bson_t child;
	destroy_bson_t d(child);
	bson_init(&child);
	if(gson(obj, child) == false)
		return false;
	if(bson_append_document(&bson, "obj_name", strlen(obj_name), &child) == false)
		return false;
}

template<class T>
static inline bool bson_append_obj(bson_t &bson, const char *obj_name, const std::list<T> &objs,)
{
	int i = 0;
	bson_t child;
	bson_init(&child);
	destroy_bson_t d(child);
	for(auto &itr : objs)
	{
		bson_t iter;
		destroy_bson_t d_user(iter);
		bson_init(&iter);
		if(gson(itr, iter) == false)
			return false;
		if(bson_append_document(&child,
		   std::to_string(i).c_str(), -1, &iter) == false)
			return false;
		i++;
	}
	if(bson_append_array(&bson, obj_name, -1, &bson) == false)
		return false;
	return true;
}
static inline bool gson(bson_iter_t &iter, std::list<user_t> *obj)
{
	while(bson_iter_next(&iter))
	{
		bson_iter_t user_iter;
		if(bson_iter_recurse(&iter, &user_iter) == false)
			return false;
		user_t user;
		if(gson(user_iter, &user) == false)
			return false;
		obj->push_back(user);
	}
	return !!obj->size();
}
template<class T>
static inline bool gson(bson_iter_t &itr, T**obj)
{
	*obj = new T;
	if(gson(itr, *obj) == false)
	{
		delete *obj;
		*obj = NULL;
		return false;
	}
	return true;
}

static inline std::string to_text(const std::string &str)
{
	std::string result;
	std::size_t pos = 0;
	result.append("\"");
	while(pos < str.size())
	{
		if(str[pos] == '\\r')
		{
			result.append("\\r");
		}
		else if(str[pos] == '\\n')
		{
			result.append("\\n");
		}
		else if(str[pos] == '\\t')
		{
			result.append("\\t");
		}
		else if(str[pos] == '"')
		{
			result.append("\\\"");
		}
		else
		{
			result.push_back(str[pos]);
		}
		pos++;
	}
	result.append("\"");
	return result;
}