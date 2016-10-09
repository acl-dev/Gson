#pragma once
#include "bson.h"
#include "struct.h"

bool gson (bson_iter_t &iter, user_t &obj);
bool gson (bson_iter_t &iter, group_t &obj);

bool gson (const user_t &user, bson_t &bson);
bool gson (const group_t &group, bson_t &bson);
