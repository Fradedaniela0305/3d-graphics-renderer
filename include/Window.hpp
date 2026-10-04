#pragma once
#include <SDL.h>

/**
 * Owns the SDL2 window/renderer lifecycle and the event loop's quit flag.
 */
class Window {

    public:

    /**
     * Constructor to create a window
     * @param title - the window's title
     * @param width - the window's width in pixels
     * @param height - the window's height in pixels
     */
    Window(const char* title, int width, int height);

    /**
     * Destructor, destroys the SDL renderer and window
     */
    ~Window();

    /**
     * Creates the SDL window and renderer
     * @returns false if window/renderer creation failed
     */
    bool init();

    /**
     * Pumps the SDL event queue; updates the quit flag
     */
    void pollEvents();

    /**
     * Getter for whether the window has been requested to close
     */
    bool shouldClose() const { return quit; }

    /**
     * Getter for the SDL renderer created by this window.
     * Window retains ownership; callers must not destroy it.
     */
    SDL_Renderer* getRenderer() const { return renderer; }

    /**
     * Getter for the window width
     */
    int getWidth() const { return width; }

    /**
     * Getter for the window height
     */
    int getHeight() const { return height; }

    private:

    const char* title;
    int width;
    int height;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool quit = false;
};
