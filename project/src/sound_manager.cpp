#include "sound_manager.hpp"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "gui/gui.hpp"

namespace gentetris {

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::play(Sound sound) {
    auto sounds_iter = sounds_.find(sound);
    if (sounds_iter != sounds_.end()) {
        auto sound_to_play = std::make_unique<sf::Sound>();
        sound_to_play->setBuffer(*(sounds_iter->second.buffer));
        sound_to_play->setVolume(sounds_iter->second.volume);
        sound_to_play->setLoop(sounds_iter->second.loop);
        sound_to_play->play();
        playing_sounds_.push_back(std::move(sound_to_play));
        garbageCollector();
    }
}

std::unordered_map<Sound, SoundManager::SoundProperties> SoundManager::sounds_ = {
    {Sound::TETRIS_THEME, SoundProperties("res/sounds/tetris_theme.ogg", 6.0f, true)},
    {Sound::CLICK, SoundProperties("res/sounds/click.ogg", 25.0f)},
    {Sound::HARD_DROP, SoundProperties("res/sounds/hard_drop.ogg", 25.0f)},
    {Sound::ROW_CLEARED, SoundProperties("res/sounds/row_cleared.ogg", 30.0f)}
};

SoundManager::SoundManager() {
    for (auto& sound_file : sounds_) {
        auto sound_buffer = std::make_unique<sf::SoundBuffer>();
        if (!sound_buffer->loadFromFile(sound_file.second.path)) {
            throw std::runtime_error("Cannot open " + sound_file.second.path);
        }
        sound_file.second.buffer = std::move(sound_buffer);
    }
}

void SoundManager::garbageCollector() {
    unsigned int i = 0;
    while (i < playing_sounds_.size()) {
        if (playing_sounds_[i]->getStatus() != sf::Sound::Status::Playing) {
            playing_sounds_.erase(playing_sounds_.begin() + i);
        } else {
            ++i;
        }
    }
}

}  // namespace gentetris
