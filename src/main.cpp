#include <iostream>
#include <string>

#include <curl/curl.h>
#include <rapidjson/document.h>

using namespace std;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    auto curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.binance.com/api/v3/depth?limit=5000&symbol=BTCUSDT");
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

        std::string response_string;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        curl_easy_perform(curl);
        rapidjson::Document response;
        if (response.Parse<0>(response_string.c_str()).HasParseError()) {
            std::cout << "Error when request to binance, response = " << response_string;
        }
        const rapidjson::Value& bids = response["bids"];
        assert(a.IsArray());
        for (rapidjson::SizeType i = 0; i < bids.Size(); i++)
            std::cout << "a[" << i << ']' << " = " << bids[i].GetFloat();
        const rapidjson::Value& asks = response["asks"];
        //cout << response_string;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        curl = NULL;
    }
}