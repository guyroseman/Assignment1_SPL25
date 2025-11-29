#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {    
    for (size_t i = 0; i < library_tracks.size(); ++i) {
        // (a) check foramt
        AudioTrack* newTrack = nullptr;
        bool isMP3 = false;
        if(library_tracks[i].type == "MP3"){
            // (b) create appropriate MP3 track type 
            newTrack = new MP3Track(library_tracks[i].title, library_tracks[i].artists,
                 library_tracks[i].duration_seconds,
                 library_tracks[i].bpm,library_tracks[i].extra_param1, // extra_param1 = bitrate,
                 library_tracks[i].extra_param2);  // extra_param2 = has_tags
            
            isMP3 = true;
        }
        // (b) create appropriate WAV track type
        else{
            newTrack = new WAVTrack(library_tracks[i].title, library_tracks[i].artists,
                 library_tracks[i].duration_seconds,
                 library_tracks[i].bpm,library_tracks[i].extra_param1, // extra_param1 = sample_rate,
                 library_tracks[i].extra_param2);  // extra_param2 = bit_depth
        }

        // (c) store in the library vector
        library.push_back(newTrack);
        
        // (d) print log creation message
        if(isMP3){
            std::cout << "– MP3: MP3Track created: " 
                      << library_tracks[i].extra_param1 << " kbps" << std::endl;
        }
        else{
            std::cout << "– WAV: WAVTrack created: " 
                      << library_tracks[i].extra_param1 << "Hz/"
                      << library_tracks[i].extra_param2 << "bit" << std::endl;
        }
    }
    std::cout << "[INFO] Track library built: " 
                      << library_tracks.size() << " tracks loaded" << std::endl; 
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // (a) Print Log
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;

    // (b) Create a new Playlist with the given name and update member variable
    playlist = Playlist(playlist_name);

    // (c) For each index in track_indices, add the corresponding track from library to the playlist
    for (size_t i = 0; i < track_indices.size(); ++i){
        // Validate index is within library bounds.
        int index = track_indices[i] - 1; // Convert 1-based to 0-based index
        if (index < 0 || index >= int(library.size())){
            std::cout << "[WARNING] Track index " << track_indices[i] 
                      << " is out of bounds. Skipping." << std::endl;
            continue; 
        }
        else{
            // Clone the track polymorphically and add to playlist
            PointerWrapper<AudioTrack> cloned_track = library[index]->clone();
            
            // If clone is nullptr, log error and skip
            if(!cloned_track){
                std::cout << "[ERROR] Cloning track failed for index: " << track_indices[i] << "\n";
                continue;
            }
            
            cloned_track->load();
            cloned_track->analyze_beatgrid();

            // Get title while wrapper is valid
            std::string track_title = cloned_track->get_title(); 

            // Add cloned track to playlist
            playlist.add_track(cloned_track.release());

            // log addition
            std::cout << "Added '" << track_title << "' to playlist '" << playlist_name << "'" << std::endl;
        }   
    }
    // log summary 
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << playlist.get_track_count() << " tracks)" << std::endl;

    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
}

/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles; // vector for titles
    std::vector<AudioTrack*> tracks = playlist.getTracks(); // vector for tracks
    
    for(size_t i = 0; i < tracks.size(); ++i){
        titles.push_back(tracks[i]->get_title()); // push each track's title to titles vector 
    }

    return titles; 
}
