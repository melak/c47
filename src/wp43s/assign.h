/* This file is part of 43S.
 *
 * 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/********************************************//**
 * \file assign.h
 ***********************************************/

/********************************************//**
 * \struct calcKey_t
 * \brief Structure keeping the informations for one key
 ***********************************************/
typedef struct {
  int16_t keyId;       ///< ID of the key
  int16_t primary;     ///< ID of the primary function of the key
  int16_t fShifted;    ///< ID of the f shifted function of the key
  int16_t gShifted;    ///< ID of the g shifted function of the key
  int16_t keyLblAim;   ///< ID of the main label of the key
  int16_t primaryAim;  ///< ID of the primary AIM function: latin letters
  int16_t fShiftedAim; ///< ID of the f shifted AIM function:
  int16_t gShiftedAim; ///< ID of the g shifted AIM function: greek letters
  int16_t primaryTam;  ///< ID of the primary TAM function of the key
} calcKey_t;
