#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <iostream>
#include "weather.hpp"

using namespace std;
using namespace WeatherHttp;

int main(void)
{
        Weather foshan ("foshan");
        foshan.getWeather ();
        foshan.showWeather ();
}
