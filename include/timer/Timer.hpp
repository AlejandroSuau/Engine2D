#pragma once

namespace Engine2D {

class Timer {
public:
    Timer(float duration_in_secs);

    void Start();
    void Reset(bool is_running = true);
    bool Update(float dt);
    float ElapsedTime() const;
    bool DidFinish() const;
    bool IsRunning() const;
    void SetDuration(float duration_in_secs);

private:
    float duration_in_secs_;
    float elapsed_time_in_secs_;
    bool is_running_;
};

}
