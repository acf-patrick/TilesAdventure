#pragma once

#include <vector>
#include <functional>
#include <SDL2/SDL.h>

class Timer
{
using Callback = std::function<void(void)>;

private:
    static std::vector<Timer*> instances;

    std::vector<Callback> callbacks_;

    Uint32 lastTick_;

    Uint32 time_;

    int count_;

    // Call recorded callbacks
    void call();

public:
    /**
     * @param time Elapsed time in seconds between callback call
     * @param count Number of callback call [default : -1 means call indefinitely]
    */
    Timer(float time, int count = -1);

    ~Timer();

    void addCallback(const Callback& callback);

    friend class Game;
};