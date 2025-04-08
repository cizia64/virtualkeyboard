/**
 * @file sdlUtils.h
 * @brief Utility functions and definitions for SDL.
 */
#ifndef _SDLUTILS_H_
#define _SDLUTILS_H_

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "window.h"

/**
 * @namespace Globals
 * @brief     Namespace containing global variables for the application.
 */
namespace Globals
{
    /**
     * @brief Global SDL surface for the screen.
     */
    extern SDL_Surface* g_screen;

    /**
     * @brief Global SDL window.
     */
    extern SDL_Window* g_sdlwindow;

    /**
     * @brief normal-text color.
     */
    extern const SDL_Color g_colorTextNormal;

    /**
     * @brief title-text color.
     */
    extern const SDL_Color g_colorTextTitle;

    /**
     * @brief The list of opened windows.
     */
    extern std::vector<CWindow*> g_windows;
}

/**
 * @namespace SDL_Utils
 * @brief     Namespace containing utility functions for SDL.
 */
namespace SDL_Utils
{
    /**
     * @enum  ETEXTALIGN
     * @brief Enumeration for text alignment.
     */
    enum class ETextAlign : Uint8
    {
        LEFT = 0,   /**< Left alignment */
        RIGHT,      /**< Right alignment */
        CENTER      /**< Center alignment */
    };

    /**
     * @brief          Creates an SDL rectangle with the given parameters.
     * @param p_x      The x coordinate.
     * @param p_y      The y coordinate.
     * @param p_width  The width.
     * @param p_height The height.
     * @return         The created SDL rectabgle.
     */
    inline SDL_Rect Rect(Sint16 p_x, Sint16 p_y, Uint16 p_width, Uint16 p_height)
    {
        return SDL_Rect{ p_x, p_y, p_width, p_height };
    }

    /**
     * @brief             Loads an image to fit the given viewport size.
     * @param p_filename  The filename of the image.
     * @param p_fitWidth  The width to fit.
     * @param p_fitHeight The height to fit.
     * @return            Pointer to the loaded SDL surface.
     */
    SDL_Surface* loadImageToFit(const std::string& p_filename, int p_fitWidth, int p_fitHeight);

    /**
     * @brief            Checks whether the given filename has a supported image extension.
     * @param p_filename The filename to check.
     * @return           TRUE if the extension is supported; otherwise, FALSE.
     */
    bool isSupportedImageExt(const std::string& p_filename);

    /**
     * @brief        Loads a TTF font.
     * @param p_font The font filename.
     * @param p_size The font sizem, in pixels.
     * @return       Pointer to the loaded TTF font.
     */
    TTF_Font* loadFont(const std::string& p_font, const int p_size);

    /**
     * @brief               Applies a source surface on destination surface.
     * @param p_x           The coordinate on the horizontal axis.
     * @param p_y           The coordinate on the vertical axis.
     * @param p_source      The source surface.
     * @param p_destination The destination surface.
     * @param p_clip        The clipping rectangle (optional).
     */
    void applySurface(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_source, SDL_Surface* p_destination, SDL_Rect* p_clip = nullptr);

    /**
     * @brief                   Renders a specific text string.
     * @param p_font            The font to use.
     * @param p_text            The text to render.
     * @param p_foregroundColor The foreground color.
     * @param p_backgroundColor The background color.
     * @return                  Pointer to the rendered SDL surface.
     */
    SDL_Surface* renderText(TTF_Font* p_font, const std::string& p_text, const SDL_Color& p_foregroundColor, const SDL_Color& p_backgroundColor);

    /**
     * @brief                   Renders a text and applies it on a given surface.
     * @param p_x               The coordinate on the horizontal axis.
     * @param p_y               The coordinate on the vertical axis.
     * @param p_destination     The destination surface.
     * @param p_font            The font to use.
     * @param p_text            The text to render.
     * @param p_foregroundColor The foreground color.
     * @param p_backgroundColor The background color.
     * @param p_align           The text alignment.
     */
    void applyText(Sint16 p_x, Sint16 p_y, SDL_Surface* p_destination, TTF_Font* p_font, const std::string& p_text, const SDL_Color& p_foregroundColor, const SDL_Color& p_backgroundColor, const ETextAlign p_align = ETextAlign::LEFT);

    /**
     * @brief          Creates a surface in the same format as the screen.
     * @param p_width  The width of the surface.
     * @param p_height The height of the surface.
     * @return         Pointer to the created SDL surface.
     */
    SDL_Surface* createSurface(int p_width, int p_height);

    /**
     * @brief          Creates an image filled with the given color.
     * @param p_width  The width of the image.
     * @param p_height The height of the image.
     * @param p_color  The color to fill.
     * @return         Pointer to the created SDL_Surface.
     */
    SDL_Surface* createImage(const int p_width, const int p_height, const Uint32 p_color);

    /**
     * @brief Renders all opened windows.
     */
    void renderAll(void);

    /**
     * @brief Cleans up SDL resources and quits the application.
     */
    void cleanupAndQuit(void);
}

#endif // _SDLUTILS_H_
