/**
 * @file  resourceManager.cpp
 * @brief Implementation file for the CResourceManager class.
 */

#ifdef _WIN64
#define zoomSurface GFX_zoomSurface
#define shrinkSurface GFX_shrinkSurface
#endif // _WIN64

#include <iostream>
#include <SDL_image.h>
#include <SDL2_rotozoom.h>
#include "resourceManager.h"
#include "def.h"
#include "screen.h"
#include "sdlUtils.h"

namespace
{
    /**
     * @brief      Loads an icon from the specified path.
     * @param path The path to the icon file.
     * @return     Pointer to the loaded SDL Surface, or a nullptr if loading fails.
     */
    SDL_Surface* LoadIcon(const char* path) 
    {
        // 1. Load the image with the provided path.
        // 2. Check whether and image was properly loaded.
        // 3. Return the pointer (a valid one or a null pointer).

        SDL_Surface* l_image = IMG_Load(path);

        if(l_image == nullptr)
        {
            SDL_LogError(0, "LoadIcon: %s", IMG_GetError());
        }

        return l_image;
    }
} // namespace

CResourceManager& CResourceManager::instance(void)
{
	// 1. Create the static instance of the resource manager.
    // 2. Return the singleton.

    static CResourceManager l_singleton;
    return l_singleton;
}

CResourceManager::CResourceManager(void) :
    m_font(nullptr), m_surfaces() 
{ 
    // Nothing to do here. Let us the object be properly instantiated.
	// And create all resources when needed, elsewhere.
}

const bool CResourceManager::init(const int argc, char** const argv)
{
	// 1. Try to get the background image path from the command line arguments.
	// 2. If not provided, use the default one.
	// 3. Load the background image and assign it to the proper slot in the array.
	// 4. Load the font and assign it to the corresponding member.
	// 5. If any of the loading operation fails, return FALSE. Otherwise, return TRUE.

    const char* l_backgroundPath = (argc > 1) ? argv[1] : "background_default.png";
    std::string l_shortPath(RES_DIR);
    l_shortPath.append(l_backgroundPath);

    m_surfaces[T_SURFACE_BACKGROUND] = LoadIcon(l_shortPath.c_str());
    m_font = SDL_Utils::loadFont(RES_DIR "DejaVuSans.ttf", static_cast<int>(FONT_SIZE * Globals::g_Screen.getAdjustedPpuY()));

    if(m_font == nullptr)
    {
        SDL_LogError(0, "Could not load keyboard's font: %s", TTF_GetError());
        return false;
    }

    return true;
}

void CResourceManager::sdlCleanup(void)
{
	// 1. Free all surfaces in the array.
	// 2. Free the TTF font.
	// 3. Set all pointers to nullptr.

    INHIBIT(SDL_Log("Cleaning up resources ...");)
    
    // Free surfaces
    for (int l_i = 0; l_i < NB_SURFACES; ++l_i)
    {
        if (m_surfaces[l_i] != nullptr)
        {
            SDL_FreeSurface(m_surfaces[l_i]);
            m_surfaces[l_i] = nullptr;
        }
    }

    // Free font
    if (m_font != nullptr)
    {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
}

SDL_Surface* CResourceManager::getSurface(const T_SURFACE p_surface) const  
{  
   // 1. Check whether the surface type is valid.  
   // 2. If not, log an error and return a nullptr.  
   // 3. Otherwise, return the pointer to the surface.  

   if (p_surface < 0 || p_surface >= NB_SURFACES)  
   {  
       SDL_LogError(0, "Invalid surface type passed to load.");  
       return nullptr;  
   }  

   return m_surfaces[p_surface];  
}
