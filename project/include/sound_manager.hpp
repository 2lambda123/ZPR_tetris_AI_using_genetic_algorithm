#ifndef GENETIC_TETRIS_SOUND_MANAGER_HPP
#define GENETIC_TETRIS_SOUND_MANAGER_HPP

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace genetic_tetris {

enum class Sound { TETRIS_THEME, CLICK, HARD_DROP, ROW_CLEARED };

class SoundManager {
public:
    static SoundManager& getInstance();

    SoundManager(const SoundManager&) = delete;
    SoundManager operator=(const SoundManager&) = delete;

    void play(Sound sound);

private:
    typedef struct SoundProperties {
        SoundProperties(std::string path, float volume, bool loop = false)
            : path(std::move(path)), volume(volume), loop(loop), buffer(nullptr) {}
        // TODO: comment about the awful constructor below
        SoundProperties(const SoundProperties& sound_properties) {
            path = sound_properties.path;
            volume = sound_properties.volume;
            loop = sound_properties.loop;
            buffer = nullptr;
        };

        std::string path;
        float volume;
        bool loop;
        std::unique_ptr<sf::SoundBuffer> buffer;
    } SoundProperties;

    static std::unordered_map<Sound, SoundProperties> sounds_;

    SoundManager();
    void garbageCollector();

    std::vector<std::unique_ptr<sf::Sound> > playing_sounds_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_SOUND_MANAGER_HPP
