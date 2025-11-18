#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
                      int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm), 
      waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    #ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto& artist : artists) {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
    #endif
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

AudioTrack::~AudioTrack() {
    // TODO: Implement the destructor
    #ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
    #endif
    
    // free allocated memory from the heap that was allocated by constructor
    // and set pointer to null
    delete[] waveform_data;
    waveform_data = nullptr;}

AudioTrack::AudioTrack(const AudioTrack& other)
{
    // TODO: Implement the copy constructor
    #ifdef DEBUG
    std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;
    #endif

    // Shallow copy simple members.
    title = other.title;
    artists = other.artists;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    // Deep Copy: Allocate new memory for waveform_data.
    waveform_data = new double[waveform_size]; 
    
    // Copy content element-by-element from the source to the new block.
    // Prevents double-delete error.
    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = other.waveform_data[i];
    }
}

AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    // TODO: Implement the copy assignment operator
    #ifdef DEBUG
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    #endif

    // Check for self-assignment
    // CRITICAL: Skip cleanup if true to prevent crashing.
    if (this == &other) {
        return *this;
    }

    // Clean up current object's old heap resource.
    // Prevents a memory leak.
    delete[] waveform_data; 

    // Shallow copy simple members.
    title = other.title;
    artists = other.artists;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    // Deep Copy: Allocate new memory for waveform_data.
    waveform_data = new double[waveform_size]; 
    
    // Copy content element-by-element from the source to the new block.
    // Prevents double-delete error.
    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = other.waveform_data[i];
    }    
    
    // Return reference to allow assignment chaining.
    return *this;
}

AudioTrack::AudioTrack(AudioTrack&& other) noexcept {
    #ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
    #endif
    
    // use std::move for containers.
    // This calls the string/vector MOVE ASSIGNMENT operators, avoiding deep copies.
    title = std::move(other.title);
    artists = std::move(other.artists);
    
    // For primitive type shallow copy.
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    // Move ownership: Steal the raw pointer.
    waveform_data = other.waveform_data;

    // Nullify the source pointer.
    // This leaves 'other' in a valid, destructible state (prevents double-delete).
    other.waveform_data = nullptr;
    other.waveform_size = 0; // Set size to 0 for clarity/safety
}

AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {
    // TODO: Implement the move assignment operator

    #ifdef DEBUG
    std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    #endif

    // Check for self-assignment
    // CRITICAL: Skip cleanup if true to prevent crashing.
    if (this == &other) {
        return *this;
    }

    // Clean up current object's old heap resource.
    // Prevents a memory leak.
    delete[] waveform_data; 


    // use std::move for non primitive types.
    // This calls the string/vector MOVE ASSIGNMENT operators, avoiding deep copies.
    title = std::move(other.title);
    artists = std::move(other.artists);
    
    // For primitive type shallow copy.
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    // Move ownership: Steal the raw pointer.
    waveform_data = other.waveform_data;

    // Nullify the source pointer.
    // This leaves 'other' in a valid, destructible state (prevents double-delete).
    other.waveform_data = nullptr;
    other.waveform_size = 0; // Set size to 0 for clarity/safety
    return *this;
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}