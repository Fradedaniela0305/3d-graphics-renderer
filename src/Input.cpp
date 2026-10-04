#include "Input.hpp"

Input::Input()
    : keyboardState(SDL_GetKeyboardState(nullptr))
{
}

void Input::poll()
{
    mouseState = SDL_GetMouseState(&mouseX, &mouseY);
}

bool Input::isKeyDown(SDL_Scancode key) const
{
    return keyboardState && keyboardState[key];
}

bool Input::isMouseButtonDown(uint8_t button) const
{
    return mouseState & SDL_BUTTON(button);
}

void Input::getMousePosition(int &x, int &y) const
{
    x = mouseX;
    y = mouseY;
}
