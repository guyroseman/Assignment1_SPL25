#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string& track_id) const {
    return findSlot(track_id) != max_size;
}

AudioTrack* LRUCache::get(const std::string& track_id) {
    size_t idx = findSlot(track_id);
    if (idx == max_size) return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * TODO: Implement the put() method for LRUCache
 */
bool LRUCache::put(PointerWrapper<AudioTrack> track) {
    // (a) Handle nullptr track
    if (!track) {
        return false;
    }

    // Use the track title as the identifier
    const std::string track_id = track->get_title();

    // (b) If track already exists, update its access time and return false
    size_t existing = findSlot(track_id);
    if (existing != max_size) {
        slots[existing].access(++access_counter);
        return false;
    }


    bool eviction_occurred = false;
    // (c) If cache is full, evict LRU first
    if (isFull()) {
        eviction_occurred = evictLRU();
    }

    // (d) Find an empty slot
    size_t slot_idx = findEmptySlot();
    
    // recheck if eviction occured
    if (slot_idx == max_size) {
        // No empty slot available even after eviction
        return eviction_occurred;
    }

    // (e) Store the new track with the current access_counter value and mark the slot as occupied
    // We use std::move(track) to transfer ownership from the parameter to the CacheSlot.
    slots[slot_idx].store(std::move(track), ++access_counter);

    // (f) Return whether an eviction occurred
    return eviction_occurred;
}

bool LRUCache::evictLRU() {
    size_t lru = findLRUSlot();
    if (lru == max_size || !slots[lru].isOccupied()) return false;
    slots[lru].clear();
    return true;
}

size_t LRUCache::size() const {
    size_t count = 0;
    for (const auto& slot : slots) if (slot.isOccupied()) ++count;
    return count;
}

void LRUCache::clear() {
    for (auto& slot : slots) {
        slot.clear();
    }
}

void LRUCache::displayStatus() const {
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i) {
        if(slots[i].isOccupied()){
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        } else {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string& track_id) const {
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id) return i;
    }
    return max_size;

}

/**
 * TODO: Implement the findLRUSlot() method for LRUCache
 */
size_t LRUCache::findLRUSlot() const {
    // Initialized to -1 to signify that no occupied slot has been found yet.
    int index=-1;
    uint64_t lowest=0;

    // Iterate through all possible cache slots (up to max_size).
    for(size_t i=0;i<max_size;i++){
        //check if occupied
        if(slots[i].isOccupied()){
            //save the access time of the first occupied slot
            if(index==-1){
                lowest=slots[i].getLastAccessTime();
                index=i;
            }
            // If the current slot's access time is lower (older) 
            // than the current minimum ('lowest'), update the LRU slot index.
            else if(lowest>slots[i].getLastAccessTime()){
                lowest=slots[i].getLastAccessTime();
                index=i;
            }
        }
    }
    //if the slots are all empty returning the max size
    if(index==-1){
        return max_size;
    }
    //return the minimum
    return index;
}

size_t LRUCache::findEmptySlot() const {
    for (size_t i = 0; i < max_size; ++i) {
        if (!slots[i].isOccupied()) return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity){
    if (max_size == capacity)
        return;
    //udpate max size
    max_size = capacity;
    //update the slots vector
    slots.resize(capacity);
}