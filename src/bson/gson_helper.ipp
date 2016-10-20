#pragma once
#include "bson.h"
#include <string>
#include <list>
#include <vector>
#include <map>

namespace acl
{

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
	typedef std::pair<bool, std::string> result_t;

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
		static inline check_nullptr(const T &, result_t *, const char *)
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

	static inline std::string bson_type_to_string(bson_type_t type)
	{
		switch(type)
		{
		case BSON_TYPE_EOD:
			return "BSON_TYPE_EOD";
		case BSON_TYPE_DOUBLE:
			return "BSON_TYPE_DOUBLE";
		case BSON_TYPE_UTF8:
			return "BSON_TYPE_UTF8";
		case BSON_TYPE_DOCUMENT:
			return "BSON_TYPE_DOCUMENT";
		case BSON_TYPE_ARRAY:
			return "BSON_TYPE_ARRAY";
		case BSON_TYPE_BINARY:
			return "BSON_TYPE_BINARY";
		case BSON_TYPE_UNDEFINED:
			return "BSON_TYPE_UNDEFINED";
		case BSON_TYPE_OID:
			return "BSON_TYPE_OID";
		case BSON_TYPE_BOOL:
			return "BSON_TYPE_BOOL";
		case BSON_TYPE_DATE_TIME:
			return "BSON_TYPE_DATE_TIME";
		case BSON_TYPE_NULL:
			return "BSON_TYPE_NULL";
		case BSON_TYPE_REGEX:
			return "BSON_TYPE_REGEX";
		case BSON_TYPE_DBPOINTER:
			return "BSON_TYPE_DBPOINTER";
		case BSON_TYPE_CODE:
			return "BSON_TYPE_CODE";
		case BSON_TYPE_SYMBOL:
			return "BSON_TYPE_SYMBOL";
		case BSON_TYPE_CODEWSCOPE:
			return "BSON_TYPE_CODEWSCOPE";
		case BSON_TYPE_INT32:
			return "BSON_TYPE_INT32";
		case BSON_TYPE_TIMESTAMP:
			return "BSON_TYPE_TIMESTAMP";
		case BSON_TYPE_INT64:
			return "BSON_TYPE_INT64";
		case BSON_TYPE_MAXKEY:
			return "BSON_TYPE_MAXKEY";
		case BSON_TYPE_MINKEY:
			return "BSON_TYPE_MINKEY";
		}
		return "unknown type";
	}
	template<class T>
	static inline result_t
		bson_get_obj(bson_iter_t &itr, T**obj);

	//bson_oid_t *
	static inline  result_t bson_get_obj(bson_iter_t &itr, bson_oid_t *obj)
	{
		bson_type_t type = bson_iter_type(&itr);
		if(type == BSON_TYPE_OID)
		{
			*obj = *bson_iter_oid(&itr);
			return std::make_pair(true, "");
		}
		return std::make_pair(false, bson_type_to_string(type) + " type error");
	}

	//bool *
	static inline result_t bson_get_obj(bson_iter_t &itr, bool *obj)
	{
		bson_type_t type = bson_iter_type(&itr);
		if(type == BSON_TYPE_BOOL)
		{
			*obj = bson_iter_as_bool(&itr);
			return std::make_pair(true, "");
		}
		return std::make_pair(false, bson_type_to_string(type) + " type error");
	}

	//int32_t *
	static inline result_t bson_get_obj(bson_iter_t &itr, int32_t *obj)
	{
		bson_type_t type = bson_iter_type(&itr);
		if(type == BSON_TYPE_INT32)
		{
			*obj = bson_iter_int32(&itr);
			return std::make_pair(true, "");
		}
		return std::make_pair(false, bson_type_to_string(type) + " type error");
	}
	//int32_t *
	static inline result_t bson_get_obj(bson_iter_t &itr, int64_t *obj)
	{
		bson_type_t type = bson_iter_type(&itr);
		if(type == BSON_TYPE_INT64)
		{
			*obj = bson_iter_int64(&itr);
			return std::make_pair(true, "");
		}
		return std::make_pair(false, bson_type_to_string(type) + " type error");
	}

	//double *
	static inline result_t bson_get_obj(bson_iter_t &itr, double *obj)
	{
		bson_type_t type = bson_iter_type(&itr);
		if(type == BSON_TYPE_DOUBLE)
		{
			*obj = bson_iter_double(&itr);
			return std::make_pair(true, "");
		}
		return std::make_pair(false, bson_type_to_string(type) + " type error");
	}
	//string *
	template<class T>
	typename std::enable_if<is_string<T>::value, result_t >::type
		static inline bson_get_obj(bson_iter_t &itr, T *obj)
	{
		bson_type_t type = bson_iter_type(&itr);
		if(type == BSON_TYPE_UTF8)
		{
			size_t len = 0;
			const char *buf = bson_iter_utf8(&itr, &len);
			if(buf)
			{
				obj->append(buf, len);
				return std::make_pair(true, "");
			}
		}
		return std::make_pair(false, bson_type_to_string(type) + " type error");
	}
	//char **
	static inline
		result_t bson_get_obj(bson_iter_t &itr, char **obj)
	{
		bson_type_t type = bson_iter_type(&itr);
		if(type == BSON_TYPE_UTF8)
		{
			size_t len = 0;
			const char *buf = bson_iter_utf8(&itr, &len);
			if(buf)
			{
				*obj = new char[len + 1];
				memcpy(*obj, buf, len);
				(*obj)[len] = 0;
				return std::make_pair(true, "");
			}
		}
		return std::make_pair(false, bson_type_to_string(type) + " type error");
	}

	template<class T>
	static inline result_t
		gson(bson_iter_t &iter, std::list<T> *obj);

	template<class T>
	static inline result_t
		gson(bson_iter_t &iter, std::vector<T> *obj);

	template<class K, class T>
	static inline result_t
		gson(bson_iter_t &iter, std::map<K, T> *obj);

	//bson to obj
	template<class T>
	static inline result_t
		gson(bson_iter_t &iter, std::list<T> *obj)
	{
		std::pair<bool, std::string > result;
		while(bson_iter_next(&iter))
		{
			bson_iter_t child;
			if(bson_iter_recurse(&iter, &child) == false)
				return std::make_pair(false, "bson_iter_recurse");
			T tmp;
			if(result = gson(child, &tmp), !result.first)
				return result;
			obj->push_back(tmp);
		}
		return std::make_pair(!!obj->size(), "");
	}
	//vector
	template<class T>
	static inline result_t
		gson(bson_iter_t &iter, std::vector<T> *obj)
	{
		std::pair<bool, std::string > result;
		while(bson_iter_next(&iter))
		{
			bson_iter_t child;
			if(bson_iter_recurse(&iter, &child) == false)
				return std::make_pair(false, "bson_iter_recurse");
			T tmp;
			if(result = gson(child, &tmp), !result.first)
				return result;
			obj->push_back(tmp);
		}
		return std::make_pair(!!obj->size(), "");
	}
	//map
	template<class K, class T>
	static inline result_t
		gson(bson_iter_t &iter, std::map<K, T> *obj)
	{
		std::pair<bool, std::string > result;
		while(bson_iter_next(&iter))
		{
			bson_iter_t child;
			if(bson_iter_recurse(&iter, &child) == false)
				return std::make_pair(false, "bson_iter_recurse");
			T tmp;
			if(result = gson(child, &tmp), !result.first)
				return result;
			obj->insert(std::make_pair(bson_iter_key(&iter), tmp));
		}
		return std::make_pair(!!obj->size(), "");
	}


	template<class T>
	static inline result_t
		bson_get_obj(bson_iter_t &itr, T**obj)
	{
		*obj = new T;
		result_t result(true, "");
		if(result = bson_get_obj(itr, *obj), !result.first)
		{
			delete *obj;
			*obj = NULL;
			return result;
		}
		return result;
	}

	// obj to bson

	template<class T>
	static inline bool gson(const std::list<T> &objs, bson_t &bson);

	template<class T>
	static inline bool gson(const std::list<T> *objs, bson_t &bson);

	template<class T>
	static inline bool gson(const std::vector<T> &objs, bson_t &bson);

	template<class T>
	static inline bool gson(const std::vector<T> *objs, bson_t &bson);

	template<class K, class T>
	static inline bool gson(const std::map<K, T> &objs, bson_t &bson);

	template<class K, class T>
	static inline bool gson(const std::map<K, T> *objs, bson_t &bson);


	template<class T>
	static inline bool gson(const std::list<T> &objs, bson_t &bson)
	{
		int i = 0;
		for(std::list<T>::const_iterator itr = objs.begin();
			itr != objs.end();
			++itr)
		{
			bson_t child2;
			destroy_bson_t d2(child2);
			bson_init(&child2);
			if(gson(*itr, child2) == false)
				return false;
			if(bson_append_document(&bson,
			   std::to_string(i).c_str(), -1, &child2) == false)
				return false;
			i++;
		}
		return true;
	}
	template<class T>
	static inline bool gson(const std::list<T> *objs, bson_t &bson)
	{
		if(objs == NULL)
			return false;
		return gson(bson, *objs);
	}

	template<class T>
	static inline bool gson(const std::vector<T> &objs, bson_t &bson)
	{
		int i = 0;
		for(std::vector<T>::const_iterator  itr = objs.begin();
			itr != objs.end();
			++itr)
		{
			bson_t child2;
			destroy_bson_t d2(child2);
			bson_init(&child2);
			if(gson(*itr, child2) == false)
				return false;
			if(bson_append_document(&bson,
			   std::to_string(i).c_str(), -1, &child2) == false)
				return false;
			i++;
		}
		return true;
	}
	template<class T>
	static inline bool gson(const std::vector<T> *objs, bson_t &bson)
	{
		if(objs == NULL)
			return false;
		return gson(bson, *objs);
	}

	template<class K, class T>
	static inline bool gson(const std::map<K, T> &objs, bson_t &bson)
	{
		int i = 0;
		for(std::map<K, T>::const_iterator  itr = objs.begin();
			itr != objs.end();
			++itr)
		{
			bson_t child2;
			destroy_bson_t d2(child2);
			bson_init(&child2);
			if(gson(itr->second, child2) == false)
				return false;
			if(bson_append_document(&bson, get_value(itr->first), -1, &child2) == false)
				return false;
			i++;
		}
		return true;
	}
	template<class K, class T>
	static inline bool gson(const std::map<K, T> *objs, bson_t &bson)
	{
		if(objs == NULL)
			return false;
		return gson(bson, obj_name, *objs);
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

}