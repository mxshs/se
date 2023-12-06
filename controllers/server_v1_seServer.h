#pragma once

#include <drogon/HttpController.h>
#include "../lib/core/se.h"

using namespace drogon;

class seServer : public drogon::HttpController<seServer>
{
    public:
        METHOD_LIST_BEGIN
        ADD_METHOD_TO(seServer::index, "/", drogon::Get);
        ADD_METHOD_TO(seServer::search, "/search/{1}", drogon::Get);
        ADD_METHOD_TO(seServer::loadDoc, "/{path}", drogon::Get);
        METHOD_LIST_END
        void index(const drogon::HttpRequestPtr &req, 
                std::function<void(const drogon::HttpResponsePtr &)> &&callback
                );
        void search(const drogon::HttpRequestPtr &req, 
                std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                std::string query   
                );
        void loadDoc(const drogon::HttpRequestPtr &req, 
                std::function<void(const drogon::HttpResponsePtr &)> &&callback,
                std::string path 
                );
    private:
        void configure();
        se::SE engine;
};

