#include <drogon/HttpAppFramework.h>
#include <drogon/HttpResponse.h>
#include <filesystem>
#include <fstream>
#include "lib/se_core/se.h"

SE ENGINE = SE();

void indexPage(const drogon::HttpRequestPtr &req, 
        std::function<void(const drogon::HttpResponsePtr &)> &&callback) {

    auto response = drogon::HttpResponse::newHttpViewResponse("index_page");

    callback(response);
};

void lookup(const drogon::HttpRequestPtr &req, 
    std::function<void(const drogon::HttpResponsePtr &)> &&callback
    ) {

    std::string query = req->query();
    std::string q;

    for (int idx = 0; idx < query.length(); idx++) {
        if (query[idx] == '=') {
            idx += 1;

            while (idx < query.length()) {
                q += query[idx];
                idx++;
            }

            break;
        }
    }

    std::vector<std::pair<std::string, float>> result = ENGINE.search(q);
    std::vector<std::string> shortText;

    for (int idx = 0; idx < result.size(); idx++) {
        shortText.push_back(textMatch(result[idx].first, q));
    }

    drogon::HttpViewData data;
    data.insert("result", result);
    data.insert("shortText", shortText);

    auto response = drogon::HttpResponse::newHttpViewResponse("search_results", data);

    callback(response);
};

void loadDoc(const drogon::HttpRequestPtr &req, 
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    std::string path) {

    std::ifstream file;
    char* result = new char[std::filesystem::file_size(path)];

    file.open(path);
    file.read(result, std::filesystem::file_size(path));

    drogon::HttpViewData data;
    data.insert("doc", result);

    auto response = drogon::HttpResponse::newHttpViewResponse("doc_page", data);

    callback(response);
};

int main() {
    drogon::app()
        .addListener("0.0.0.0", 1024)
        .registerHandler("/", &indexPage, {drogon::Get})
        .registerHandler("/search", &lookup, {drogon::Get})
        .registerHandlerViaRegex("(.*)", &loadDoc, {drogon::Get})
        .run();

    return 0;
}

