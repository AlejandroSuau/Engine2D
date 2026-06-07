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

void AudioManager::Update(float dt) {
    for (auto it = pending_sounds_.begin(); it != pending_sounds_.end(); ) {
        it->remainingSeconds -= dt;

        if (it->remainingSeconds <= 0.0f) {
            PlayOneShot(it->id, it->volume);
            it = pending_sounds_.erase(it);
        } else {
            ++it;
        }
    }
}

Mix_Chunk* AudioManager::GetChunk(SoundId id) const {
    return chunks_[ToIndex(id)];
}

void AudioManager::PlayOneShot(SoundId id) {
    PlayOneShot(id, MIX_MAX_VOLUME);
}

void AudioManager::PlayOneShot(SoundId id, int volume) {
    Mix_Chunk* chunk = GetChunk(id);
    if (!chunk) {
        return;
    }

    const int channel = Mix_PlayChannel(-1, chunk, 0);
    if (channel >= 0) {
        Mix_Volume(channel, std::clamp(volume, 0, MIX_MAX_VOLUME));
    }
}

void AudioManager::PlayDelayedOneShot(SoundId id, float delaySeconds, int volume) {
    pending_sounds_.push_back(PendingSound{
        id,
        delaySeconds,
        std::clamp(volume, 0, MIX_MAX_VOLUME)
    });
}

int AudioManager::PlayLoop(SoundId id, int volume) {
    Mix_Chunk* chunk = GetChunk(id);
    if (!chunk) {
        return -1;
    }

    const int channel = Mix_PlayChannel(-1, chunk, -1);
    if (channel >= 0) {
        Mix_Volume(channel, std::clamp(volume, 0, MIX_MAX_VOLUME));
    }

    return channel;
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

    // Vector desde el jugador hasta la fuente sonora.
    // Puesto que las paredes no bloquean sonido, basta con la distancia directa
    // entre casillas
    const auto dx = static_cast<float>(source.x - listener.x);
    const auto dy = static_cast<float>(source.y - listener.y);
    const float distance_cells = Length(dx, dy);

    // Se normaliza la distancia real del mapa a un rango de 0 a 1
    const float normalized_distance = std::clamp(
        distance_cells / static_cast<float>(kMaxDistanceCells),
        0.0f,
        1.0f
    );

    const auto sdl_distance = static_cast<Uint8>(
        std::clamp(
            static_cast<int>(normalized_distance * kMaxDistance),
            kMinDistance,
            kMaxDistance
        )
    );

    // Dirección hacia la que mira el jugador (normalizada cardinal)
    const auto forward_x = static_cast<float>(listenerDir.x);
    const auto forward_y = static_cast<float>(listenerDir.y);

    float angle_degrees = 0.0f;

    // Si jugador y fuente están en la misma casilla no hay dirección, se 
    // deja el ángulo a 0 y se mantiene distancia mínima
    if (distance_cells > 0.001f) {
        // Vector normalizado hacia la fuente sonora
        const float source_x = dx / distance_cells;
        const float source_y = dy / distance_cells;

        // Converte coordenadas del mapa a coordenadas relativas al jugador
        const float right_x = -forward_y;
        const float right_y = forward_x;

        // Proyecciones de la fuente respecto al eje fronntal y lateral del jugador.
        // forward_dot > 0 = fuente delante
        // right_dot > 0 = fuente a la derecha
        const float forward_dot = forward_x * source_x + forward_y * source_y;
        const float right_dot = right_x * source_x + right_y * source_y;

        // Ángulo relativo: 0 delante, 90 derecha, 180 detrás, 270 izquierda
        angle_degrees = std::atan2(right_dot, forward_dot) * 180.0f / 3.14159265f;

        if (angle_degrees < 0.0f) {
            angle_degrees += 360.0f;
        }
    }

    // Panning + atenuación por distancia
    Mix_SetPosition(
        channel,
        static_cast<Sint16>(angle_degrees),
        sdl_distance
    );
}