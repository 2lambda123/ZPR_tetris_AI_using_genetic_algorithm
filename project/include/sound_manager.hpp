/*
 * Author: Rafał Kulus
 */

#ifndef GENETIC_TETRIS_SOUND_MANAGER_HPP
#define GENETIC_TETRIS_SOUND_MANAGER_HPP

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace genetic_tetris {

class SoundManager {
public:
    enum class Sound { TETRIS_THEME, CLICK, HARD_DROP, ROW_CLEARED };

    static SoundManager& getInstance();

    SoundManager(const SoundManager&) = delete;
    SoundManager operator=(const SoundManager&) = delete;

    void play(Sound sound);

private:
    typedef struct SoundProperties {
        SoundProperties(std::string path, float volume, bool loop = false);
        SoundProperties(const SoundProperties& sound_properties);

        std::string path;
        float volume;
        bool loop;
        std::unique_ptr<sf::SoundBuffer> buffer;
    } SoundProperties;

    static std::unordered_map<Sound, SoundProperties>& getSounds();

    SoundManager();
    void garbageCollector();

    std::vector<std::unique_ptr<sf::Sound> > playing_sounds_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_SOUND_MANAGER_HPP
