#include <cstdio>

#include "alsa_output.h"
#include "audio_pipeline.h"
#include "ui/ui.h"

using namespace keebird;

static const uint32_t SAMPLE_RATE = 44100;
static const uint32_t CONTROL_RATE = SAMPLE_RATE / 10;
static const uint8_t CHANNEL_COUNT = 2;
static const size_t FRAMES_PER_PERIOD = 128;
static const uint16_t DEFAULT_BPM = 120;
static const uint8_t DEFAULT_DOWNBEAT = 4;
static constexpr uint16_t DISPLAY_WIDTH = 160;
static constexpr uint16_t DISPLAY_HEIGHT = 128;

int main(int argc, char** argv) {
    int rc;
    int16_t sample_buffer[FRAMES_PER_PERIOD*CHANNEL_COUNT];
    Ui ui(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    AudioPipeline pipeline(SAMPLE_RATE, CONTROL_RATE);
    AlsaOutput sound_out(SAMPLE_RATE);

    rc = sound_out.Start();
    if (rc < 0) {
        printf("Failed to open sound playback device!\n");
        return rc;
    }

    rc = ui.Start();
    if (rc < 0) {
        printf("Failed to start UI!\n");
        return rc;
    }

    uint32_t seconds = 5;
    uint32_t total_samples = SAMPLE_RATE * seconds;
    for (uint32_t current_samples = 0;
         current_samples < total_samples;
         current_samples += FRAMES_PER_PERIOD) {
        pipeline.Fill(sample_buffer, FRAMES_PER_PERIOD, CHANNEL_COUNT);
        sound_out.Write(sample_buffer, FRAMES_PER_PERIOD);
    }

    sound_out.Stop();
    
    return 0;
}

