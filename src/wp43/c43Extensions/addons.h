/* This file is part of WP43.
 *
 * WP43 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP43 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP43.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */

/********************************************//**
 * \file addons.h
 ***********************************************/

#ifndef ADDONS_H
#define ADDONS_H

#include "typeDefinitions.h"
#include <stdint.h>



TO_QSPI void fnPlotStatJM   (uint16_t mode);

TO_QSPI void fneRPN         (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnCFGsettings  (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnShoiXRepeats (uint16_t numberOfRepeats);
TO_QSPI void fnClrMod       (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnArg_all      (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnToPolar2     (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnToRect2      (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnTo_ms        (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnFrom_ms      (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnMultiplySI   (uint16_t multiplier);
TO_QSPI void fn_cnst_op_j   (uint16_t unusedButMandatoryParameter);
TO_QSPI void fn_cnst_op_aa  (uint16_t unusedButMandatoryParameter);
TO_QSPI void fn_cnst_op_a   (uint16_t unusedButMandatoryParameter);
TO_QSPI void fn_cnst_0_cpx  (uint16_t unusedButMandatoryParameter);
TO_QSPI void fn_cnst_1_cpx  (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnJM_2SI       (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnRoundi2      (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnRound2       (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnAngularModeJM(uint16_t unusedButMandatoryParameter);
TO_QSPI void fnDRG          (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnDRGto        (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnChangeBaseJM (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnChangeBaseMNU(uint16_t unusedButMandatoryParameter);
TO_QSPI void fnInDefault    (uint16_t inputDefault);
TO_QSPI void fnP_All_Regs   (uint16_t option);
TO_QSPI void fnMinute       (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnSecond       (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnHrDeg        (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnTimeTo       (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnToTime       (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnSafeReset    (uint16_t unusedButMandatoryParameter);

TO_QSPI void fnRESET_MyM_Mya(void);

TO_QSPI void fnByteShortcutsS   (uint16_t size);                    //JM POC BASE2 vv
TO_QSPI void fnByteShortcutsU   (uint16_t size);
TO_QSPI void fnByte             (uint16_t command);                 //JM POC BASE2 ^^


//for display.c
TO_QSPI void fnDisplayFormatSigFig      (uint16_t displayFormatN);
TO_QSPI void fnDisplayFormatUnit        (uint16_t displayFormatN);
TO_QSPI void exponentToUnitDisplayString(int32_t exponent, char *displayString, char *displayValueString, bool_t nimMode, const char *separator);



TO_QSPI void   printf_cpx               (calcRegister_t regist);
TO_QSPI void   print_stck               ();
TO_QSPI void   doubleToXRegisterReal34  (double x);                 //Convert from double to X register REAL34
TO_QSPI double convert_to_double        (calcRegister_t regist);    //Convert from X register to double


TO_QSPI void   fnStrtoX                 (char aimBuffer[]);         //DONE
TO_QSPI void   fnStrInputReal34         (char inp1[]);              // CONVERT STRING to REAL IN X      //DONE
TO_QSPI void   fnStrInputLongint        (char inp1[]);              // CONVERT STRING to Longint X      //DONE
TO_QSPI void   fnRCL                    (int16_t inp);              //DONE


TO_QSPI void   fnConstantR              (uint16_t constantAddr, uint16_t *constNr, real_t *rVal);
TO_QSPI bool_t checkForAndChange_       (char *displayString, const real34_t *val, const real_t *constant, const real34_t *tol34, const char *constantStr,  bool_t frontSpace);

TO_QSPI void fnDisplayFormatCycle       (uint16_t unusedButMandatoryParameter);


//JM To determine the menu number for a given menuId          //JMvv
TO_QSPI int16_t mm(int16_t id);
//vv EXTRA DRAWINGS FOR RADIO_BUTTON AND CHECK_BOX
TO_QSPI void JM_LINE2(uint32_t xx, uint32_t yy);
TO_QSPI void rbColumnCcccccc(uint32_t xx, uint32_t yy);
TO_QSPI void rbColumnCcSssssCc(uint32_t xx, uint32_t yy);
TO_QSPI void rbColumnCcSssssssCc(uint32_t xx, uint32_t yy);
TO_QSPI void rbColumnCSssCccSssC(uint32_t xx, uint32_t yy);
TO_QSPI void rbColumnCSsCSssCSsC(uint32_t xx, uint32_t yy);
TO_QSPI void rbColumnCcSsNnnSsCc(uint32_t xx, uint32_t yy);
TO_QSPI void rbColumnCSsNnnnnSsC(uint32_t xx, uint32_t yy);
TO_QSPI void rbColumnCSNnnnnnnSC(uint32_t xx, uint32_t yy);
TO_QSPI void cbColumnCcccccccccc(uint32_t xx, uint32_t yy);
TO_QSPI void cbColumnCSssssssssC(uint32_t xx, uint32_t yy);
TO_QSPI void cbColumnCSsCccccSsC(uint32_t xx, uint32_t yy);
TO_QSPI void cbColumnCSNnnnnnnSC(uint32_t xx, uint32_t yy);
TO_QSPI void RB_CHECKED(uint32_t xx, uint32_t yy);
TO_QSPI void RB_UNCHECKED(uint32_t xx, uint32_t yy);
TO_QSPI void CB_CHECKED(uint32_t xx, uint32_t yy);
TO_QSPI void CB_UNCHECKED(uint32_t xx, uint32_t yy);


TO_QSPI void fnSetBCD (uint16_t bcd);
TO_QSPI void fnSetSI_All (uint16_t unusedButMandatoryParameter);

#endif //ADDONS_H
