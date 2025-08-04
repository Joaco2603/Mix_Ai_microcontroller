// AudioPlayer.cpp
#include <M5GFX.h>
#include "AudioPlayer.h"
#include <driver/i2s.h>
// #include <driver/i2s_pdm.h>

// Configuración de pines I2S para M5Stack
#define I2S_BCLK 12
#define I2S_LRC 0
#define I2S_DOUT 2

extern M5GFX display;

AudioPlayer::AudioPlayer()
{
    audio = nullptr;
    isPlaying = false;
    currentVolume = 15;
    currentFile = "";
}

AudioPlayer::~AudioPlayer()
{
    if (audio)
    {
        delete audio;
    }
}

bool AudioPlayer::begin()
{
    // i2s_config_t i2s_config = {
    //     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_PDM),
    //     // .sample_rate = 44100,
    //     .sample_rate = 16000,
    //     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    //     .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT, // mono
    //     .communication_format = I2S_COMM_FORMAT_I2S,
    //     // .intr_alloc_flags = 0,
    //     // .dma_buf_count = 8,
    //     // .dma_buf_len = 512,
    //     // .use_apll = false,
    //     // .tx_desc_auto_clear = true,
    //     // .fixed_mclk = 0
    //     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    //     .dma_buf_count = 2,
    //     .dma_buf_len = 1024
    // };

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = 64000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 1024,
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = 12,
        .ws_io_num = 0,
        .data_out_num = 2,
        .data_in_num = I2S_PIN_NO_CHANGE};

    // Instalar I2S
    if (i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL) != ESP_OK)
    {
        Serial.println("Error instalando I2S driver");
        return false;
    }

    // Asignar pines
    if (i2s_set_pin(I2S_NUM_0, &pin_config) != ESP_OK)
    {
        Serial.println("Error configurando pines I2S");
        return false;
    }

    // i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);

    Serial.println("AudioPlayer inicializado correctamente");
    return true;
}

bool AudioPlayer::playMixedFiles(const char *f1, const char *f2, const char *f3)
{
    if (!mixer)
    {
        Serial.println("[Error] mixer no inicializado");
        return false;
    }

    mixer->begin();
    mixer->addTrack(f1, 0.7f);
    mixer->addTrack(f2, 0.3f);
    mixer->addTrack(f3, 0.3f);
    isPlaying = true;
    return true;
}

void AudioPlayer::setMixer(AudioMixer *m)
{
    mixer = m;
}

bool AudioPlayer::playFile(const char *filename)
{
    if (!audio)
    {
        display.println("Error: Audio no inicializado");
        return false;
    }

    if (!SD.exists(filename))
    {
        File root = SD.open("/");
        File file = root.openNextFile();
        while (file)
        {
            Serial.print("Archivo: ");
            Serial.println(file.name());
            file = root.openNextFile();
        }
        return false;
    }

    // Verificar si el archivo existe
    if (!SD.exists(filename))
    {
        display.println("Error: Archivo no encontrado\n");
        return false;
    }

    // Reproducir archivo
    audio->connecttoFS(SD, filename); // Pass SD by reference, not pointer
    isPlaying = true;
    currentFile = String(filename);
    Serial.printf("Reproduciendo: %s\n", filename);

    return true;
}

void AudioPlayer::stop()
{
    // if (audio)
    // {
    //     audio->stopSong();
    //     isPlaying = false;
    //     currentFile = "";
    //     Serial.println("Audio detenido");
    // }
}

void AudioPlayer::pause()
{
    // if (audio && isPlaying)
    // {
    //     audio->pauseResume();
    //     isPlaying = false;
    //     Serial.println("Audio pausado");
    // }
}

void AudioPlayer::resume()
{
    // if (audio && !isPlaying && currentFile != "")
    // {
    //     audio->pauseResume();
    //     isPlaying = true;
    //     Serial.println("Audio reanudado");
    // }
}

void AudioPlayer::togglePlayPause()
{
    if (isPlaying)
    {
        pause();
    }
    else
    {
        resume();
    }
}

void AudioPlayer::setVolume(int volume)
{
    if (volume < 0)
        volume = 0;
    if (volume > 21)
        volume = 21;

    currentVolume = volume;
    if (audio)
    {
        audio->setVolume(currentVolume);
        Serial.printf("Volumen: %d\n", currentVolume);
    }
}

void AudioPlayer::volumeUp()
{
    setVolume(currentVolume + 1);
}

void AudioPlayer::volumeDown()
{
    setVolume(currentVolume - 1);
}

int AudioPlayer::getVolume()
{
    return currentVolume;
}

bool AudioPlayer::isCurrentlyPlaying()
{
    // return isPlaying && audio && audio->isRunning();
    return true;
}

String AudioPlayer::getCurrentFile()
{
    return currentFile;
}

void AudioPlayer::update()
{
    if (!isPlaying || !mixer)
    {
        isPlaying = false;
        return;
    }

    int16_t buffer[512];
    size_t n = mixer->mix(buffer, 512);

    if (n > 0)
    {
        writeToI2S(buffer, n);
    }
    else
    {
        display.println("Fin de reproduccion");
        isPlaying = false;
    }
}

// void AudioPlayer::writeToI2S(int16_t *buffer, size_t samples)
// {
//     int16_t stereoBuffer[1024];
//     for (size_t i = 0; i < samples; ++i)
//     {
//         stereoBuffer[2 * i] = buffer[i];
//         stereoBuffer[2 * i + 1] = buffer[i];
//     }

//     size_t bytes_written;
//     esp_err_t err = i2s_write(I2S_NUM_0, stereoBuffer, samples * 2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);
//     if (err != ESP_OK) {
//         Serial.printf("Error en i2s_write: %d\n", err);
//     }
// }

// void AudioPlayer::writeToI2S(int16_t *buffer, size_t samples)
// {
//     // DEBUG: Verificar si el buffer tiene audio
//     bool hasAudio = false;
//     for (size_t i = 0; i < samples && !hasAudio; ++i) {
//         if (buffer[i] != 0) hasAudio = true;
//     }

//     int16_t stereoBuffer[1024];
//     for (size_t i = 0; i < samples; ++i)
//     {
//         stereoBuffer[2 * i] = buffer[i];
//         stereoBuffer[2 * i + 1] = buffer[i];
//     }

//     size_t bytes_written;
//     esp_err_t err = i2s_write(I2S_NUM_0, stereoBuffer, samples * 2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);

// }

void AudioPlayer::writeToI2S(int16_t *buffer, size_t samples)
{
    int16_t stereoBuffer[1024];
    for (size_t i = 0; i < samples; ++i)
    {
        stereoBuffer[2 * i] = buffer[i];     // Canal izquierdo
        stereoBuffer[2 * i + 1] = buffer[i]; // Canal derecho
    }

    size_t bytes_written;
    esp_err_t err = i2s_write(I2S_NUM_0, stereoBuffer, samples * 2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);
    if (err != ESP_OK)
    {
        Serial.printf("Error en i2s_write: %d\n", err);
    }
}
