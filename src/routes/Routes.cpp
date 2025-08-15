#include "Routes.h"
#include <ArduinoJson.h>

#include "AudioMixer.h"
#include "AudioPlayer.h"

void Routes::init(AsyncWebServer &server, AudioMixer *mixer, AudioPlayer *player)
{
    // Cambiar volumen de un canal
    server.on("/volume", HTTP_POST, [mixer, player](AsyncWebServerRequest *request)
              {
        if (!request->hasParam("channel", true) || !request->hasParam("value", true)) {
            request->send(400, "application/json", "{\"error\":\"Missing channel or value\"}");
            return;
        }

        int channel = request->getParam("channel", true)->value().toInt();
        float value = request->getParam("value", true)->value().toFloat();

        if (channel < 0 || channel >= 4) {
            request->send(400, "application/json", "{\"error\":\"Invalid channel\"}");
            return;
        }

        if (!mixer) {
            request->send(500, "application/json", "{\"error\":\"Mixer not initialized\"}");
            return;
        }

        // Clamp
        if (value < 0.0f) value = 0.0f;
        if (value > 100.0f) value = 100.0f;

        mixer->getChannel(channel).setGain(value / 100.0f);

        request->send(200, "application/json", "{\"status\":\"volume set\"}"); });

    // Mute / Unmute un canal
    server.on("/muteChannel", HTTP_POST, [mixer, player](AsyncWebServerRequest *request)
              {
        if (!request->hasParam("channel", true) || !request->hasParam("mute", true)) {
            request->send(400, "application/json", "{\"error\":\"Missing channel or mute\"}");
            return;
        }

        int channel = request->getParam("channel", true)->value().toInt();
        bool mute = request->getParam("mute", true)->value() == "true";

        if (channel < 0 || channel >= 4) {
            request->send(400, "application/json", "{\"error\":\"Invalid channel\"}");
            return;
        }

        mixer->getChannel(channel).setMuted(mute);

        request->send(200, "application/json", "{\"status\":\"channel mute updated\"}"); });

    server.on("/stop",HTTP_POST,[mixer,player](AsyncWebServerRequest *request){

    });

    // Mute / Unmute parlante
    server.on("/pause", HTTP_POST, [mixer, player](AsyncWebServerRequest *request)
              {
        if (!request->hasParam("mute", true)) {
            request->send(400, "application/json", "{\"error\":\"Missing mute\"}");
            return;
        }

        bool m = request->getParam("mute", true)->value() == "true";

        player->mute(m);

        request->send(200, "application/json", "{\"status\":\"speaker mute updated\"}"); });

    // Estado de todos los canales
    server.on("/speakerStatus", HTTP_GET, [mixer, player](AsyncWebServerRequest *request) {
        DynamicJsonDocument doc(512);
        JsonArray arr = doc.createNestedArray("channels");
        for (int i = 0; i < 4; i++) {
            MixerChannel& ch = mixer->getChannel(i);
            JsonObject obj = arr.createNestedObject();
            obj["active"] = ch.isActive();
            obj["gain"] = ch.getGain();
            obj["muted"] = ch.isMuted();
        }
        doc["speakerMuted"] = player->isMuted(); // Probablemente querías esto en lugar de getVolume()
        doc["speakerVolume"] = player->getVolume(); // Esto debería ser el volumen del speaker
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });
}
