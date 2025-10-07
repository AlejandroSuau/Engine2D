#include "timer/Timer.hpp"

namespace Engine2D {

Timer::Timer(float duration_in_secs) 
    : duration_in_secs_(duration_in_secs)
    , elapsed_time_in_secs_(0.f)
    , is_running_(false) {}

void Timer::Start() {
    is_running_ = true;
    elapsed_time_in_secs_ = 0;
}

void Timer::Reset(bool is_running) {
    is_running_ = is_running;
    elapsed_time_in_secs_ = 0;
}

bool Timer::Update(float dt) {
    if (!is_running_) return false;

    elapsed_time_in_secs_ += dt;
    const bool did_finish = DidFinish();
    if (did_finish) {
        Reset();
    }
    return did_finish;
}

float Timer::ElapsedTime() const {
    return elapsed_time_in_secs_;

}
bool Timer::DidFinish() const {
    return elapsed_time_in_secs_ >= duration_in_secs_;
}

bool Timer::IsRunning() const {
    return is_running_;
}

void Timer::SetDuration(float duration_in_secs) {
    duration_in_secs_ = duration_in_secs;
}

}

