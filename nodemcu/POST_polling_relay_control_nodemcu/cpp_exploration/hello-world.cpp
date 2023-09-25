#include <iostream>
using namespace std;

#include "./.env"

#define WIFI_SSID ENV_WIFI_SSID
#define WIFI_PASSWORD ENV_WIFI_PASSWORD

int main()
{
    cout << "hello, world" << endl
         << WIFI_SSID << endl
         << WIFI_PASSWORD
         << endl;
    return 0;
}
