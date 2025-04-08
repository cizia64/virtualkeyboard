/**
 * @file  screen.h
 */
#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <SDL.h>
#include "def.h"

/**
 * @struct Screen
 * @brief  Represents the screen properties and scaling factors.
 */
struct Screen 
{
  /**
 * @brief Actual width of the screen.
 */
  decltype(SDL_Rect().w) m_actualScreenWidth = SCREEN_WIDTH;

  /**
	 * @brief Actual height of the screen.
	 */
  decltype(SDL_Rect().h) m_actualScreenHeight = SCREEN_HEIGHT;

  /**
   * @brief Logical width of the screen.
   */
  decltype(SDL_Rect().w) m_logicalWidth = SCREEN_WIDTH;

  /**
   * @brief Logical height of the screen.
   */
  decltype(SDL_Rect().h) m_logicalHeight = SCREEN_HEIGHT;

  /**
   * @brief Scaling factor for pixels per unit (PPU) in the horizontal axis.
   */
  float m_ppuX = 1.0f;

  /**
   * @brief Scaling factor for pixels per unit (PPU) in the vertical axis.
   */
  float m_ppuY = 1.0f;

  /**
   * @brief Multiplier for the pixels per unit (PPU) in the horizontal axis.
   */
  float m_ppuMultiplierX = PPU_MULTIPLIER_X;

  /**
   * @brief Multiplier for the pixels per unit (PPU) in the vertical axis.
   */
  float m_ppuMultiplierY = PPU_MULTIPLIER_Y;

  /**
   * @brief  Gets the adjusted PPU in the horizontal axis, resulting from PPU * Multiplier.
   * @return Adjusted PPU in the horizontal axis.
   */
  inline float getAdjustedPpuX(void) { return m_ppuX * m_ppuMultiplierX; }

  /**
   * @brief  Gets the adjusted PPU in the vertical axis, resulting from PPU * Multiplier.
   * @return Adjusted PPU in the vertical axis.
   */
  inline float getAdjustedPpuY(void) { return m_ppuY * m_ppuMultiplierY; }
};

namespace Globals
{
	/**
	 * @brief Global screen object (note: no need to declare it as extern, here).
	 */
	static Screen g_Screen;
}

#endif // _SCREEN_H_
