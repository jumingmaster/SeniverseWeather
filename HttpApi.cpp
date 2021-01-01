#include "HttpApi.hpp"

size_t getCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
        size_t realSize = size * nmemb;
        WeatherHttp::DataPacket *userdata = (WeatherHttp::DataPacket*)data;
        
        userdata->data = (char *)realloc (userdata->data, userdata->lenth + realSize);
        

        if (userdata->data)
        {
                memcpy (&userdata->data[userdata->lenth], ptr, realSize);
                userdata->lenth += realSize;
        }
        else
                perror ("relloc error");

        return realSize;
}

WeatherHttp::WeatherHttpApi::WeatherHttpApi ()
{
        replyData->data = (char *)malloc (1);
        replyData->lenth = 0;

        rCode = curl_global_init(CURL_GLOBAL_SSL);
        if (rCode != CURLE_OK)
        {
                std::cout << "curl init failed: ";
                error ();
                exit (EXIT_FAILURE);
        }
        curl_pointer = curl_easy_init ();
        if (!curl_pointer)
        {
                std::cout << "curl easy_init failed: ";
                error ();
                exit (EXIT_FAILURE);
        }

        curl_easy_setopt(curl_pointer, CURLOPT_TIMEOUT, 3L);//请求超时时长
        curl_easy_setopt(curl_pointer, CURLOPT_CONNECTTIMEOUT, 10L); //连接超时时长 
        curl_easy_setopt(curl_pointer, CURLOPT_FOLLOWLOCATION, 1L);//允许重定向
        curl_easy_setopt(curl_pointer, CURLOPT_HEADER, 0L);  //若启用，会将头文件的信息作为数据流输出
        curl_easy_setopt(curl_pointer, CURLOPT_WRITEDATA, replyData);
        curl_easy_setopt(curl_pointer, CURLOPT_WRITEFUNCTION, getCallback);  //得到请求结果后的回调函数
        curl_easy_setopt(curl_pointer, CURLOPT_NOSIGNAL, 1L); //关闭中断信号响应
        curl_easy_setopt(curl_pointer, CURLOPT_VERBOSE, 0L); //启用时会汇报所有的信息
        curl_easy_setopt(curl_pointer, CURLOPT_URL, url.c_str() ); //需要获取的URL地址
        
        
        
}

WeatherHttp::WeatherHttpApi::~WeatherHttpApi ()
{
        free (replyData->data);
        curl_easy_cleanup (curl_pointer);
        curl_global_cleanup ();
}

CURLcode WeatherHttp::WeatherHttpApi::Get (bool debug)
{
        if (debug)
                curl_easy_setopt(curl_pointer, CURLOPT_VERBOSE, 1L); //启用时会汇报所有的信息
        else
                curl_easy_setopt(curl_pointer, CURLOPT_VERBOSE, 0L); //启用时会汇报所有的信息
        
        if (url.empty())
        {
                std::cerr << "Invaild url." << std::endl;
                return CURLE_LDAP_INVALID_URL;
        }

        rCode = curl_easy_perform(curl_pointer);
        return rCode;
}


void WeatherHttp::WeatherHttpApi::setUrl (const std::string &__url)
{
        if (__url.empty())
        {
                url.clear();
                url = API_Address;   
                if (!privateKey.empty())
                {
                           
                        url.append ("key=").append(privateKey);
                }

                if (!location.empty())
                        url.append(connector).append ("location=").append(location);
                else 
                        url.append(connector).append ("location=").append("beijing");

                if (!language.empty())
                        url.append(connector).append ("language=").append(language);
                else
                        url.append(connector).append ("location=").append(languages[zhHans]);

                switch (unit)
                {
                case 'c':
                        url.append(connector).append ("unit=c");
                        break;
                case 'f':
                        url.append(connector).append ("unit=f");
                        break;
                default:
                        url.append(connector).append ("unit=").append("c");
                }
        }

        else
                url = __url;
        curl_easy_setopt (curl_pointer, CURLOPT_URL, url.c_str());    
        
}