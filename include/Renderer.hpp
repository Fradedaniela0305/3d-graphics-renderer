#pragma once
#include "Color.hpp"
#include "Triangle.hpp"
#include <SDL.h>

/**
 * Draws pixels, shapes, and triangles onto an SDL renderer.
 */
class Renderer
{

    public:

    /**
     * Constructor to create a renderer that draws onto the given SDL renderer.
     * Renderer does not take ownership of the SDL_Renderer; the caller (Window) remains responsible for it.
     * @param renderer - the SDL renderer to draw onto
     */
    explicit Renderer(SDL_Renderer* renderer);

    /**
     * Clears the renderer to a solid color
     * @param color - the color to clear the screen to
     */
    void clear(Color color);

    /**
     * Presents the rendered frame to the screen
     */
    void present();

    /**
     * Draws a single pixel
     * @param x - x-coordinate of the pixel
     * @param y - y-coordinate of the pixel
     * @param color - color of the pixel
     */
    void drawPixel(int x, int y, Color color);

    /**
     * Draws a line between two points
     * @param x1 - x-coordinate of the first point
     * @param y1 - y-coordinate of the first point
     * @param x2 - x-coordinate of the second point
     * @param y2 - y-coordinate of the second point
     * @param color - color of the line
     */
    void drawLine(int x1, int y1, int x2, int y2, Color color);

    /**
     * Draws a rectangle
     * @param x - x-coordinate of the top-left corner
     * @param y - y-coordinate of the top-left corner
     * @param w - width of the rectangle
     * @param h - height of the rectangle
     * @param color - color of the rectangle
     * @param filled - whether the rectangle is filled or just outlined
     */
    void drawRect(int x, int y, int w, int h, Color color, bool filled = true);

    /**
     * Draws a circle
     * @param centerX - x-coordinate of the circle's center
     * @param centerY - y-coordinate of the circle's center
     * @param radius - radius of the circle
     * @param color - color of the circle
     * @param filled - whether the circle is filled or just outlined
     */
    void drawCircle(int centerX, int centerY, int radius, Color color, bool filled = true);

    /**
     * Draws a triangle
     * @param triangle - the triangle to draw
     * @param color - color of the triangle
     */
    void drawTriangle(Triangle triangle, Color color);

    /**
     * Draws a filled triangle
     * @param triangle - the triangle to draw
     * @param color - color of the triangle
     */
    void drawFilledTriangle(Triangle triangle, Color color);

    private:

    SDL_Renderer* renderer;
};
