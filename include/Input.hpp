#pragma once
#include <SDL.h>
#include <cstdint>

/**
 * Tracks keyboard and mouse input state.
 */
class Input
{

    public:

    /**
     * Constructor to create an input tracker. Requires SDL to already be initialized.
     */
    Input();

    /**
     * Refreshes the keyboard and mouse state for the current frame
     */
    void poll();

    /**
     * Getter for whether a keyboard key is currently pressed
     * @param key - the SDL scancode of the key to check
     */
    bool isKeyDown(SDL_Scancode key) const;

    /**
     * Getter for whether a mouse button is currently pressed
     * @param button - the SDL button index to check
     */
    bool isMouseButtonDown(uint8_t button) const;

    /**
     * Getter for the current mouse position
     * @param x - output parameter for the mouse x-coordinate
     * @param y - output parameter for the mouse y-coordinate
     */
    void getMousePosition(int& x, int& y) const;

    private:

    const uint8_t* keyboardState = nullptr;
    uint32_t mouseState = 0;
    int mouseX = 0;
    int mouseY = 0;
};
