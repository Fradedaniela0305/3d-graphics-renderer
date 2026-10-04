#include "Clock.hpp"
#include <SDL.h>

Clock::Clock()
    : lastTickCounter(SDL_GetPerformanceCounter())
{
}

float Clock::tick()
{
    uint64_t now = SDL_GetPerformanceCounter();
    float deltaTime = (float)(now - lastTickCounter) / (float)SDL_GetPerformanceFrequency();
    lastTickCounter = now;
    return deltaTime;
}
