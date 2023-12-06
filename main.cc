#include <drogon/HttpAppFramework.h>
#include <filesystem>
#include <fstream>
#include "lib/core/se.h"

using namespace std;

se::Cache CA = se::Cache("path/to/directory/with/documents");
se::SE ENGINE = se::SE(CA);

void indexPage(const drogon::HttpRequestPtr &req, 
        function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto response = drogon::HttpResponse::newHttpResponse();

    response->setBody(R"(
    <html>
        <head>
            <script src="https://unpkg.com/htmx.org@1.9.9"></script>
        </head>
        <body>
            <form hx-get="/search" hx-target="#result">
                <input type="text" name="search">
                <input type="submit" value="Search">
            </form>
            <div id="result"></div>
        </body>
    </html>
    )");

    callback(response);
};

void lookup(const drogon::HttpRequestPtr &req, 
    function<void(const drogon::HttpResponsePtr &)> &&callback
    ) {

    string query = req->query();
    string q;

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

    vector<pair<string, float>> result = ENGINE.search(q + " ");

    string responseBody = "<div>";

    for (auto const [path, val]: result) {
        responseBody += "<p><a href=\"" + path + "\">" + path + "</a></p>";
    };

    responseBody += "</div>";
    auto response = drogon::HttpResponse::newHttpResponse();

    response->setBody(responseBody);

    callback(response);
};

void loadDoc(const drogon::HttpRequestPtr &req, 
    function<void(const drogon::HttpResponsePtr &)> &&callback,
    string path) {

    auto response = drogon::HttpResponse::newHttpResponse();

    ifstream file;
    char* result = new char[filesystem::file_size(path)];

    file.open(path);
    file.read(result, filesystem::file_size(path));

    response->setBody(result);

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

