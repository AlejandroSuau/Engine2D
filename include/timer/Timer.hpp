#pragma once

class Timer {
public:
    Timer(float duration_in_millisecs);

    void Start();
    void Reset(bool is_running = false);
    bool Update(float dt);
    float ElapsedTime() const;
    bool DidFinish() const;
    bool IsRunning() const;
    void SetDuration(float duration_in_millisecs);

private:
    float duration_in_millisecs_;
    float elapsed_time_in_millisecs_;
    bool is_running_;
};