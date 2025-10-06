#include <iostream>
#include <vector>
#include <cmath>
#include <sndfile.h>
#include <fftw3.h>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iomanip>

int next_power_of_2(int n) {
    return pow(2, ceil(log2(n)));
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_wav_file>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    SF_INFO sfinfo;
    SNDFILE *infile = sf_open(filename, SFM_READ, &sfinfo);

    if (!infile) {
        std::cerr << "libsndfile error: " << sf_strerror(NULL) << std::endl;
        return 1;
    }


    // We want to measure every 100ms, a sample rate is the how many samples are per second, to get the reference every 100ms
    const double CHUNK_DURATION_MS = 100.0;
    const int CHUNK_SAMPLES = static_cast<int>(sfinfo.samplerate * (CHUNK_DURATION_MS / 1000.0));

    // The FFT is the most efficient in power 2. Since we only take part of the samples (e.g. 4410) we find the nearest ceiling and fill the left over with zeros (Zero padding) 
    const int FFT_SIZE = next_power_of_2(CHUNK_SAMPLES);

    // We print the info we have of the sample so far
    const int CHANNELS = sfinfo.channels;
    std::cout << "File: " << filename << std::endl;
    std::cout << "Sample Rate: " << sfinfo.samplerate << " Hz" << std::endl;
    std::cout << "Channels: " << CHANNELS << std::endl;
    std::cout << "Analyzing in " << CHUNK_DURATION_MS << "ms chunks (" << CHUNK_SAMPLES << " samples per chunk)" << std::endl;
    std::cout << "FFT Size: " << FFT_SIZE << std::endl;
    std::cout << "------------------------------------------" << std::endl;


    // temporary buffer for libsndfile to place raw audio data. It is multiplying channels to account for e.g. stereo
    std::vector<double> audio_chunk(CHUNK_SAMPLES * CHANNELS);

    // Allocate memory similair to malloc in C, however more efficient. This contain size of samples with zero padding
    double* fft_in = (double*) fftw_malloc(sizeof(double) * FFT_SIZE);

    // The output of an FFT on real-world data (like audio) is a series of complex numbers. Complex numbers contain magnitude and phase.
    // The list will turn perfectly symmetrical, due fft requires things to be periodic, therefore secondhalf is redundant. +1 is need to capture the first sample. 
    fftw_complex* fft_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (FFT_SIZE / 2 + 1));

    // Plans ahead to find the most efficient computation path for real-to-complex calculation, making it more effiecient in the loop
    fftw_plan plan = fftw_plan_dft_r2c_1d(FFT_SIZE, fft_in, fft_out, FFTW_ESTIMATE);

    // Since we cannot always assure that the window with the samples is periodic, we need to prevent leakage. 
    // This is done in this case with Hann Window. Be forcing the ends and start to be multiplied with 0, making it periodic.
    std::vector<double> hann_window(CHUNK_SAMPLES);
    for (int i = 0; i < CHUNK_SAMPLES; ++i) {
        hann_window[i] = 0.5 * (1 - cos(2 * M_PI * i / (CHUNK_SAMPLES - 1)));
    }

    const int NUM_BINS = FFT_SIZE / 2 + 1;
    
    // Stores the magnitude (energy) of each bin from the PREVIOUS FFT chunk.
    std::vector<double> previous_magnitudes(NUM_BINS, 0.0); 
    
    // A simple threshold: a large jump in flux suggests an onset (beat).
    // This value will need manual tuning for different files.
    const double BEAT_THRESHOLD = 500.0; 
    
    // This stores the magnitudes for the CURRENT frame.
    std::vector<double> current_magnitudes(NUM_BINS);

    sf_count_t frames_read;
    double current_time = 0.0;

    std::cout << "Starting Spectral Flux Analysis..." << std::endl;
    std::cout << "Beat Detection Threshold: " << BEAT_THRESHOLD << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    while ((frames_read = sf_readf_double(infile, audio_chunk.data(), CHUNK_SAMPLES)) > 0) {
        // Zero out the FFT input buffer
        for (int i = 0; i < FFT_SIZE; ++i) {
            fft_in[i] = 0.0;
        }

        for (int i = 0; i < frames_read; ++i) {
            // Mix to mono by averaging channels
            double mono_sample = 0.0;
            for (int j = 0; j < CHANNELS; ++j) {
                mono_sample += audio_chunk[i * CHANNELS + j];
            }
            mono_sample /= CHANNELS;

            // Apply the Hann window
            fft_in[i] = mono_sample * hann_window[i];
        }

        fftw_execute(plan);

        double spectral_flux = 0.0;

        // Iterate through the frequency bins (from 0 up to Nyquist)
        for (int i = 0; i < NUM_BINS; ++i) {
            double real = fft_out[i][0];
            double imag = fft_out[i][1];
            double magnitude = std::sqrt(real * real + imag * imag);
            
            current_magnitudes[i] = magnitude;

            // Spectral Flux is the sum of only POSITIVE changes in magnitude.
            // This favors increases in volume (onsets) and ignores decreases (decays).
            double diff = magnitude - previous_magnitudes[i];
            spectral_flux += std::max(0.0, diff);
        }

        // ------------------------------------------
        // BEAT IDENTIFICATION
        // ------------------------------------------
        if (spectral_flux > BEAT_THRESHOLD) {
            std::cout << "Time: " << std::fixed << std::setprecision(3) << current_time 
                      << "s | *** BEAT DETECTED *** | Flux: " 
                      << std::fixed << std::setprecision(1) << spectral_flux << std::endl;
        } else {
             // For debugging/analysis, you can print the flux even if no beat is detected:
             std::cout << "Time: " << current_time << "s | Flux: " << spectral_flux << std::endl;
        }


        // IMPORTANT: Update the previous magnitudes for the next iteration
        previous_magnitudes = current_magnitudes;

        current_time += (CHUNK_DURATION_MS / 1000.0);
    }

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "End of file reached." << std::endl;

    // Free up memory
    fftw_destroy_plan(plan);
    fftw_free(fft_in);
    fftw_free(fft_out);
    sf_close(infile);

    return 0;
}