#include "Routes.h"
#include <ArduinoJson.h>
#include "AudioPlayer.h"

extern AudioPlayer audioPlayer;

void Routes::init(AsyncWebServer& server) {
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send(200, "text/plain", "ok");
  });

  // Endpoint para regular el volumen por canal
  server.on("/volume", HTTP_POST, [](AsyncWebServerRequest* request){
    if (!request->hasParam("channel", true) || !request->hasParam("value", true)) {
      request->send(400, "application/json", "{\"error\":\"Missing channel or value\"}");
      return;
    }
    int channel = request->getParam("channel", true)->value().toInt();
    int value = request->getParam("value", true)->value().toInt();

    // Aquí deberías implementar la lógica para ajustar el volumen por canal
    // Por ejemplo: audioPlayer.setChannelVolume(channel, value);

    request->send(200, "application/json", "{\"status\":\"volume set\"}");
  });

  // Endpoint para mutear por canal
  server.on("/mute", HTTP_POST, [](AsyncWebServerRequest* request){
    if (!request->hasParam("channel", true)) {
      request->send(400, "application/json", "{\"error\":\"Missing channel\"}");
      return;
    }
    int channel = request->getParam("channel", true)->value().toInt();

    // Aquí deberías implementar la lógica para mutear el canal
    // Por ejemplo: audioPlayer.muteChannel(channel);

    request->send(200, "application/json", "{\"status\":\"channel muted\"}");
  });
}
