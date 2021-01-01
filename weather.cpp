#include "weather.hpp"


WeatherHttp::Weather::Weather(std::string City)
    
{
    city = City;
    api.setPrivateKey ("SHvxFEcpUHpihET3l");
    api.setLocation (City);
    api.setLanguage (WeatherHttpApi::zhHans);
    api.setUnit ();
    api.setUrl ();
}

void WeatherHttp::Weather::setCity (const std::string &City)
{
    
    city = City;
    api.setLocation (City);
    api.setUrl ();

}

void WeatherHttp::Weather::getWeather (bool debug)
{
    if (api.Get (debug) == CURLE_OK)
    {
        
        if (!parseJson())
            std::cerr << "parse data to json failed." << std::endl;
        else
            api.clearData();
    }
    else 
        api.error();
    
}

bool WeatherHttp::Weather::parseJson (void)
{
    document.Parse(api.getData(), api.getDataLenth ());
    assert(document.IsObject());
    assert(document.HasMember("results"));
    assert(document["results"].IsArray());

    const rapidjson::Value &results = document["results"];
    assert(results[0].HasMember("location"));
    assert(results[0]["location"].IsObject());
    const rapidjson::Value &locationObj = results[0]["location"];


    assert(locationObj.HasMember("name"));
    assert(locationObj["name"].IsString());
    city = locationObj["name"].GetString();

    assert (locationObj.HasMember("country"));
    assert(locationObj["country"].IsString());
    country = locationObj["country"].GetString();

    assert(results[0].HasMember("now"));
    assert(results[0]["now"].IsObject());
    const rapidjson::Value &now = results[0]["now"];

    assert(now.HasMember("text"));
    assert(now["text"].IsString());
    weather = now["text"].GetString();

    assert(now.HasMember("temperature"));
    assert(now["temperature"].IsString());
    temp = now["temperature"].GetString();
    

    assert(results[0].HasMember("last_update"));
    assert(results[0]["last_update"].IsString());
    lastUpdate = results[0]["last_update"].GetString();

    return true;
}