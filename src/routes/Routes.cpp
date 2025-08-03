#include "Routes.h"

void Routes::init(AsyncWebServer& server) {
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send(200, "text/plain", "ok");
  });
}
