#include "AudioManager.hpp"

#include <iostream>
#include <algorithm>

namespace {
    constexpr int kMaxDistanceCells = 10;
    constexpr int kMinDistance = 0;
    constexpr int kMaxDistance = 255;

    int ToIndex(SoundId id) {
        return static_cast<int>(id);
    }

    float Length(float x, float y) {
        return std::sqrt(x * x + y * y);
    }
}

bool AudioManager::Load() {
    bool success = true;

    success &= LoadChunk(SoundId::MonsterSnore, "assets/sounds/monster_snore.wav");
    success &= LoadChunk(SoundId::WallHit, "assets/sounds/wall_hit.wav");
    success &= LoadChunk(SoundId::Waterfall, "assets/sounds/waterfall.wav");
    success &= LoadChunk(SoundId::MonsterEat, "assets/sounds/monster_eat.wav");
    success &= LoadChunk(SoundId::PlayerStep, "assets/sounds/player_step.wav");
    success &= LoadChunk(SoundId::MonsterStep, "assets/sounds/monster_step.wav");
    success &= LoadChunk(SoundId::Victory, "assets/sounds/victory.wav");
    success &= LoadChunk(SoundId::Defeat, "assets/sounds/defeat.wav");

    return success;
}

bool AudioManager::LoadChunk(SoundId id, const std::string& path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());

    if (!chunk) {
        std::cerr << "Failed to load sound: " << path
                  << " | SDL_mixer error: " << Mix_GetError() << std::endl;
        return false;
    }


    chunks_[ToIndex(id)] = chunk;
    return true;
}

void AudioManager::Shutdown() {
    StopAll();

    for (auto*& chunk : chunks_) {
        if (chunk) {
            Mix_FreeChunk(chunk);
            chunk = nullptr;
        }
    }
}

Mix_Chunk* AudioManager::GetChunk(SoundId id) const {
    return chunks_[ToIndex(id)];
}
void AudioManager::PlayOneShot(SoundId id) {
    Mix_Chunk* chunk = GetChunk(id);
    if (!chunk) {
        return;
    }

    Mix_PlayChannel(-1, chunk, 0);
}

int AudioManager::PlayLoop(SoundId id) {
    Mix_Chunk* chunk = GetChunk(id);
    if (!chunk) {
        return -1;
    }

    return Mix_PlayChannel(-1, chunk, -1);
}

void AudioManager::StopChannel(int channel) {
    if (channel >= 0) {
        Mix_HaltChannel(channel);
    }
}

void AudioManager::StopAll() {
    Mix_HaltChannel(-1);
}

void AudioManager::UpdateSpatialChannel(
    int channel,
    ColRow_t listener,
    ColRow_t source,
    Vec2<int> listenerDir) {

    if (channel < 0 || !Mix_Playing(channel)) {
        return;
    }

    const float dx = static_cast<float>(source.x - listener.x);
    const float dy = static_cast<float>(source.y - listener.y);

    const float distance_cells = Length(dx, dy);
    const float normalized_distance = std::clamp(
        distance_cells / static_cast<float>(kMaxDistanceCells),
        0.0f,
        1.0f
    );

    const Uint8 sdl_distance = static_cast<Uint8>(
        std::clamp(
            static_cast<int>(normalized_distance * kMaxDistance),
            kMinDistance,
            kMaxDistance
        )
    );

    const float forward_x = static_cast<float>(listenerDir.x);
    const float forward_y = static_cast<float>(listenerDir.y);

    float angle_degrees = 0.0f;

    if (distance_cells > 0.001f) {
        const float source_x = dx / distance_cells;
        const float source_y = dy / distance_cells;

        const float right_x = -forward_y;
        const float right_y = forward_x;

        const float forward_dot = forward_x * source_x + forward_y * source_y;
        const float right_dot = right_x * source_x + right_y * source_y;

        angle_degrees = std::atan2(right_dot, forward_dot) * 180.0f / 3.14159265f;

        if (angle_degrees < 0.0f) {
            angle_degrees += 360.0f;
        }
    }

    Mix_SetPosition(
        channel,
        static_cast<Sint16>(angle_degrees),
        sdl_distance
    );
}