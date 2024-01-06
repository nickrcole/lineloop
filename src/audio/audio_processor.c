/**
 * @file audio_processor.c
 * @date 1/2/24
 * @brief Responsible for performing Fast Fourier Transforms (FFTs) on audio buffer to split into frequency bands
 * @author Nicholas Cole https://nicholascole.dev
 */


#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "./../../include/render_toolkit.h"
#include "./../../include/audio.h"

#define SAMPLE_RATE          44100
#define SPECTRUM_PERCENTAGE   0.15
double* bands;
BarData* bars;
int NUM_BANDS;

void attach_bar_components(BarData* bars_buf) {
    bars = bars_buf;
}

float normalize(double* temp_buf) {
    float max = temp_buf[0];
    for (int i = 1; i < NUM_BARS; i++) {
        if (temp_buf[i] > max) {
            max = temp_buf[i];
        }
    }

    for (int i = 0; i < NUM_BARS; i++) {
        temp_buf[i] = temp_buf[i] / max;
    }

    return max;
}

// Only about the first 15% of the spectrum is relevant
void splitIntoBands(fftw_complex* fftResult, int numBands) {
    // Calculate the size of the relevant spectrum
    int relevantSize = FRAMES_PER_BUFFER * SPECTRUM_PERCENTAGE;

    // Calculate the size of each band in the relevant spectrum
    int bandSize = relevantSize / numBands;

    // Process each band in the relevant spectrum
    double average = 0.0;
    for (int band = 0; band < numBands; band++) {
        double bandIntensity = 0.0;

        // Calculate the intensity in the current band of the relevant spectrum
        for (int i = band * bandSize; i < (band + 1) * bandSize; i++) {
            double magnitude = sqrt(fftResult[i][0] * fftResult[i][0] + fftResult[i][1] * fftResult[i][1]);
            bandIntensity += magnitude;
        }

        // Normalize the intensity and print the result
        bandIntensity /= bandSize;
        average += bandIntensity;
        bands[band] = bandIntensity * 100;
    }
    double* temp_buf = malloc(sizeof(double) * NUM_BARS);
    memcpy(temp_buf, bands, sizeof(double) * NUM_BARS);
    normalize(temp_buf);
    for (int band = 0; band < numBands; band++) {
        bars[band].last_height = bars[band].height;
        bars[band].height = 14 * temp_buf[band];
    }
}

void fft(SAMPLE* fft_buf) {
    fftw_complex* in;
    fftw_complex* out;
    fftw_plan plan;

    in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * FRAMES_PER_BUFFER);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * FRAMES_PER_BUFFER);
    plan = fftw_plan_dft_1d(FRAMES_PER_BUFFER, in, out, FFTW_FORWARD, FFTW_MEASURE);

    // Copy the audio buffer to the input array for FFT
    for (int i = 0; i < FRAMES_PER_BUFFER; i++) {
        in[i][0] = fft_buf[i];
        in[i][1] = 0.0;
    }

    // Execute the FFT
    fftw_execute(plan);

    // Split the result into frequency bands
    splitIntoBands(out, NUM_BANDS);

    // Clean up
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
}

void audio_processor_init(double* bands_buf, int barNum) {
    bands = bands_buf;
    NUM_BANDS = barNum;
}