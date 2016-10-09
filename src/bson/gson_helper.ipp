#pragma once
#include "bson.h"
#include <string>
#include <list>

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
typename std::enable_if<is_string<T>::value && std::is_pointer<T>::value, const char *>::type
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
 check_value(const T &, result_t *,const char *)
{
	return true;
}

template<class T>
typename std::enable_if<!std::is_pointer<T>::value, bool>::type
check_value(const T *t, result_t *result, const char *error_point)
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

static inline bool gson(bson_iter_t &itr, int32_t *obj)
{
	bson_type_t type = bson_iter_type(&itr);
	if(type == BSON_TYPE_INT32 ||
	   type == BSON_TYPE_INT64)
	{
		*obj = bson_iter_int32(&itr);
		return true;
	}
	return false;
}

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

static inline bool gson(bson_iter_t &itr, std::string *obj)
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

static inline bool gson(const std::list<user_t> &users, bson_t &bson)
{
	int i = 0;
	for(auto &itr : users)
	{
		bson_t user;
		destroy_bson_t d_user(user);
		bson_init(&user);
		if(gson(itr, user) == false)
			return false;
		if(bson_append_document(&bson, std::to_string(i).c_str(), -1, &user) == false)
			return false;
		i++;
	}
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
	return gson(itr, *obj);
}

static inline std::string to_text(const std::string &str)
{
	std::string result;
	int pos = 0;
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