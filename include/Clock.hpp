#pragma once
#include <cstdint>

/**
 * Tracks frame timing.
 */
class Clock
{

    public:

    /**
     * Constructor to create a clock, starting the timer immediately
     */
    Clock();

    /**
     * Advances the frame clock
     * @returns delta time in seconds since the last call
     */
    float tick();

    private:

    uint64_t lastTickCounter;
};
