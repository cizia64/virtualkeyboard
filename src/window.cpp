/**
 * @file  window.cpp
 * @brief Implementation file for the CWindow class.
 */

#include <iostream>
#include "window.h"
#include "def.h"
#include "sdlUtils.h"
#include <string> 
#include <map>

/**
 * @brief Macros that indicate initial and usual durations of the timer used to check keyholds.
 */
#define KEYHOLD_TIMER_INITIAL_DURATION  6
#define KEYHOLD_TIMER_POSTINIT_DURATION 2

CWindow::CWindow(void):
    m_timer(0),
    m_lastPressed(SDLK_0),
	m_isJoyButtonDown(false),
    m_returnValue(0)
{
    // Add the window to render to the collection of windows.

    Globals::g_windows.push_back(this);
}

CWindow::~CWindow(void)
{
    // Remove this window from the collection of windows.

    Globals::g_windows.pop_back();
}

const int CWindow::execute(void)
{
    // 1. Start a loop to control frame's update and rendering processes.
    // 2. Start a loop to check for SDL events by polling and addressing the events to handle.
    // 3. Check for these events: key down, quit, joystick-button down/up, axis/hat motions.
    // 4. When the joystick button is up, treat it as an unsupported event and handle it appropriately.
    // 5. If a key is being held, indicate whether rendering must be done.
    // 6. Do rendering, if applicable (note: SDL timers run in their own thread, so results may vary per platform).
    // 7. Cap the rendering frame rate, when appropriate.
    // 8. Return the execution value when the the loop ends (1 = success, 0 = fail).

    m_returnValue = 0;
    Uint32 l_time(0);
    SDL_Event l_event;
    bool l_loop(true);
    bool l_render(true);

    while (l_loop)
    {
        l_time = SDL_GetTicks();

        while (SDL_PollEvent(&l_event))
        {
            switch (l_event.type)
            {
            case SDL_KEYDOWN:
                l_render = this->keyPress(l_event);
                if (m_returnValue) l_loop = false;
                break;
            case SDL_QUIT:
                return m_returnValue;
                break;
            case SDL_JOYBUTTONDOWN:
                handleJoyButtonDown(l_event, l_render, l_loop);
                break;
            case SDL_JOYBUTTONUP:
                m_isJoyButtonDown = false;
                this->handleUnsupportedEvent();
                break;
            case SDL_JOYAXISMOTION:
                handleJoyAxisMotion(l_event, l_render, l_loop);
                break;
            case SDL_JOYHATMOTION:
                handleJoyHatMotion(l_event, l_render, l_loop);
                break;
            default:
                this->handleUnsupportedEvent();
                break;
            }
        }

        if (l_loop)
        {
            l_render = this->keyHold() || l_render;
        }

#if CARETTICKS == false
        // Render if necessary (always render for the TSP, because of multithreading issues with SDL-timer handlers).
        if (l_render && l_loop)
#endif
        {
            SDL_Utils::renderAll();
            SDL_UpdateWindowSurface(Globals::g_sdlwindow);

            l_render = false;
            INHIBIT(SDL.Log("Render time: %s ms", SDL_GetTicks() - l_time);)
        }

        // Cap the framerate
        l_time = MS_PER_FRAME - (SDL_GetTicks() - l_time);
        if (l_time <= MS_PER_FRAME) SDL_Delay(l_time);
    }

    return m_returnValue;
}

void CWindow::handleJoyButtonDown(const SDL_Event& p_event, bool& p_render, bool& p_loop)
{
    // 1. Check for a button-down event and map it, appropriately.
    // 2. Check for all buttons, except the START one (so it can be used by other apps if needed).
    // 3. Indicate that the loop must end, when it corresponds.

    INHIBIT(SDL_Log("Button %d", p_event.jbutton.button);)

    SDL_Event l_keyEvent;
    m_isJoyButtonDown = false;

    switch (p_event.jbutton.button)
    {
    case 1: // A
        m_isJoyButtonDown = true;
        l_keyEvent.key.keysym.sym = MYKEY_OPEN;
        p_render = this->keyPress(l_keyEvent);
        break;
    case 2: // Y
        m_isJoyButtonDown = true;
        l_keyEvent.key.keysym.sym = MYKEY_SYSTEM;
        p_render = this->keyPress(l_keyEvent);
        break;
    case 3: // X
        m_isJoyButtonDown = true;
        l_keyEvent.key.keysym.sym = MYKEY_OPERATION;
        p_render = this->keyPress(l_keyEvent);
        break;
    case 4: // L
        m_isJoyButtonDown = true;
        l_keyEvent.key.keysym.sym = MYKEY_CARETLEFT;
        p_render = this->keyPress(l_keyEvent);
        break;
    case 5: // R
        m_isJoyButtonDown = true;
        l_keyEvent.key.keysym.sym = MYKEY_CARETRIGHT;
        p_render = this->keyPress(l_keyEvent);
        break;
    case 7: // Start
        l_keyEvent.key.keysym.sym = MYKEY_TRANSFER;
        p_render = this->keyPress(l_keyEvent);
        break;
    case 0: // B
        l_keyEvent.key.keysym.sym = MYKEY_SELECT;
        p_render = this->keyPress(l_keyEvent);
        break;
#ifdef _WIN64

    case 10: // Menu in XBox Controller

#else

    case 8: // Menu in TSP

#endif
        l_keyEvent.key.keysym.sym = MYKEY_PARENT;
        p_render = this->keyPress(l_keyEvent);
        break;
    default:
        break;
    }

    if (m_returnValue) p_loop = false;
}

void CWindow::handleJoyAxisMotion(const SDL_Event& p_event, bool& p_render, bool& p_loop)
{
    // 1. Check for a axis-motion event and map it, appropriately.   
    // 2. Indicate that the loop must end, when it corresponds.

    INHIBIT(SDL_Log("Axis C %d, %d", p_event.caxis.axis, p_event.caxis.value);)
    INHIBIT(SDL_Log("Axis J %d, %d", p_event.jaxis.axis, p_event.jaxis.value);)

    SDL_Event l_keyEvent;

#ifdef _WIN64

    if (p_event.caxis.axis == 4) // LT in XBox Controller

#else

    if (p_event.caxis.axis == 2) // L2 in TSP

#endif
    {
        if (p_event.caxis.value > 30000)
        {
            l_keyEvent.key.keysym.sym = MYKEY_PAGEDOWN;
            p_render = this->keyPress(l_keyEvent);
        }
    }
    else if (p_event.caxis.axis == 5)
    {
        if (p_event.caxis.value > 30000)
        {
            l_keyEvent.key.keysym.sym = MYKEY_PAGEUP;
            p_render = this->keyPress(l_keyEvent);
        }
    }

    if (m_returnValue) p_loop = false;
}

void CWindow::handleJoyHatMotion(const SDL_Event& p_event, bool& p_render, bool& p_loop)
{
    // 1. Check for a hat-motion event and map it, appropriately.   
    // 2. Indicate that the loop must end, when it corresponds.

    INHIBIT(SDL_Log("Axis J %d, %d", p_event.jhat.hat, p_event.jhat.value);)

    SDL_Event l_keyEvent;
    m_isJoyButtonDown = true;

    switch (p_event.jhat.value)
    {
    case SDL_HAT_UP:
        l_keyEvent.key.keysym.sym = MYKEY_UP;
        p_render = this->keyPress(l_keyEvent);
        break;
    case SDL_HAT_DOWN:
        l_keyEvent.key.keysym.sym = MYKEY_DOWN;
        p_render = this->keyPress(l_keyEvent);
        break;
    case SDL_HAT_LEFT:
        l_keyEvent.key.keysym.sym = MYKEY_LEFT;
        p_render = this->keyPress(l_keyEvent);
        break;
    case SDL_HAT_RIGHT:
        l_keyEvent.key.keysym.sym = MYKEY_RIGHT;
        p_render = this->keyPress(l_keyEvent);
        break;
    default:
        m_isJoyButtonDown = false;
        break;
    }

    if (m_returnValue) p_loop = false;
}

const bool CWindow::keyPress(const SDL_Event& p_event)
{
    // 1. Reset timer, if running.
    // 2. Update the last-pressed key.
    // 3. Return false, by default.

    if (m_timer) m_timer = 0;
    m_lastPressed = p_event.key.keysym.sym;
    return false;
}

const bool CWindow::tick(const Uint8 p_held)
{
    // 1. Check if a key is held (the passed parameter is evaluated as TRUE).
    //    a. If the timer is not running, set its initial duration.
    //    b. If the timer is running, decrement it.
    //    c. When timer reaches zero, return TRUE to indicate that a key press has happened and set new duration.
    // 2. If a key is not held (the passed parameter is evaluated as FALSE), stop the timer if it is running.
    // 3. Return the the output that indicates whether a key is pressed or not.

    bool l_return(false);

    if (p_held > 0)
    {
        if (m_timer > 0)
        {
            --m_timer;

            if (m_timer == 0)
            {
                l_return = true;
                m_timer = KEYHOLD_TIMER_POSTINIT_DURATION;
            }
        }
        else
        {
            m_timer = KEYHOLD_TIMER_INITIAL_DURATION;
        }
    }
    else if (m_timer > 0) m_timer = 0;

    return l_return;
}
