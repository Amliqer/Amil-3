#include "YaApp.h"

string OW_API_KEY = "e7c0bce5516c376f0d980ddc956bb225";
string YA_API_KEY = "ec846f28-142d-4543-aba7-ec9a3195a6ff";

httplib::Client geoCli("http://api.openweathermap.org");

using json = nlohmann::json;

void YaScheduleApp::RequestResponceFrom() {
    std::string cityNameFrom;
    std::cout << "Vvedite gorod: ";
    std::cin >> cityNameFrom;

    httplib::Client geoCli("http://api.openweathermap.org");

    std::string requestFrom = "/geo/1.0/direct?";
    requestFrom += "q=" + cityNameFrom;
    requestFrom += "&limit=" + '10';
    requestFrom += "&appid=" + OW_API_KEY;

    auto responseFrom = geoCli.Get(requestFrom);    
}

void YaScheduleApp::ResultFrom() {

    json resultCodeFrom;

    httplib::Client cityCodeCli("http://api.rasp.yandex.net");
    std::string cityCodeReqFrom = "/v3.0/nearest_settlement/?";
    cityCodeReqFrom += "apikey=" + YA_API_KEY;

    if (auto responseCodeFrom = cityCodeCli.Get(cityCodeReqFrom)) {
        if (responseCodeFrom->status == 200) {
            resultCodeFrom = json::parse(responseCodeFrom->body);
        }
        else {
            std::cout << "Bad request" << std::endl;
        }
    }
    httplib::Client geoCli("http://api.openweathermap.org");

    std::string codeFrom = resultCodeFrom["code"].get<std::string>();

    std::string scheduleReq = "/v3.0/search/?";
    scheduleReq += "apikey=" + YA_API_KEY;
    scheduleReq += "&from=" + codeFrom;


    auto responseFrom = geoCli.Get(requestFrom);

    json resultFrom;
    if (responseFrom) {
        if (responseFrom->status == 200) {
            resultFrom = json::parse(responseFrom->body);
        }
        else {
            std::cout << "Bad request" << std::endl;
        }
    }

    std::string lonFrom = to_string(resultFrom[0]["lon"]);
    std::string latFrom = to_string(resultFrom[0]["lat"]);

    cityCodeReqFrom += "&lat=" + latFrom;
    cityCodeReqFrom += "&lng=" + lonFrom;

    std::cout << lonFrom << " " << latFrom << std::endl;
}
    
void YaScheduleApp::WhereTo() {
    using json = nlohmann::json;
    setlocale(LC_ALL, "RUS");

    std::string cityNameTo;
    std::cout << "Vvedite gorod naznacheniya: ";
    std::cin >> cityNameTo;


    httplib::Client geoCli("http://api.openweathermap.org");

    std::string requestTo = "/geo/1.0/direct?";
    requestTo += "q=" + cityNameTo;
    requestTo += "&limit=" + '10';
    requestTo += "&appid=" + OW_API_KEY;

    nlohmann::json resultTo;

    if (auto responseTo = geoCli.Get(requestTo)) {
        if (responseTo->status == 200) {
            resultTo = nlohmann::json::parse(responseTo->body);
        }
        else {
            std::cout << "Bad request" << std::endl;
        }
    }

    std::string lonTo = to_string(resultTo[0]["lon"]);
    std::string latTo = to_string(resultTo[0]["lat"]);

    std::cout << lonTo << " " << latTo << std::endl;


    httplib::Client cityCodeCli("http://api.rasp.yandex.net");

        

    json resultCodeTo;

    std::string cityCodeReqTo = "/v3.0/nearest_settlement/?";
    cityCodeReqTo += "apikey=" + YA_API_KEY;
    cityCodeReqTo += "&lat=" + latTo;
    cityCodeReqTo += "&lng=" + lonTo;

    if (auto responseCodeTo = cityCodeCli.Get(cityCodeReqTo)) {
        if (responseCodeTo->status == 200) {
            resultCodeTo = json::parse(responseCodeTo->body);
        }
        else {
            std::cout << "Bad request" << std::endl;
        }
    }

    std::string codeTo = resultCodeTo["code"].get<std::string>();


    std::cout << codeTo << std::endl;

    json scheduleResult;


    std::string scheduleReq = "/v3.0/search/?";
    scheduleReq += "apikey=" + YA_API_KEY;
    scheduleReq += "&to=" + codeTo;

    if (auto responseSchedule = cityCodeCli.Get(scheduleReq)) {
        if (responseSchedule->status == 200) {
            scheduleResult = json::parse(responseSchedule->body);
        }
        else {
            std::cout << "Bad request" << std::endl;
        }
    }

    std::cout << scheduleResult;
}