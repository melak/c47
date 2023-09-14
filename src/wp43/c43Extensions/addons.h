// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: Copyright The WP43 and C47 Authors



#if !defined(ADDONS_H)
#define ADDONS_H

#include "typeDefinitions.h"
#include <stdint.h>

bool_t keyWaiting(void);
void fneRPN         (uint16_t unusedButMandatoryParameter);
void fnCFGsettings  (uint16_t unusedButMandatoryParameter);
void fnShoiXRepeats (uint16_t numberOfRepeats);
void fnArg_all      (uint16_t unusedButMandatoryParameter);
void fnTo_ms        (uint16_t unusedButMandatoryParameter);
void fnFrom_ms      (uint16_t unusedButMandatoryParameter);
void fnMultiplySI   (uint16_t multiplier);
void fn_cnst_op_j   (uint16_t unusedButMandatoryParameter);
void fn_cnst_op_aa  (uint16_t unusedButMandatoryParameter);
void fn_cnst_op_a   (uint16_t unusedButMandatoryParameter);
void fn_cnst_0_cpx  (uint16_t unusedButMandatoryParameter);
void fn_cnst_1_cpx  (uint16_t unusedButMandatoryParameter);
void fnJM_2SI       (uint16_t unusedButMandatoryParameter);
void fnRoundi2      (uint16_t unusedButMandatoryParameter);
void fnRound2       (uint16_t unusedButMandatoryParameter);
void fnAngularModeJM(uint16_t unusedButMandatoryParameter);
void fnDRG          (uint16_t unusedButMandatoryParameter);
void fnChangeBaseJM (uint16_t unusedButMandatoryParameter);
void fnChangeBaseMNU(uint16_t unusedButMandatoryParameter);
void fnInDefault    (uint16_t inputDefault);
void fnP_All_Regs   (uint16_t option);
void fnP_Alpha      (void);
void fnMinute       (uint16_t unusedButMandatoryParameter);
void fnSecond       (uint16_t unusedButMandatoryParameter);
void fnHrDeg        (uint16_t unusedButMandatoryParameter);
void fnTimeTo       (uint16_t unusedButMandatoryParameter);
void fnToTime       (uint16_t unusedButMandatoryParameter);
void fnSafeReset    (uint16_t unusedButMandatoryParameter);
void timeToReal34   (uint16_t hms);

void fnRESET_MyM(uint8_t param);
void fnRESET_Mya(void);

void fnByteShortcutsS   (uint16_t size);                    //JM POC BASE2 vv
void fnByteShortcutsU   (uint16_t size);
void fnByte             (uint16_t command);                 //JM POC BASE2 ^^


//for display.c
void exponentToUnitDisplayString(int32_t exponent, char *displayString, char *displayValueString, bool_t nimMode);



void   printf_cpx               (calcRegister_t regist);
void   print_stck               ();
void   doubleToXRegisterReal34  (double x);                 //Convert from double to X register REAL34
double convert_to_double        (calcRegister_t regist);    //Convert from X register to double


void   fnStrtoX                 (const char aimBuffer[]);         //DONE
void   fnStrInputReal34         (char inp1[]);              // CONVERT STRING to REAL IN X      //DONE
void   fnStrInputLongint        (char inp1[]);              // CONVERT STRING to Longint X      //DONE
void   fnRCL                    (int16_t inp);              //DONE


void   fnConstantR              (uint16_t constantAddr, uint16_t *constNr, real_t *rVal);
bool_t checkForAndChange_       (char *displayString, const real34_t *val, const real_t *constant, const real34_t *tol34, const char *constantStr,  bool_t frontSpace);

void fnDisplayFormatCycle       (uint16_t unusedButMandatoryParameter);


//JM To determine the menu number for a given menuId          //JMvv
int16_t mm(int16_t id);
//vv EXTRA DRAWINGS FOR RADIO_BUTTON AND CHECK_BOX
void JM_LINE2(uint32_t xx, uint32_t yy);
void rbColumnCcccccc(uint32_t xx, uint32_t yy);
void rbColumnCcSssssCc(uint32_t xx, uint32_t yy);
void rbColumnCcSssssssCc(uint32_t xx, uint32_t yy);
void rbColumnCSssCccSssC(uint32_t xx, uint32_t yy);
void rbColumnCSsCSssCSsC(uint32_t xx, uint32_t yy);
void rbColumnCcSsNnnSsCc(uint32_t xx, uint32_t yy);
void rbColumnCSsNnnnnSsC(uint32_t xx, uint32_t yy);
void rbColumnCSNnnnnnnSC(uint32_t xx, uint32_t yy);
void cbColumnCcccccccccc(uint32_t xx, uint32_t yy);
void cbColumnCSssssssssC(uint32_t xx, uint32_t yy);
void cbColumnCSsCccccSsC(uint32_t xx, uint32_t yy);
void cbColumnCSNnnnnnnSC(uint32_t xx, uint32_t yy);
void RB_CHECKED(uint32_t xx, uint32_t yy);
void RB_UNCHECKED(uint32_t xx, uint32_t yy);
void CB_CHECKED(uint32_t xx, uint32_t yy);
void CB_UNCHECKED(uint32_t xx, uint32_t yy);


void fnSetBCD (uint16_t bcd);
void setFGLSettings(uint16_t option);
void fnLongPressSwitches (uint16_t option);
void fnSetSI_All (uint16_t unusedButMandatoryParameter);
void fnSetCPXmult (uint16_t unusedButMandatoryParameter);

#endif // !ADDONS_H
