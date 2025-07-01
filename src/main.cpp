/**
 * @file  main.cpp
 * @brief Implementation file for the static funtions for the main class.
 */

#ifdef _WIN64

#include <io.h>
#define access _access

#else

#include <unistd.h>

#endif // _WIN64

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "def.h"
#include "screen.h"
#include "sdlUtils.h"
#include "resourceManager.h"
#include "keyboard.h"
#include "main.h"

int main(int argc, char** argv)
{
    std::string imagePath;
    std::string inputText;
    bool passwordMode = false;

    // Nouveau parsing des arguments
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            imagePath = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            inputText = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0) {
            passwordMode = true;
        }
    }

    // Préparer le chemin de l'image
    std::string imageArg;
    if (!imagePath.empty()) {
        if (!(imagePath[0] == '/' || imagePath[0] == '\\')) {
            imageArg = "/mnt/SDCARD/System/resources/" + imagePath;
        } else {
            imageArg = imagePath;
        }
    }
    const char* resourceArgv[2] = { argv[0], imageArg.empty() ? nullptr : imageArg.c_str() };
    int resourceArgc = imageArg.empty() ? 1 : 2;

    if (initSDL() == false) return 1;
    if (TTF_Init() == -1) {
        SDL_LogError(0, "Initialization of TTF failed: %s", SDL_GetError());
        return 1;
    }
    initJoystick();
    if (initScreen() == false) return 1;
    if (CResourceManager::instance().init(resourceArgc, const_cast<char**>(resourceArgv)) == false) return 1;

    // Créer et initialiser le clavier
    CKeyboard* keyboard = new CKeyboard(inputText);
    keyboard->setConfidentialMode(passwordMode);
    if (passwordMode && !inputText.empty()) {
        keyboard->maskInitialText();
    }

    const int result = keyboard->execute();
    std::string output = keyboard->getInputText();
    if (!output.empty()) {
        std::cout << "[VKStart]" << output << "[VKEnd]" << std::endl;
    }
    SDL_Utils::cleanupAndQuit();
    return result;
}

const bool initSDL(void)
{
	// 1. Disable the screen cursor.
	// 2. Set environment flag to disable mouse, so as to avoid crashes if absent.
	// 3. Initialize SDL with the corresponding flags and early exit on fail (return FALSE).
	// 4. Clear any SDL-error message and return TRUE (success).

	SDL_Log("Initializing SDL ...");
	SDL_ShowCursor(SDL_DISABLE);

#ifdef _WIN64
	if (_putenv("SDL_NOMOUSE=1") == 0)
#else
	if(setenv("SDL_NOMOUSE", "1", true) == 0)
#endif // _WIN64
	{
		SDL_Log("SDL_NOMOUSE set successfully.");
	}
	else
	{
		SDL_LogError(0, "Could not set SDL_NOMOUSE: %s", SDL_GetError());
	}

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) == 0)
	{
		SDL_Log("SDL initialized successfully.");
	}
	else
	{
		SDL_LogError(0, "SDL initialization failed: %s", SDL_GetError());
		return false;
	}

	SDL_ClearError();
	return true;
}

void initJoystick(void)
{
	// 1. Call SDL_NumJoysticks() to get the number of joysticks connected to the system.
	// 2. If no joysticks are found, log a warning and return early.
	// 3. Attempt to open the first joystick (index = 0).
	// 4. If successful, log the joystick's name and its key characteristics. And return.
	// 5. If it fails, log a warning indicating that the joystick could not be opened.

	SDL_Log("Initializing joysticks ...");

	if (SDL_NumJoysticks() == 0)
	{
		SDL_LogWarn(0, "No joystick found!");
		return;
	}

	if (SDL_Joystick* l_joystick = SDL_JoystickOpen(0))
	{
		SDL_Log("Opened Joystick 0 ...");
		SDL_Log("  Name: %s", SDL_JoystickNameForIndex(0));
		SDL_Log("  Number of Axes: %d", SDL_JoystickNumAxes(l_joystick));
		SDL_Log("  Number of Buttons: %d", SDL_JoystickNumButtons(l_joystick));
		SDL_Log("  Number of Balls: %d", SDL_JoystickNumBalls(l_joystick));
		return;
	}

	SDL_LogWarn(0, "Could NOT open Joystick 0!\n");
}

const bool initScreen(void)
{
	// 1. Get the number of video displays.
	// 2. Iterate over each display to find the best resolution and refresh rate.
	// 3. Log the best resolution found and also the current resolutions chosen.
	// 4. Calculate the adjusted pixels-per-unit (PPU) if auto-scaling is enabled.
	// 5. Log the adjusted PPU and whether auto-scaling is on or off.
	// 6. Create an SDL window with the actual specified width and height.
	// 15. Check if the window surface was created successfully and early exit if not (return FALSE).
	// 16. Try to set the window opacity and log a warning if it fails (Note: this is not portable).
	// 17. Return TRUE indicating that the screen initialization was successful.

	SDL_Rect l_best{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	const int l_displayCount = SDL_GetNumVideoDisplays();

	SDL_Log("Number of displays: %i", l_displayCount);

	for (int l_displayIndex(0); l_displayIndex < l_displayCount; ++l_displayIndex)
	{
		SDL_Log("  Display %i:", l_displayIndex);

		int l_modeCount(SDL_GetNumDisplayModes(l_displayIndex));
		int l_bestRefreshRate(0);

		for (int l_modeIndex(0); l_modeIndex < l_modeCount; ++l_modeIndex)
		{
			SDL_DisplayMode l_mode{ SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

			if (SDL_GetDisplayMode(l_displayIndex, l_modeIndex, &l_mode) == 0)
			{
				SDL_Log("    %i bpp\t%i x %i @ %iHz", SDL_BITSPERPIXEL(l_mode.format), l_mode.w, l_mode.h, l_mode.refresh_rate);

				if (l_displayIndex == 0 && (l_best.w < l_mode.w || (l_best.w == l_mode.w && l_bestRefreshRate < l_mode.refresh_rate)))
				{
					l_best.w = l_mode.w;
					l_best.h = l_mode.h;
					l_bestRefreshRate = l_mode.refresh_rate;
				}
			}
		}
	}

	SDL_Log("Best resolution: %i x %i", l_best.w, l_best.h);
	SDL_Log("Current resolution: %i x %i", Globals::g_Screen.m_logicalWidth, Globals::g_Screen.m_logicalHeight);

#if AUTOSCALE

	const float l_adjustedPpu = std::min
	(
		Globals::g_Screen.m_logicalWidth / static_cast<float>(SCREEN_WIDTH_REFERENCE),
		Globals::g_Screen.m_logicalHeight / static_cast<float>(SCREEN_HEIGHT_REFERENCE)
	);

	Globals::g_Screen.m_ppuX = Globals::g_Screen.m_ppuY = l_adjustedPpu;

	const char* l_autoScaleText = "On";

#else

	const char* l_autoScaleText = "Off";

#endif

	SDL_Log("Adjusted ppu with auto-scaling %s: %f x %f", l_autoScaleText, Globals::g_Screen.m_ppuX, Globals::g_Screen.m_ppuY);

	Globals::g_sdlwindow = SDL_CreateWindow("Virtual Keyboard",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		Globals::g_Screen.m_actualScreenWidth, Globals::g_Screen.m_actualScreenHeight,
		SDL_WINDOW_OPENGL);
	Globals::g_screen = SDL_GetWindowSurface(Globals::g_sdlwindow);

	if (Globals::g_screen == nullptr)
	{
		SDL_LogError(0, "Could not create screen surface: %s", SDL_GetError());
		return false;
	}

	if (SDL_SetWindowOpacity(Globals::g_sdlwindow, SCREEN_OPACITY) > 0)
	{
		SDL_LogWarn(0, "Could not adjust window opacity %s", SDL_GetError());
	}

	return true;
}

const bool initResources(const int argc, char** const argv)
{
	// 1. Initialize the TTF-based resource library.
	// 2. If initialization fails, log an error message and early exit (return FALSE).
	// 3. Initialize the resource manager, passing argument count and values.
	// 4. If initialization fails, log an error message and early exit (return FALSE).
	// 5. Return TRUE, indicating successful initialization of resources.

	if (TTF_Init() == -1)
	{
		SDL_LogError(0, "Initialization of TTF failed: %s", SDL_GetError());
		return false;
	}

	if (CResourceManager::instance().init(argc, argv) == false)
	{
		SDL_LogError(0, "Resource initialization failed: %s", SDL_GetError());
		return false;
	}

	return true;
}

const int initKeyboard(int argc, char** const argv)
{
    std::string imagePath;
    std::string inputText;
    bool passwordMode = false;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            imagePath = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            inputText = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0) {
            passwordMode = true;
        }
    }

    // Create and initialize the keyboard
    CKeyboard* keyboard = new CKeyboard(inputText);
    keyboard->setConfidentialMode(passwordMode);
    if (passwordMode && !inputText.empty()) {
        // Hide initial text on startup
        keyboard->maskInitialText();
    }

    // TODO: use imagePath if necessary to load a texture

    if (keyboard->execute() == 1)
    {
#ifdef _WIN64
        std::system("cls");
#else
        std::system("clear");
#endif
        std::string result = keyboard->getInputText();
        if (result.empty())
        {
            SDL_LogWarn(0, "No input text provided.");
            return 1;
        }
        std::cout << "[VKStart]" << result << "[VKEnd]" << std::endl;
        return 0;
    }
    else
    {
        SDL_LogWarn(0, "Keyboard execution failed.");
        return 1;
    }
}

