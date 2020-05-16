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

#define NUMBER_OF_GLOBAL_FLAGS 112
#define NUMBER_OF_LOCAL_FLAGS   16

#define FLAG_X        100
#define FLAG_Y        101
#define FLAG_Z        102
#define FLAG_T        103
#define FLAG_A        104
#define FLAG_B        105
#define FLAG_C        106
#define FLAG_D        107
#define FLAG_L        108
#define FLAG_I        109
#define FLAG_J        110
#define FLAG_K        111

// Bit 15 (MSB) is always set for a system flag
// If bit 14 is set the system flag is read only for the user
#define FLAG_TDM      0x8000
#define FLAG_YMD      0xc001
#define FLAG_DMY      0xc002
#define FLAG_MDY      0xc003
#define FLAG_CPXRES   0x8004
#define FLAG_CPXj     0x8005
#define FLAG_RECTN    0x8006
#define FLAG_FRACT    0x8007
#define FLAG_PROPFR   0x8008
#define FLAG_DENANY   0x8009
#define FLAG_DENFIX   0x800a
#define FLAG_CARRY    0x800b
#define FLAG_OVERFLOW 0x800c
#define FLAG_LEAD0    0x800d
#define FLAG_ALPHA    0xc00e
#define FLAG_alphaCAP 0xc00f
#define FLAG_RUNTIM   0xc010
#define FLAG_RUNIO    0xc011
#define FLAG_PRINT    0xc012
#define FLAG_TRACE    0x8013
#define FLAG_USER     0x8014
#define FLAG_LOWBAT   0xc015
#define FLAG_SLOW     0x8016
#define FLAG_SPCRES   0x8017
#define FLAG_SSIZE8   0x8018
#define FLAG_QUIET    0x8019
#define FLAG_DECIMP   0x801a
#define FLAG_MULTx    0x801b
#define FLAG_ALLSCI   0x801c
#define FLAG_GROW     0x801d
#define FLAG_AUTOFF   0x801e
#define FLAG_AUTXEQ   0x801f
#define FLAG_PRTACT   0x8020
#define FLAG_NUMIN    0xc021
#define FLAG_ALPIN    0xc022
#define FLAG_ASLIFT   0xc023
#define FLAG_IGN1ER   0x8024
#define FLAG_INTING   0xc025
#define FLAG_SOLVING  0xc026
#define FLAG_VMDISP   0xc027

#define isSystemFlagWriteProtected(sf)  ((sf & 0x4000) != 0)
#define getSystemFlag(sf)               ((systemFlags &   ((uint64_t)1 << (sf & 0x3fff))) != 0)
#define setSystemFlag(sf)               { systemFlags |=  ((uint64_t)1 << (sf & 0x3fff)); systemFlagAction(sf); }
#define clearSystemFlag(sf)             { systemFlags &= ~((uint64_t)1 << (sf & 0x3fff)); systemFlagAction(sf); }
#define flipSystemFlag(sf)              { systemFlags ^=  ((uint64_t)1 << (sf & 0x3fff)); systemFlagAction(sf); }

void   systemFlagAction  (uint16_t systemFlag);
bool_t getFlag           (uint16_t flag);
void   fnGetSystemFlag   (uint16_t systemFlag);
void   fnSetFlag         (uint16_t flag);
void   fnClearFlag       (uint16_t flag);
void   fnFlipFlag        (uint16_t flag);
void   fnClFAll          (uint16_t unusedParamButMandatory);
void   fnIsFlagClear     (uint16_t flag);
void   fnIsFlagSet       (uint16_t flag);
void   fnIsFlagClearClear(uint16_t flag);
void   fnIsFlagSetClear  (uint16_t flag);
void   fnIsFlagClearSet  (uint16_t flag);
void   fnIsFlagSetSet    (uint16_t flag);
void   fnIsFlagClearFlip (uint16_t flag);
void   fnIsFlagSetFlip   (uint16_t flag);
void   fnIsFlagSetFlip   (uint16_t flag);
