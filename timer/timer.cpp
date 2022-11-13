#include "timer.h"

std::set<Timer*, Timer::Compare> Timer::instances;

Timer::Timer(float time, int count) :
    time_(time * 1000), count_(count)
{
    lastTick_ = SDL_GetTicks();
    instances.insert(this);
}

Timer::~Timer()
{
    instances.erase(this);
}

void Timer::addCallback(const Callback& callback)
{
    callbacks_.emplace_back(callback);
}

bool Timer::call()
{
    if (count_ == 0)
        return true;

    auto curr = SDL_GetTicks();

    if (curr - lastTick_ < time_)
        return false;

    lastTick_ = curr;
    count_--;

    for (auto callback : callbacks_)
        callback();

    return true;
}