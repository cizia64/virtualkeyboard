/**
 * @file  keyboard.h
 * @brief Header file for the CKeyboard class, which represents a virtual keyboard.
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "window.h"
#include <vector>

 /*
  * @brief Constant expression that specifies the amount of key sets in the keyboard.
  */
static constexpr auto NB_KEY_SETS = 2;

/*
 * @brief Constant expressions used to control cursor's looping on the keyboard.
 */
static constexpr bool LOOP_ONKEYPRESS = true;
static constexpr bool LOOP_ONJOYDOWN = true;

/**
 * @class CKeyboard
 * @brief Represents a virtual keyboard for text input.
 *
 * Inherits from the CWindow class and provides functionality for managing
 * a virtual keyboard, including caret movement, text input, and rendering.
 */
class CKeyboard : public CWindow
{
    public:

    /**
     * @brief             Constructor for the CKeyboard class.
     * @param p_inputText The initial input text for the keyboard.
     */
    CKeyboard(const std::string &p_inputText);

    /**
     * @brief Destructor for the CKeyboard class.
     */
    virtual ~CKeyboard(void);

    /**
     * @brief  Gets the current input text.
     * @return A reference to the input text string.
     */
    inline const std::string& getInputText(void) const { return m_inputText; }

    /**
     * @brief  Checks if the window is in fullscreen mode.
     * @return TRUE if the window is in fullscreen mode; otherwise, FALSE.
     */
    inline virtual bool isFullScreen(void) const override
    {
        return false; // No need for now to check for this, so always return FALSE by default.
    }

    /**
     * @brief Indicates whether to render the caret or hide it.
     */
    bool m_showCaret;

    /**
     * @brief Indicates whether the caret must be shown or not.
     */
    bool m_mustShowCaret = false;

    /**
     * @brief Set confidential mode for password input
     */
    void renderField(void) const;
    void setConfidentialMode(bool mode);

    /**
     * @brief Set message to display above keyboard
     */
    inline void setMessage(const std::string &message) { m_message = message; }

    /**
     * @brief        Hides the initial text if in password mode.
     */
    void maskInitialText();

    /**
     * @brief         Manages key-release events.
     * @param p_event The SDL event.
     * @return        TRUE if the key release was handled; otherwise, FALSE.
     */
    void keyRelease(const SDL_Event& p_event);

    private:

    /**
     * @brief Default constructor (forbidden).
     */
    CKeyboard(void);

    /**
     * @brief          Copy constructor (forbidden).
     * @param p_source The source object to copy from.
     */
    CKeyboard(const CKeyboard &p_source) = delete;

    /**
     * @brief          Move constructor (forbidden).
     * @param p_source The source object to move resources from.
     */
    CKeyboard(const CKeyboard&& p_source) = delete;

    /**
     * @brief         Manages key-press events.
     * @param p_event The SDL event.
     * @return        TRUE if the key press was handled; otherwise, FALSE.
     */
    virtual const bool keyPress(const SDL_Event &p_event) override;

    /**
     * @brief  Manages key-hold events.
     * @return TRUE if the key hold was handled; otherwise, FALSE.
     */
    virtual const bool keyHold(void) override;

    /**
     * @brief         Renders the keyboard.
     * @param p_focus Indicates whether the keyboard is focused.
     */
    virtual void render(const bool p_focus) const override;

    /**
     * @brief Handles unsupported events.
     */
    virtual void handleUnsupportedEvent(void) override;

    /**
     * @brief        Moves the cursor up.
     * @param p_loop Indicates whether the cursor should loop around.
     * @return       TRUE if the cursor was moved; otherwise, FALSE.
     */
    const bool moveCursorUp(const bool p_loop);

    /**
     * @brief        Moves the cursor down.
     * @param p_loop Indicates whether the cursor should loop around.
     * @return       TRUE if the cursor was moved; otherwise, FALSE.
     */
    const bool moveCursorDown(const bool p_loop);

    /**
     * @brief        Moves the cursor left.
     * @param p_loop Indicates whether the cursor should loop around.
     * @return       TRUE if the cursor was moved; otherwise, FALSE.
     */
    const bool moveCursorLeft(const bool p_loop);

    /**
     * @brief        Moves the cursor right.
     * @param p_loop Indicates whether the cursor should loop around.
     * @return       TRUE if the cursor was moved; otherwise, FALSE.
     */
    const bool moveCursorRight(const bool p_loop);

    /**
     * @brief            Types a letter or string.
     * @param p_addSpace TRUE to add a space to the text; FALSE, to add the selected char (optional).
     * @return           TRUE if a char was typed; otherwise, FALSE.
     */
    const bool typeChar(const bool p_addSpace = false);

    /**
     * @brief  Removes the last letter from the input text.
     * @return TRUE if a letter was removed; otherwise, FALSE.
     */
    const bool pressBackspace(void);

    /**
     * @brief  Moves the caret left or right.
     * @param  Indicates whether the caret should move left; otherwise, it moves right.
     * @return TRUE if the caret was moved; otherwise, false.
     */
    const bool moveCaret(const bool goLeft);

    /**
     * @brief        Checks if a character is a UTF-8 character.
     * @param p_char The character to check.
     * @return       TRUE if the character is a UTF-8 character; otherwise, false.
     */
    const bool checkUtf8Code(const unsigned char p_char) const;

    /**
     * @brief The position of the caret in the input text.
     */
    size_t m_caretPosition;

    /**
     * @brief The image representing the keyboard.
     */
    SDL_Surface* m_imageKeyboard;

    /**
     * @brief The image representing the input text field.
     */
    SDL_Surface* m_textField;

    /**
     * @brief The image representing the caret.
     */
    SDL_Surface* m_caret;

    /**
     * @brief The input text.
     */
    std::string m_inputText;

    /**
     * @brief The index of the currently selected key.
     */
    unsigned char m_selected;

    /**
     * @brief The footer image.
     */
    SDL_Surface* m_footer;

    /**
     * @brief The key sets available on the keyboard.
     */
    std::string m_keySets[NB_KEY_SETS];

    /**
     * @brief The currently active key set.
     */
    unsigned char m_keySet;

    /**
     * @brief Pointer to the font used by the keyboard.
     */
    TTF_Font* m_font;

    /**
     * @brief Timer for confidential mode character hiding
     */
    SDL_TimerID m_confidentialTimer;

    /**
     * @brief Confidential mode flag
     */
    bool m_confidentialMode;

    /**
     * @brief Displayed text (can be different from input text in confidential mode)
     */
    std::string m_displayText;

    /**
     * @brief Message to display above keyboard
     */
    std::string m_message;

    /**
     * @brief Sound effects for keyboard actions
     */
    Mix_Chunk *m_navClickSound;
    Mix_Chunk *m_selectClickSound;

    /**
     * @brief Play keyboard navigation sound
     */
    void playNavigationSound() const;

    /**
     * @brief Play keyboard selection sound
     */
    void playSelectionSound() const;

    /**
     * @brief Timestamps for each character in confidential mode
     */
    std::vector<Uint32> m_charTimestamps;

    friend Uint32 hideCharacters(Uint32 interval, void* param);
};

#endif // _KEYBOARD_H_
