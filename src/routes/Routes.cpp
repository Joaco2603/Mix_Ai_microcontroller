#include "Routes.h"
#include <ArduinoJson.h>
#include "AudioMixer.h"

extern AudioMixer* mixer;

void Routes::init(AsyncWebServer& server) {
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send(200, "text/plain", "ok");
  });

  // Endpoint that change volumes
  // server.on("/volume", HTTP_POST, [](AsyncWebServerRequest* request){
  //   if (!request->hasParam("channel", true) || !request->hasParam("value", true)) {
  //     request->send(400, "application/json", "{\"error\":\"Missing channel or value\"}");
  //     return;
  //   }
  //   int channel = request->getParam("channel", true)->value().toInt();
  //   int value = request->getParam("value", true)->value().toInt();

  //   if (channel < 0 || channel >= MAX_CHANNELS) {
  //     request->send(400, "application/json", "{\"error\":\"Invalid channel\"}");
  //     return;
  //   }

  //   mixer->setChannelGain(channel, value / 100.0f);

  //   request->send(200, "application/json", "{\"status\":\"volume set\"}");
  // });

  // // Endpoint that mutes the channel
  // server.on("/mute", HTTP_POST, [](AsyncWebServerRequest* request){
  //   if (!request->hasParam("channel", true)) {
  //     request->send(400, "application/json", "{\"error\":\"Missing channel\"}");
  //     return;
  //   }
  //   int channel = request->getParam("channel", true)->value().toInt();

  //   if (channel < 0 || channel >= AudioMixer::MAX_CHANNELS) {
  //     request->send(400, "application/json", "{\"error\":\"Invalid channel\"}");
  //     return;
  //   }

  //   mixer->setChannelGain(channel, 0.0f); // Mute the channel by setting gain to 0.0f

  //   request->send(200, "application/json", "{\"status\":\"channel muted\"}");
  // });
}

