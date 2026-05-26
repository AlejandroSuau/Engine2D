#pragma once

#include "Grid.hpp"

#include <SDL_mixer.h>

#include <string>
#include <array>

enum class SoundId {
    MonsterSnore,
    WallHit,
    Waterfall,
    MonsterEat,
    PlayerStep,
    MonsterStep,
    Victory,
    Defeat,
    Count // Para saber la cantidad
};

class AudioManager {
public:
    bool Load();
    void Shutdown();

    void PlayOneShot(SoundId id);
    int PlayLoop(SoundId id);
    void StopChannel(int channel);

    void UpdateSpatialChannel(
        int channel,
        ColRow_t listener,
        ColRow_t source,
        Vec2<int> listenerDir);
    void StopAll();

private:
    static constexpr int kSoundCount = static_cast<int>(SoundId::Count);
    std::array<Mix_Chunk*, kSoundCount> chunks_ {};

    Mix_Chunk* GetChunk(SoundId id) const;
    bool LoadChunk(SoundId id, const std::string& path);
};
