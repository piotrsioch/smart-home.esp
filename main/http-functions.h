#ifndef HTTP_FUNCTIONS_H
#define HTTP_FUNCTIONS_H

#include <HTTPClient.h>
#include <string>

void initializeWifi();
void sendRequest(const std::string& path, const std::string& data);
void sendRequestToEndpoint(const std::string& endpointName, const std::string& data);
void getAndSetLightState();

#endif