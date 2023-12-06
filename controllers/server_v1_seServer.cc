//#include "server_v1_seServer.h"
//
//const std::string PATH = "/home/mxk/dev/projects/search_engine/src";
//se::Cache CA = se::Cache();
//se::SE ENGINE = se::SE(CA);
//
//void seServer::configure() {
//    this->engine = ENGINE;
//};
//
//void seServer::index(const drogon::HttpRequestPtr &req, 
//    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
//    auto response = drogon::HttpResponse::newHttpResponse();
//
//    response->setBody("<div>Hello!</div>");
//    callback(response);
//};
//
//void seServer::search(const drogon::HttpRequestPtr &req, 
//    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
//    std::string query) {
//
//    std::vector<std::pair<std::string, float>> result = this->engine.search(query);
//
//    std::vector<std::string> truePath;
//    std::string responseBody = "<div>";
//
//    for (auto const [path, val]: result) {
//        int idx = 0;
//        int count = 0;
//
//        while (true) {
//            if (path[idx] == '/') {
//                count += 1;
//            }
//
//            if (count == 2) {
//                std::string fullPath = PATH;
//
//                while (idx < path.length()) {
//                    fullPath += path[idx];
//                    idx ++;
//                }
//
//                responseBody += "<p><a href=\"" + fullPath + "\">" + fullPath + "</a></p>";
//
//                break;
//            }
//
//            idx++;
//        };
//    };
//
//    responseBody += "</div>";
//    auto response = drogon::HttpResponse::newHttpResponse();
//
//    response->setBody(responseBody);
//
//    callback(response);
//};
//
//void seServer::loadDoc(const drogon::HttpRequestPtr &req, 
//    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
//    std::string path) {
//};
//
