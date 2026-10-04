#include "Renderer.hpp"

Renderer::Renderer(SDL_Renderer* renderer)
    : renderer(renderer)
{
}

void Renderer::clear(Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void Renderer::present()
{
    SDL_RenderPresent(renderer);
}

void Renderer::drawPixel(int x, int y, Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, x, y);
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Renderer::drawTriangle(Triangle triangle, Color color)
{
    drawLine(triangle.getP1().getX(), triangle.getP1().getY(), triangle.getP2().getX(), triangle.getP2().getY(), color);
    drawLine(triangle.getP2().getX(), triangle.getP2().getY(), triangle.getP3().getX(), triangle.getP3().getY(), color);
    drawLine(triangle.getP3().getX(), triangle.getP3().getY(), triangle.getP1().getX(), triangle.getP1().getY(), color);
}

/**
 * Draws a filled triangle
 * @param triangle - the triangle to draw
 * @param color - color of the triangle
 */
void Renderer::drawFilledTriangle(Triangle triangle, Color color)
{
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    SDL_Vertex vertices[3] = {
        {{(float)triangle.getP1().getX(), (float)triangle.getP1().getY()}, sdlColor, {0, 0}},
        {{(float)triangle.getP2().getX(), (float)triangle.getP2().getY()}, sdlColor, {0, 0}},
        {{(float)triangle.getP3().getX(), (float)triangle.getP3().getY()}, sdlColor, {0, 0}},
    };
    SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);
}

void Renderer::drawRect(int x, int y, int w, int h, Color color, bool filled)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    if (filled)
    {
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void Renderer::drawCircle(int centerX, int centerY, int radius, Color color, bool filled)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        if (filled)
        {
            SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX + x, centerY + y);
            SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX + x, centerY - y);
            SDL_RenderDrawLine(renderer, centerX - y, centerY + x, centerX + y, centerY + x);
            SDL_RenderDrawLine(renderer, centerX - y, centerY - x, centerX + y, centerY - x);
        }
        else
        {
            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
            SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
            SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
            SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        }

        y++;
        if (err <= 0)
        {
            err += 2 * y + 1;
        }
        if (err > 0)
        {
            x--;
            err -= 2 * x + 1;
        }
    }
}
