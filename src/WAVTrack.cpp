#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    
std::cout << "[WAVTrack::load] Loading WAV: \"" << title 
              << "\" at " << sample_rate << "Hz/" << bit_depth << "bit (uncompressed)...\n";

long long file_size = (long long)duration_seconds * sample_rate * (bit_depth / 8) * 2;

std::cout << "  → Estimated file size: " << file_size << " bytes\n";
std::cout << "  → Fast loading due to uncompressed format.\n";
}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement WAV-specific beat detection analysis
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    // 3. Print number of beats and mention uncompressed precision
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"

    double beats_estimated = (static_cast<double>(duration_seconds) / 60.0) * bpm;
    
    std::cout << "  → Estimated beats: " << int(beats_estimated) 
              << " → Precision factor: 1 (uncompressed audio)" << std::endl;
}

double WAVTrack::get_quality_score() const {
    // TODO: Implement WAV quality scoring
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    double score = 70;

    if (sample_rate >= 44100){
        score += 10;
    }

    if (sample_rate >= 96000){
        score += 5;
    }

    if (bit_depth >= 16){
        score += 10;
    }

    if (bit_depth >= 24){
        score += 5;
    }
    
    // No need to check score > 100 as it cannot reach any number out of the range of 70-100

    return score; 
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // Creates a new WAVTrack object using the WAVTrack Copy Constructor.
    // The base class AudioTrack's deep copy logic ensures waveform_data is copied.
    return PointerWrapper<AudioTrack>(new WAVTrack(*this));
}