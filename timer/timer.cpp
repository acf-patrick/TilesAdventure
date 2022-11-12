#include "timer.h"

std::vector<Timer*> Timer::instances;

Timer::Timer(float time, int count) :
    time_(time * 1000), count_(count)
{
    lastTick_ = SDL_GetTicks();
    instances.emplace_back(this);
}

Timer::~Timer()
{
    instances.erase(std::remove(instances.begin(), instances.end(), this));
}

void Timer::addCallback(const Callback& callback)
{
    callbacks_.emplace_back(callback);
}

void Timer::call()
{
    if (count_ == 0)
        return;

    auto curr = SDL_GetTicks();
    if (curr - lastTick_ >= time_)
    {
        lastTick_ = curr;
        count_--;

        for (auto callback : callbacks_)
            callback();
    }
}