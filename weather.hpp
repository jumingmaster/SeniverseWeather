#include "HttpApi.hpp"
#include <rapidjson/document.h>

namespace WeatherHttp
{
class Weather;
};

class WeatherHttp::Weather
{
        
public:
        Weather(std::string City = "beijing");
        ~Weather () = default;
        void setCity (const std::string &City);
        void getWeather (bool debug = false);
        
        void showWeather (void)
        {
                std::cout 
                        << "Country    : " << country << std::endl
                        << "City       : " << city << std::endl
                        << "Temperature: " << temp + "℃" << std::endl
                        << "Weather    : " << weather << std::endl
                        << "Last Update: " << lastUpdate << std::endl;
        }

private:
        std::string country;
        std::string city;
        std::string temp;
        std::string weather;
        std::string lastUpdate;
        WeatherHttpApi api;
        rapidjson::Document document;

        bool parseJson (void);
};
