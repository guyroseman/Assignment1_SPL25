#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // (a) check if track with the same title is already in cache
    if(cache.contains(track.get_title())){
        // (b) HIT
        cache.get(track.get_title()); // update LRU order
        return 1;
    }

    // (c) MISS - track not found 
    // Create a polymorphic clone of the track
    PointerWrapper<AudioTrack> track_clone = track.clone();
    
    // Check if pointer is null
    if(!track_clone){
        throw std::runtime_error("Attempted to dereference a null PointerWrapper.");
    }
    
    // Simulate loading on the cloned track, and do a beatgrid analysis.
    AudioTrack* clone_ptr = track_clone.get();
    clone_ptr->load();
    clone_ptr->analyze_beatgrid();

    //Wrap the prepared clone in a new PointerWrapper
    PointerWrapper<AudioTrack> wrapped_clone(clone_ptr);

    // Insert into cache and move ownership into cache
    bool eviction = cache.put(std::move(wrapped_clone));

    if(eviction){
        // (d) MISS with eviction
        return -1;
    }
    // (e) MISS without eviction
    return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Ownership remains with the LRUCache, fulfilling the requirement.
    return cache.get(track_title);
}
