#pragma once
#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <iostream>
#include <array>
#include <stdlib.h>
#include <string.h>




namespace WeatherHttp
{


struct DataPacket
{
        char *data;
        size_t lenth;
};

class WeatherHttpApi
{
public:
        WeatherHttpApi ();
        virtual ~WeatherHttpApi ();
       
        enum Language {
                zhHans, zhHant, en, ja, de, fr, 
                es, pt, hi, id, ru, th, ar
                };
        
        
        std::string API_Address = "https://api.seniverse.com/v3/weather/now.json?";
        const std::string connector = "&";
        
        void setUrl (const std::string &__url = "");
        void setUnit (const char __unit = 'c') { unit = __unit; }
        void setLanguage (Language locale = zhHans) { language = languages[locale]; }
        void setLocation (const std::string &local) { location = local; }
        void setPrivateKey (const std::string &key="") { privateKey = key; }
        void error () const { std::cerr << curl_easy_strerror (rCode) << std::endl; }
        CURLcode Get (bool debug);
        char * getData () const {  return replyData->data; }
        size_t getDataLenth () const { return replyData->lenth; }
        void clearData (void) { memset (replyData->data, 0, replyData->lenth); replyData->lenth = 0; }

private:
        CURL *curl_pointer;
        CURLcode rCode;
        std::string privateKey;
        std::string location;
        std::string language;
        std::string url;
        char unit;
        DataPacket *replyData;
        

protected:
        const std::array<const char *, 13>languages = {
                "zh-Hans",
                "zh-Hant",
                "en",
                "ja",
                "de",
                "fr",
                "es",
                "pt",
                "hi",
                "id",
                "ru",
                "th",
                "ar"
        };

        

};

}
