/**
 * @file  sdlUtils.cpp
 * @brief Implementation file for globals in SDL_Utils namespace.
 */

#ifdef _WIN64
#define zoomSurface GFX_zoomSurface
#endif // _WIN64

#include "sdlUtils.h"
#include <algorithm>
#include <iostream>
#include <SDL2_rotozoom.h>
#include <SDL_image.h>
#include "def.h"
#include "resourceManager.h"
#include "screen.h"

bool SDL_Utils::isSupportedImageExt(const std::string& p_filename) 
{
	// Check if the file extension is supported (jpg, jpeg, png, ico, bmp and xcf).

    return p_filename == "jpg" || p_filename == "jpeg" || p_filename == "png" || p_filename == "ico" || p_filename == "bmp" || p_filename == "xcf";
}

SDL_Surface *SDL_Utils::loadImageToFit(const std::string &p_filename, int p_fitWidth, int p_fitHeight)
{
	// 1. Load the image with its filename into an SDL surface.
	// 2. Check if the image is loaded successfully and if not return a null pointer.
	// 3. Calculate the aspect ratio of the image.
	// 4. Determine the target width and height based on the aspect ratio and the given fit dimensions.
	// 5. Scale the image to fit the target dimensions.
	// 6. Convert the scaled image to RGBA8888 format.
	// 7. Free the original image surface.
	// 8. Return the scaled image surface.

    SDL_Surface* l_imgage = IMG_Load(p_filename.c_str());

    if (IMG_GetError() != nullptr && *IMG_GetError() != '\0') 
    {
        SDL_Log("Error when loading image: %s", IMG_GetError());
        SDL_ClearError();
        return nullptr;
    }

    const double l_aspectRatio = static_cast<double>(l_imgage->w) / l_imgage->h;
    int l_targetWidth, l_targetHeight;

    if (p_fitWidth * l_imgage->h <= p_fitHeight * l_imgage->w) 
    {
        l_targetWidth = std::min(l_imgage->w, p_fitWidth);
        l_targetHeight = static_cast<int>(l_targetWidth / l_aspectRatio);
    } 
    else 
    {
        l_targetHeight = std::min(l_imgage->h, p_fitHeight);
        l_targetWidth = static_cast<int>(l_targetHeight * l_aspectRatio);
    }

    l_targetWidth = static_cast<int>(l_targetWidth * Globals::g_Screen.m_ppuX);
    l_targetHeight = static_cast<int>(l_targetHeight * Globals::g_Screen.m_ppuY);

    SDL_Surface* l_image2 = zoomSurface(l_imgage, static_cast<double>(l_targetWidth) / l_imgage->w, static_cast<double>(l_targetHeight) / l_imgage->h, SMOOTHING_ON);
    SDL_FreeSurface(l_imgage);

    SDL_Surface* l_imgage3 = SDL_ConvertSurfaceFormat(l_image2, SDL_PIXELFORMAT_RGBA8888, 0); 
    SDL_FreeSurface(l_image2);
    return l_imgage3;
}

void SDL_Utils::applySurface(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_source, SDL_Surface* p_destination, SDL_Rect *p_clip)
{
	// 1. Create a rectangle to hold the offset position.
	// 2. Set the horizontal and vertical coordinates of the rectangle to the specified position.
	// 3. Blit the source surface onto the destination surface at the specified position.
	// 4. If a clipping rectangle is provided, use it to limit the area of the source surface that is rendered.

    SDL_Rect l_offset{};
    l_offset.x = p_x;
    l_offset.y = p_y;
    SDL_BlitSurface(p_source, p_clip, p_destination, &l_offset);
}

TTF_Font *SDL_Utils::loadFont(const std::string &p_font, const int p_size)
{
	// 1. Open the font file with the specified size.
	// 2. If the font file cannot be opened, log an error message.
	// 3. Return the loaded font (a null pointer if the loading operation fails).

    INHIBIT(SLD_Log("SDL_utils::loadFont(%s,%s)", p_font, p_size);)

    TTF_Font* l_font = TTF_OpenFont(p_font.c_str(), p_size);
    
    if (l_font == nullptr)
    {
		SDL_Log("Error when loading TTF font: %s", TTF_GetError());
		SDL_ClearError();
	}
    
    return l_font;
}

SDL_Surface* SDL_Utils::renderText(TTF_Font *p_font, const std::string &p_text, const SDL_Color& p_foregroundColor, const SDL_Color& p_backgroundColor)
{
	// 1. Render the text using the TTF font and the specified foreground and background colors.
	// 2. If the rendering operation fails, log an error message.
	// 3. Return the rendered surface (a null pointer if the rendering operation fails).

    SDL_Surface* result = TTF_RenderUTF8_Shaded(p_font, p_text.c_str(), p_foregroundColor, p_backgroundColor);

    if (result == nullptr) 
    {
		SDL_Log("Error getting TTF-shaded surface: %s", TTF_GetError());
        SDL_ClearError();
    }

    return result;
}

void SDL_Utils::applyText(Sint16 p_x, Sint16 p_y, SDL_Surface* p_destination, TTF_Font *p_font, const std::string &p_text, const SDL_Color &p_foregroundColor, const SDL_Color &p_backgroundColor, const ETextAlign p_align)
{
	// 1. Render the text using the TTF font and the specified foreground and background colors.
	// 2. Calculate the width of the rendered text, in pixels.
	// 3. Depending on the specified alignment, adjust the horizontal coordinate of the text position.
	// 4. Apply the rendered text surface onto the destination surface at the adjusted position.
	// 5. Free the rendered surface.

    SDL_Surface* l_text = renderText(p_font, p_text, p_foregroundColor, p_backgroundColor);
    auto value = l_text->w / Globals::g_Screen.getAdjustedPpuX();

    switch (p_align)
    {
        case ETextAlign::LEFT:
            applySurface(p_x - l_text->w, p_y, l_text, p_destination);
            break;
        case ETextAlign::RIGHT:
            applySurface(p_x, p_y, l_text, p_destination);
            break;
        case ETextAlign::CENTER:
            applySurface(p_x - l_text->w / 2, p_y, l_text, p_destination);
            break;
        default:
            break;
    }

    SDL_FreeSurface(l_text);
}

SDL_Surface* SDL_Utils::createSurface(int p_width, int p_height)
{
	// 1. Create a new SDL surface with the specified width and height, using the same pixel format as the screen.
	// 2. Return the created surface (a null pointer if the creation operation fails).

    return SDL_CreateRGBSurface(SDL_SWSURFACE, p_width, p_height, Globals::g_screen->format->BitsPerPixel, Globals::g_screen->format->Rmask, Globals::g_screen->format->Gmask, Globals::g_screen->format->Bmask, Globals::g_screen->format->Amask);
}

SDL_Surface* SDL_Utils::createImage(const int p_width, const int p_height, const Uint32 p_color)
{
	// 1. Create a new SDL surface with the specified width and height.
	// 2. If the surface creation fails, log an error message.
	// 3. Fill a rectangle in the surface with the specified color.
	// 4. Return the created surface (a null pointer if the creation operation fails).

    SDL_Surface* l_surface = createSurface(p_width, p_height);

    if (l_surface == nullptr)
    {
        SDL_LogError(0, "Could not create surface: %s", SDL_GetError());
    }
    else
    {
        SDL_FillRect(l_surface, nullptr, p_color);
    }

    return l_surface;
}

void SDL_Utils::renderAll(void)
{
    // 1. If there are no windows to render, return.
    // 2. Set an index value to the last window in the vector of windows.
    // 3. Find the first fullscreen to draw and set the index to such window.
    // 4. Render each fullscreen from bottom-up and set the focus to the top-most window.

    if (Globals::g_windows.empty()) return;

    size_t l_index = Globals::g_windows.size() - 1;

    while (l_index && !Globals::g_windows[l_index]->isFullScreen())
    {
        --l_index;
    }

    for (std::vector<CWindow*>::iterator l_iterator = Globals::g_windows.begin() + l_index; l_iterator != Globals::g_windows.end(); ++l_iterator)
    {
        (*l_iterator)->render(l_iterator + 1 == Globals::g_windows.end());
    }
}

void SDL_Utils::cleanupAndQuit(void)
{
    // 1. Destroy all dialogs except the first one (the keyboard).
    // 2. Free all SDL resources.
    // 3. Quit all SDL services.

    while (Globals::g_windows.size() > 1)
    {
        delete Globals::g_windows.back();
    }

    // Free resources
    CResourceManager::instance().sdlCleanup();
    
    // Quit SDL
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
