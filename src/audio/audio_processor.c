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
#include "./../../include/audio.h"

#define SAMPLE_RATE 44100
double* bands;
int NUM_BANDS;

void splitIntoBands(fftw_complex* fftResult, int numBands) {
    // Calculate the size of each band
    int bandSize = FRAMES_PER_BUFFER / numBands;

    // Process each band
    // fputs("\033c", stdout);
    double average = 0.0;
    for (int band = 0; band < numBands; band++) {
        double bandIntensity = 0.0;

        // Calculate the intensity in the current band
        for (int i = band * bandSize; i < (band + 1) * bandSize; i++) {
            double magnitude = sqrt(fftResult[i][0] * fftResult[i][0] + fftResult[i][1] * fftResult[i][1]);
            bandIntensity += magnitude;
        }

        // Normalize the intensity and print the result
        bandIntensity /= bandSize;
        // printf("Band %d intensity: %f\n", band + 1, bandIntensity);

        average += bandIntensity;
        bands[band] = bandIntensity * 100;
        // printf("%f ", bands[band]);
    }
    // printf("\n");
    // average /= numBands;
    // for (int band = 0; band < numBands; band++) {
    //   if (bands[band] < (2 * average)) {
    //     bands[band] *= 20;
    //   }
    //   double scaled_intensity = bands[band] * 50.0;  // Adjust the scale factor as needed
    //   int num_hashes = (int)scaled_intensity;
    //     if (num_hashes > 50) {
    //         num_hashes = 50;
    //     }
    //     printf("%f ", scaled_intensity);

    //     // Print the "#" characters
    //     // for (int i = 0; i < num_hashes; i++) {
    //     //     printf("#");
    //     // }

    //     // printf("\n");
    // }
    // printf("\n");
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