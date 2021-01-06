#ifndef GENETIC_TETRIS_SOUND_MANAGER_HPP
#define GENETIC_TETRIS_SOUND_MANAGER_HPP

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <unordered_map>
#include <vector>
#include <memory>

namespace gentetris {

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
            : path(path), volume(volume), loop(loop), buffer(nullptr) {}
        SoundProperties(const SoundProperties& sound_properties){
            path = sound_properties.path;
            volume = sound_properties.volume;
            loop = sound_properties.loop;
//            buffer = std::move(sound_properties.buffer);
            buffer = nullptr;
        };
//        SoundProperties(const SoundProperties& sound_properties) = default;

        std::string path;
        float volume;
        bool loop;
        std::unique_ptr<sf::SoundBuffer> buffer;
    } SoundProperties;

    static const std::vector<std::pair<Sound, SoundProperties> > SOUND_FILES_;

    SoundManager();
    void garbageCollector();

    std::unordered_map<Sound, std::unique_ptr<SoundProperties> > sounds_;
    std::vector<std::unique_ptr<sf::Sound> > playing_sounds_;
};

}  // namespace gentetris

#endif  // GENETIC_TETRIS_SOUND_MANAGER_HPP
