namespace acl
{
    typedef std::pair<bool, std::string> result_t;

    //group_t
    bool gson (const group_t&obj, bson_t &$bson);
    bool gson (const group_t*obj, bson_t &$bson);
    result_t gson(bson_t &bson,      group_t&obj);
    result_t gson(bson_iter_t &$itr,group_t &obj);
    result_t gson(bson_iter_t &$itr,group_t *obj);

    //user_t
    bool gson (const user_t&obj, bson_t &$bson);
    bool gson (const user_t*obj, bson_t &$bson);
    result_t gson(bson_t &bson,      user_t&obj);
    result_t gson(bson_iter_t &$itr,user_t &obj);
    result_t gson(bson_iter_t &$itr,user_t *obj);
}///end of acl.
