#pragma once

#include "Grid.hpp"

#include <SDL_mixer.h>

#include <string>
#include <array>
#include <vector>

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

    void PlayOneShot(SoundId id, int volume);
    void PlayOneShot(SoundId id);
    void PlayDelayedOneShot(SoundId id, float delaySeconds, int volume);
    int PlayLoop(SoundId id, int volume);
    void StopChannel(int channel);

    void Update(float dt);
    void UpdateSpatialChannel(
        int channel,
        ColRow_t listener,
        ColRow_t source,
        Vec2<int> listenerDir);
    void StopAll();

private:
    struct PendingSound {
        SoundId id;
        float remainingSeconds;
        int volume;
    };

    static constexpr int kSoundCount = static_cast<int>(SoundId::Count);
    std::array<Mix_Chunk*, kSoundCount> chunks_ {};
    std::vector<PendingSound> pending_sounds_;

    Mix_Chunk* GetChunk(SoundId id) const;
    bool LoadChunk(SoundId id, const std::string& path);
};
