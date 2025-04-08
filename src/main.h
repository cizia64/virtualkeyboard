/**
 * @file  main.h
 * @brief Main header file for the Virtual Keyboard application.
 */
#ifndef _MAIN_H_
#define _MAIN_H_

/**
 * @namespace Globals
 * @brief     Namespace containing global variables for the application.
 */
namespace Globals 
{
    /**
     * @brief Global SDL window.
     */
    SDL_Window* g_sdlwindow = nullptr;

    /**
     * @brief Global SDL surface for the screen.
     */
    SDL_Surface* g_screen = nullptr;

    /**
     * @brief Normal-text color.
     */
    const SDL_Color g_colorTextNormal = { COLOR_TEXT_NORMAL };

    /**
     * @brief Title-text color.
     */
    const SDL_Color g_colorTextTitle = { COLOR_TEXT_TITLE };

    /**
     * @brief Vector of window pointers.
     */
    std::vector<CWindow*> g_windows;
}

/**
 * @brief  Initializes SDL.
 * @return TRUE if the SDL was initialized successfully; otherwise, FALSE.
 */
const bool initSDL(void);

/**
 * @brief Initializes the joystick, if found.
 */
void initJoystick(void);

/**
 * @brief  Initializes the screen.
 * @return TRUE if the screen was initialized successfully; otherwise, FALSE.
 */
const bool initScreen(void);

/**
 * @brief      Initializes resources.
 * @param argc The amount of external arguments passed when executed the program.
 * @param argv The array of passed arguments.
 * @return     TRUE if resources were initialized successfully; otherwise, FALSE.
 */
const bool initResources(const int p_argumentCount, char** const p_argumentValues);

/**
 * @brief      Initializes the keyboard.
 * @param argc The amount of external arguments passed when executed the program.
 * @param argv The array of passed arguments.
 * @return     0 if the keyboard was initialized successfully; otherwise, 1.
 */
const int initKeyboard(const int p_argumentCount, char** const p_argumentValues);

#endif // _MAIN_H_
