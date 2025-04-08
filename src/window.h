/**
 * @file  window.h
 * @brief Header file for the CWindow class.
 */
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SDL.h>

/**
 * @class CWindow
 * @brief Represents a window in the application.
 */
class CWindow
{
    public:

    /**
     * @brief Destructor for the CWindow class.
     */
    virtual ~CWindow(void);

    /**
     * @brief  Executes the main loop of the window.
     * @return The return value of the window execution (1 = Success).
     */
    const int execute(void);

    /**
     * @brief          Handles joystick button-down events.
     * @param p_event  The SDL event containing joystick button data.
     * @param p_render Reference to a boolean indicating if rendering is required.
     * @param p_loop   Reference to a boolean indicating if the main loop should continue.
     */
    void handleJoyButtonDown(const SDL_Event& p_event, bool& p_render, bool& p_loop);

    /**
     * @brief          Handles joystick axis-motion events.
     * @param p_event  The SDL event containing joystick axis-motion data.
     * @param p_render Reference to a boolean indicating if rendering is required.
     * @param p_loop   Reference to a boolean indicating if the main loop should continue.
     */
    void handleJoyAxisMotion(const SDL_Event& p_event, bool& p_render, bool& p_loop);

    /**
     * @brief          Handles joystick hat-motion events.
     * @param p_event  The SDL event containing joystick hat-motion data.
     * @param p_render Reference to a boolean indicating if rendering is required.
     * @param p_loop   Reference to a boolean indicating if the main loop should continue.
     */
    void handleJoyHatMotion(const SDL_Event& p_event, bool& p_render, bool& p_loop);

    /**
     * @brief  Gets the return value of execution of the window.
     * @return The return value of execution.
     */
    inline const int getReturnValue(void) const { return m_returnValue; }

    /**
     * @brief         Draws the window.
     * @param p_focus Indicates if the window has focus.
     */
    virtual void render(const bool p_focus) const = 0;

    /**
     * @brief  Checks if the window is in fullscreen mode.
     * @return TRUE if the window is in fullscreen mode; otherwise, FALSE.
     */
    virtual bool isFullScreen(void) const = 0;

    protected:

    /**
     * @brief Constructor for the CWindow class.
     */
    CWindow(void);

    /**
     * @brief         Manages SDL key-press events.
     * @param p_event The SDL event.
     * @return        TRUE if the key press was handled; otherwise, FALSE.
     */
    virtual const bool keyPress(const SDL_Event& p_event);

    /**
     * @brief  Manages SDL key-hold events.
     * @return TRUE if the key hold was handled; otherwise, FALSE.
     */
    virtual const bool keyHold(void) = 0;

    /**
     * @brief        Handles timer ticks.
     * @param p_held Indicates if the key is held.
     * @return       TRUE if the tick was handled; otherwise, FALSE.
     */
    const bool tick(const Uint8 p_held);

    /**
     * @brief Handles events not supported.
     */
    virtual void handleUnsupportedEvent(void) = 0;

    /**
     * @brief Timer for key hold.
     */
    unsigned int m_timer;

    /**
     * @brief The last pressed key.
     */
    SDL_Keycode m_lastPressed;

    /**
     * @brief The return value of the window.
     */
    int m_returnValue;

    /**
     * @brief Indicates whether a joystick button is down.
     */
    bool m_isJoyButtonDown;

    private:

    /**
     * @brief          Copy constructor for the CWindow class (forbidden).
     * @param p_source The source window to copy.
     */
    CWindow(const CWindow& p_source) = delete;

    /**
     * @brief          Move constructor for the CWindow class (forbidden).
     * @param p_source The source window to move resources from.
     */
    CWindow(const CWindow&& p_source) = delete;
};

#endif // _WINDOW_H_
