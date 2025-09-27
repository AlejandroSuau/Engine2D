#include "timer/Timer.hpp"

Timer::Timer(float duration_in_millisecs) 
    : duration_in_millisecs_(duration_in_millisecs)
    , elapsed_time_in_millisecs_(0.f)
    , is_running_(false) {}

void Timer::Start() {
    is_running_ = true;
    elapsed_time_in_millisecs_ = 0;
}

void Timer::Reset(bool is_running) {
    is_running_ = is_running;
    elapsed_time_in_millisecs_ = 0;
}

bool Timer::Update(float dt) {
    if (!is_running_) return false;

    elapsed_time_in_millisecs_ += dt;
    const bool did_finish = DidFinish();
    if (did_finish) {
        Reset();
    }
    return did_finish;
}

float Timer::ElapsedTime() const {
    return elapsed_time_in_millisecs_;

}
bool Timer::DidFinish() const {
    return elapsed_time_in_millisecs_ >= duration_in_millisecs_;
}

bool Timer::IsRunning() const {
    return is_running_;
}

void Timer::SetDuration(float duration_in_millisecs) {
    duration_in_millisecs_ = duration_in_millisecs;
}
