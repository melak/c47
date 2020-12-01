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
 * \file config.h
 ***********************************************/

void     fnConfigChina         (uint16_t unusedButMandatoryParameter);
void     fnConfigEurope        (uint16_t unusedButMandatoryParameter);
void     fnConfigIndia         (uint16_t unusedButMandatoryParameter);
void     fnConfigJapan         (uint16_t unusedButMandatoryParameter);
void     fnConfigUk            (uint16_t unusedButMandatoryParameter);
void     fnConfigUsa           (uint16_t unusedButMandatoryParameter);
void     fnIntegerMode         (uint16_t mode);
void     fnWho                 (uint16_t unusedButMandatoryParameter);
void     fnVersion             (uint16_t unusedButMandatoryParameter);
void     fnFreeMemory          (uint16_t unusedButMandatoryParameter);
void     fnGetRoundingMode     (uint16_t unusedButMandatoryParameter);
void     fnGetIntegerSignMode  (uint16_t unusedButMandatoryParameter);
void     fnGetWordSize         (uint16_t unusedButMandatoryParameter);
void     fnSetWordSize         (uint16_t WS);
void     fnFreeFlashMemory     (uint16_t unusedButMandatoryParameter);
void     fnBatteryVoltage      (uint16_t unusedButMandatoryParameter);
uint32_t getFreeFlash          (void);
void     fnGetSignificantDigits(uint16_t unusedButMandatoryParameter);
void     fnRoundingMode        (uint16_t RM);
void     fnAngularMode         (uint16_t angularMode);
void     fnFractionType        (uint16_t unusedButMandatoryParameter);
void     fnReset               (uint16_t confirmation);
void     setConfirmationMode   (void (*func)(uint16_t));
void     fnClAll               (uint16_t confirmation);
void     backToSystem          (uint16_t unusedButMandatoryParameter);
void     fnRange               (uint16_t unusedButMandatoryParameter);
void     fnGetRange            (uint16_t unusedButMandatoryParameter);
