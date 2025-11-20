#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    auto_sync = false;
    bpm_tolerance = 0; // default tolerance

    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...." << std::endl;

    for(int i = 0; i < 2; i++){
        if(decks[i] != nullptr){
            delete decks[i];
            decks[i] = nullptr;
        }
    }

}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    if(decks[0] == nullptr && decks[1] == nullptr){
        // Clone the reffered pointer 
        // Clone function returns a wrapped pointer 
        // intiallize deck by realeasing the wrapped pointer and return a (cloned)raw pointer
        decks[0] = track.clone().release();
        active_deck = 0;
    }
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;

    //(b) Clone track polymorphically
    PointerWrapper<AudioTrack> cloned_track = track.clone();
    
    //(c) Check for clone failure
    if(!cloned_track){
        std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone." << std::endl;
        return -1;
    }

    // (d) Identify target dec
    size_t load_index = (1 - active_deck);

    // (e) Log deck switch:
    std::cout << "[Deck Switch] Target deck: " << load_index << std::endl;

    // (f) Unload target deck if occupied
    if(decks[load_index] != nullptr){
        delete decks[load_index];
        decks[load_index] = nullptr;
    }

    // (g) Perform track preparation 
    cloned_track->load();
    cloned_track->analyze_beatgrid();


    // (h) BPM managment - auto sync if enabled and mixable
    if(auto_sync && can_mix_tracks(cloned_track)){
        sync_bpm(cloned_track);
    }

    // (i) assign track to target deck
    decks[load_index] = cloned_track.release();
    std::cout << "[Load Complete] '" << decks[load_index]->get_title() << "' is now loaded on deck " << load_index << std::endl;

    // (j) Unload previous active deck
    std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() << ")" << std::endl;
    delete decks[active_deck];
    decks[active_deck] = nullptr;

    // (k) Switch active deck
    active_deck = load_index;
    std::cout << "[Active Deck] Switched to deck " << active_deck << std::endl;

    return load_index; 
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] == nullptr || !track) {
        return false; // Cannot mix if active deck is empty or track is null
    }
    int bpm_active = decks[active_deck]->get_bpm();
    int bpm_new = track->get_bpm();
    
    int bpm_diff = std::abs(bpm_active - bpm_new);
    
    if(bpm_diff <= bpm_tolerance){
        return true;
    }
    return false;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] != nullptr && track){
        int original_bpm = track->get_bpm();
        int active_bpm = decks[active_deck]->get_bpm();
        int average_bpm = (active_bpm + original_bpm) / 2;
        
        track->set_bpm(average_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << original_bpm << " to " << average_bpm << std::endl;
    }
}
