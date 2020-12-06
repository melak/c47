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
 * \file saveRestoreCalcState.c
 ***********************************************/

#include "wp43s.h"

#define BACKUP_VERSION         49  // Changed the way programs are displayed
#define START_REGISTER_VALUE 1522
#define BACKUP               ppgm_fp // The FIL *ppgm_fp pointer is provided by DMCP

static void save(const void *buffer, uint32_t size, void *stream) {
  #ifdef DMCP_BUILD
    UINT bytesWritten;
    f_write(stream, buffer, size, &bytesWritten);
  #else // !DMCP_BUILD
    fwrite(buffer, 1, size, stream);
  #endif // DMCP_BUILD
}



static uint32_t restore(void *buffer, uint32_t size, void *stream) {
  #ifdef DMCP_BUILD
    UINT bytesRead;
    f_read(stream, buffer, size, &bytesRead);
    return(bytesRead);
  #else // !DMCP_BUILD
    return(fread(buffer, 1, size, stream));
  #endif // DMCP_BUILD
}



#ifdef PC_BUILD
  void saveCalc(void) {
    uint32_t backupVersion = BACKUP_VERSION;
    uint32_t ramSize       = RAM_SIZE;
    uint32_t ramPtr;
    FILE *ppgm_fp;

    BACKUP = fopen("backup.bin", "wb");
    if(BACKUP == NULL) {
      printf("Cannot save calc's memory in file backup.bin!\n");
      exit(0);
    }

    if(calcMode == CM_CONFIRMATION) {
      calcMode = previousCalcMode;
      refreshScreen();
    }

    printf("Begin of calc's backup\n");


    save(&backupVersion,                      sizeof(backupVersion),                      BACKUP);
    save(&ramSize,                            sizeof(ramSize),                            BACKUP);
    save(ram,                                 TO_BYTES(RAM_SIZE),                         BACKUP);
    save(freeMemoryRegions,                   sizeof(freeMemoryRegions),                  BACKUP);
    save(&numberOfFreeMemoryRegions,          sizeof(numberOfFreeMemoryRegions),          BACKUP);
    save(globalFlags,                         sizeof(globalFlags),                        BACKUP);
    save(errorMessage,                        sizeof(errorMessage),                       BACKUP);
    save(aimBuffer,                           sizeof(aimBuffer),                          BACKUP);
    save(nimBufferDisplay,                    sizeof(nimBufferDisplay),                   BACKUP);
    save(tamBuffer,                           sizeof(tamBuffer),                          BACKUP);
    save(asmBuffer,                           sizeof(asmBuffer),                          BACKUP);
    save(oldTime,                             sizeof(oldTime),                            BACKUP);
    save(dateTimeString,                      sizeof(dateTimeString),                     BACKUP);
    save(softmenuStack,                       sizeof(softmenuStack),                      BACKUP);
    save(reg,                                 sizeof(reg),                                BACKUP);
    save(savedStackRegister,                  sizeof(savedStackRegister),                 BACKUP);
    save(kbd_usr,                             sizeof(kbd_usr),                            BACKUP);
    save(&tamFunction,                        sizeof(tamFunction),                        BACKUP);
    save(&tamNumber,                          sizeof(tamNumber),                          BACKUP);
    save(&tamNumberMin,                       sizeof(tamNumberMin),                       BACKUP);
    save(&tamNumberMax,                       sizeof(tamNumberMax),                       BACKUP);
    save(&tamDigit,                           sizeof(tamDigit),                           BACKUP);
    save(&tamOperation,                       sizeof(tamOperation),                       BACKUP);
    save(&tamLetteredRegister,                sizeof(tamLetteredRegister),                BACKUP);
    save(&tamCurrentOperation,                sizeof(tamCurrentOperation),                BACKUP);
    save(&rbrRegister,                        sizeof(rbrRegister),                        BACKUP);
    save(&numberOfLocalFlags,                 sizeof(numberOfLocalFlags),                 BACKUP);
    ramPtr = TO_WP43SMEMPTR(allLocalRegisterPointer);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43SMEMPTR(allNamedVariablePointer);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43SMEMPTR(statisticalSumsPointer);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43SMEMPTR(savedStatisticalSumsPointer);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    save(&xCursor,                            sizeof(xCursor),                            BACKUP);
    save(&yCursor,                            sizeof(yCursor),                            BACKUP);
    save(&firstGregorianDay,                  sizeof(firstGregorianDay),                  BACKUP);
    save(&denMax,                             sizeof(denMax),                             BACKUP);
    save(&softmenuStackPointer,               sizeof(softmenuStackPointer),               BACKUP);
    save(&softmenuStackPointerBeforeAIM,      sizeof(softmenuStackPointerBeforeAIM),      BACKUP);
    save(&transitionSystemState,              sizeof(transitionSystemState),              BACKUP);
    save(&currentRegisterBrowserScreen,       sizeof(currentRegisterBrowserScreen),       BACKUP);
    save(&currentFntScr,                      sizeof(currentFntScr),                      BACKUP);
    save(&currentFlgScr,                      sizeof(currentFlgScr),                      BACKUP);
    save(&displayFormat,                      sizeof(displayFormat),                      BACKUP);
    save(&displayFormatDigits,                sizeof(displayFormatDigits),                BACKUP);
    save(&shortIntegerWordSize,               sizeof(shortIntegerWordSize),               BACKUP);
    save(&significantDigits,                  sizeof(significantDigits),                  BACKUP);
    save(&shortIntegerMode,                   sizeof(shortIntegerMode),                   BACKUP);
    save(&currentAngularMode,                 sizeof(currentAngularMode),                 BACKUP);
    save(&groupingGap,                        sizeof(groupingGap),                        BACKUP);
    save(&roundingMode,                       sizeof(roundingMode),                       BACKUP);
    save(&calcMode,                           sizeof(calcMode),                           BACKUP);
    save(&nextChar,                           sizeof(nextChar),                           BACKUP);
    save(&alphaCase,                          sizeof(alphaCase),                          BACKUP);
    save(&hourGlassIconEnabled,               sizeof(hourGlassIconEnabled),               BACKUP);
    save(&watchIconEnabled,                   sizeof(watchIconEnabled),                   BACKUP);
    save(&serialIOIconEnabled,                sizeof(serialIOIconEnabled),                BACKUP);
    save(&printerIconEnabled,                 sizeof(printerIconEnabled),                 BACKUP);
    save(&cursorEnabled,                      sizeof(cursorEnabled),                      BACKUP);
    save(&cursorFont,                         sizeof(cursorFont),                         BACKUP);
    save(&rbr1stDigit,                        sizeof(rbr1stDigit),                        BACKUP);
    save(&shiftF,                             sizeof(shiftF),                             BACKUP);
    save(&shiftG,                             sizeof(shiftG),                             BACKUP);
    save(&tamMode,                            sizeof(tamMode),                            BACKUP);
    save(&rbrMode,                            sizeof(rbrMode),                            BACKUP);
    save(&showContent,                        sizeof(showContent),                        BACKUP);
    save(&numScreensNumericFont,              sizeof(numScreensNumericFont),              BACKUP);
    save(&numLinesNumericFont,                sizeof(numLinesNumericFont),                BACKUP);
    save(&numLinesStandardFont,               sizeof(numLinesStandardFont),               BACKUP);
    save(&numScreensStandardFont,             sizeof(numScreensStandardFont),             BACKUP);
    save(&previousCalcMode,                   sizeof(previousCalcMode),                   BACKUP);
    save(&lastErrorCode,                      sizeof(lastErrorCode),                      BACKUP);
    save(&nimNumberPart,                      sizeof(nimNumberPart),                      BACKUP);
    save(&displayStack,                       sizeof(displayStack),                       BACKUP);
    save(&hexDigits,                          sizeof(hexDigits),                          BACKUP);
    save(&errorMessageRegisterLine,           sizeof(errorMessageRegisterLine),           BACKUP);
    save(&shortIntegerMask,                   sizeof(shortIntegerMask),                   BACKUP);
    save(&shortIntegerSignBit,                sizeof(shortIntegerSignBit),                BACKUP);
    save(&temporaryInformation,               sizeof(temporaryInformation),               BACKUP);
    save(&glyphNotFound,                      sizeof(glyphNotFound),                      BACKUP);
    save(&funcOK,                             sizeof(funcOK),                             BACKUP);
    save(&screenChange,                       sizeof(screenChange),                       BACKUP);
    save(&exponentSignLocation,               sizeof(exponentSignLocation),               BACKUP);
    save(&denominatorLocation,                sizeof(denominatorLocation),                BACKUP);
    save(&imaginaryExponentSignLocation,      sizeof(imaginaryExponentSignLocation),      BACKUP);
    save(&imaginaryMantissaSignLocation,      sizeof(imaginaryMantissaSignLocation),      BACKUP);
    save(&lineTWidth,                         sizeof(lineTWidth),                         BACKUP);
    save(&lastIntegerBase,                    sizeof(lastIntegerBase),                    BACKUP);
    save(&wp43sMemInBytes,                    sizeof(wp43sMemInBytes),                    BACKUP);
    save(&gmpMemInBytes,                      sizeof(gmpMemInBytes),                      BACKUP);
    save(&alphaSelectionMenu,                 sizeof(alphaSelectionMenu),                 BACKUP);
    save(&lastFcnsMenuPos,                    sizeof(lastFcnsMenuPos),                    BACKUP);
    save(&lastMenuMenuPos,                    sizeof(lastMenuMenuPos),                    BACKUP);
    save(&lastCnstMenuPos,                    sizeof(lastCnstMenuPos),                    BACKUP);
    save(&lastSyFlMenuPos,                    sizeof(lastSyFlMenuPos),                    BACKUP);
    save(&lastAIntMenuPos,                    sizeof(lastAIntMenuPos),                    BACKUP);
    save(&lastProgMenuPos,                    sizeof(lastProgMenuPos),                    BACKUP);
    save(&lgCatalogSelection,                 sizeof(lgCatalogSelection),                 BACKUP);
    save(displayValueX,                       sizeof(displayValueX),                      BACKUP);
    save(&pcg32_global,                       sizeof(pcg32_global),                       BACKUP);
    save(&exponentLimit,                      sizeof(exponentLimit),                      BACKUP);
    save(&keyActionProcessed,                 sizeof(keyActionProcessed),                 BACKUP);
    save(&systemFlags,                        sizeof(systemFlags),                        BACKUP);
    save(&savedSystemFlags,                   sizeof(savedSystemFlags),                   BACKUP);
    save(&thereIsSomethingToUndo,             sizeof(thereIsSomethingToUndo),             BACKUP);
    ramPtr = TO_WP43SMEMPTR(beginOfProgramMemory);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // beginOfProgramMemory pointer to block
    ramPtr = (uint32_t)((void *)beginOfProgramMemory -        TO_PCMEMPTR(TO_WP43SMEMPTR(beginOfProgramMemory)));
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // beginOfProgramMemory offset within block
    ramPtr = TO_WP43SMEMPTR(firstFreeProgramByte);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstFreeProgramByte pointer to block
    ramPtr = (uint32_t)((void *)firstFreeProgramByte - TO_PCMEMPTR(TO_WP43SMEMPTR(firstFreeProgramByte)));
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstFreeProgramByte offset within block
    ramPtr = TO_WP43SMEMPTR(firstDisplayedStep);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstDisplayedStep pointer to block
    ramPtr = (uint32_t)((void *)firstDisplayedStep - TO_PCMEMPTR(TO_WP43SMEMPTR(firstDisplayedStep)));
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstDisplayedStep offset within block
    ramPtr = TO_WP43SMEMPTR(currentStep);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // currentStep pointer to block
    ramPtr = (uint32_t)((void *)currentStep - TO_PCMEMPTR(TO_WP43SMEMPTR(currentStep)));
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // currentStep offset within block
    save(&freeProgramBytes,                   sizeof(freeProgramBytes),                   BACKUP);
    save(&firstDisplayedLocalStepNumber,      sizeof(firstDisplayedLocalStepNumber),      BACKUP);
    save(&currentLocalStepNumber,             sizeof(currentLocalStepNumber),             BACKUP);
    save(&currentProgramNumber,               sizeof(currentProgramNumber),               BACKUP);
    save(&lastProgramListEnd,                 sizeof(lastProgramListEnd),                 BACKUP);
    save(&programListEnd,                     sizeof(programListEnd),                     BACKUP);

    fclose(BACKUP);
    printf("End of calc's backup\n");
  }



  void restoreCalc(void) {
    uint32_t backupVersion, ramSize, ramPtr;
    FILE *ppgm_fp;

    fnReset(CONFIRMED);
    BACKUP = fopen("backup.bin", "rb");
    if(BACKUP == NULL) {
      printf("Cannot restore calc's memory from file backup.bin! Performing RESET\n");
      return;
    }

    restore(&backupVersion,                      sizeof(backupVersion),                      BACKUP);
    restore(&ramSize,                            sizeof(ramSize),                            BACKUP);
    if(backupVersion != BACKUP_VERSION || ramSize != RAM_SIZE) {
      fclose(BACKUP);

      printf("Cannot restore calc's memory from file backup.bin! File backup.bin is from another backup version.\n");
      printf("               Backup file      Program\n");
      printf("backupVersion  %6u           %6d\n", backupVersion, BACKUP_VERSION);
      printf("ramSize blocks %6u           %6d\n", ramSize, RAM_SIZE);
      printf("ramSize bytes  %6u           %6d\n", TO_BYTES(ramSize), TO_BYTES(RAM_SIZE));
      return;
    }
    else {
      printf("Begin of calc's restoration\n");

      restore(ram,                                 TO_BYTES(RAM_SIZE),                         BACKUP);
      restore(freeMemoryRegions,                   sizeof(freeMemoryRegions),                  BACKUP);
      restore(&numberOfFreeMemoryRegions,          sizeof(numberOfFreeMemoryRegions),          BACKUP);
      restore(globalFlags,                         sizeof(globalFlags),                        BACKUP);
      restore(errorMessage,                        sizeof(errorMessage),                       BACKUP);
      restore(aimBuffer,                           sizeof(aimBuffer),                          BACKUP);
      restore(nimBufferDisplay,                    sizeof(nimBufferDisplay),                   BACKUP);
      restore(tamBuffer,                           sizeof(tamBuffer),                          BACKUP);
      restore(asmBuffer,                           sizeof(asmBuffer),                          BACKUP);
      restore(oldTime,                             sizeof(oldTime),                            BACKUP);
      restore(dateTimeString,                      sizeof(dateTimeString),                     BACKUP);
      restore(softmenuStack,                       sizeof(softmenuStack),                      BACKUP);
      restore(reg,                                 sizeof(reg),                                BACKUP);
      restore(savedStackRegister,                  sizeof(savedStackRegister),                 BACKUP);
      restore(kbd_usr,                             sizeof(kbd_usr),                            BACKUP);
      restore(&tamFunction,                        sizeof(tamFunction),                        BACKUP);
      restore(&tamNumber,                          sizeof(tamNumber),                          BACKUP);
      restore(&tamNumberMin,                       sizeof(tamNumberMin),                       BACKUP);
      restore(&tamNumberMax,                       sizeof(tamNumberMax),                       BACKUP);
      restore(&tamDigit,                           sizeof(tamDigit),                           BACKUP);
      restore(&tamOperation,                       sizeof(tamOperation),                       BACKUP);
      restore(&tamLetteredRegister,                sizeof(tamLetteredRegister),                BACKUP);
      restore(&tamCurrentOperation,                sizeof(tamCurrentOperation),                BACKUP);
      restore(&rbrRegister,                        sizeof(rbrRegister),                        BACKUP);
      restore(&numberOfLocalFlags,                 sizeof(numberOfLocalFlags),                 BACKUP);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      allLocalRegisterPointer = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      allNamedVariablePointer = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      statisticalSumsPointer = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      savedStatisticalSumsPointer = TO_PCMEMPTR(ramPtr);
      restore(&xCursor,                            sizeof(xCursor),                            BACKUP);
      restore(&yCursor,                            sizeof(yCursor),                            BACKUP);
      restore(&firstGregorianDay,                  sizeof(firstGregorianDay),                  BACKUP);
      restore(&denMax,                             sizeof(denMax),                             BACKUP);
      restore(&softmenuStackPointer,               sizeof(softmenuStackPointer),               BACKUP);
      restore(&softmenuStackPointerBeforeAIM,      sizeof(softmenuStackPointerBeforeAIM),      BACKUP);
      restore(&transitionSystemState,              sizeof(transitionSystemState),              BACKUP);
      restore(&currentRegisterBrowserScreen,       sizeof(currentRegisterBrowserScreen),       BACKUP);
      restore(&currentFntScr,                      sizeof(currentFntScr),                      BACKUP);
      restore(&currentFlgScr,                      sizeof(currentFlgScr),                      BACKUP);
      restore(&displayFormat,                      sizeof(displayFormat),                      BACKUP);
      restore(&displayFormatDigits,                sizeof(displayFormatDigits),                BACKUP);
      restore(&shortIntegerWordSize,               sizeof(shortIntegerWordSize),               BACKUP);
      restore(&significantDigits,                  sizeof(significantDigits),                  BACKUP);
      restore(&shortIntegerMode,                   sizeof(shortIntegerMode),                   BACKUP);
      restore(&currentAngularMode,                 sizeof(currentAngularMode),                 BACKUP);
      restore(&groupingGap,                        sizeof(groupingGap),                        BACKUP);
      restore(&roundingMode,                       sizeof(roundingMode),                       BACKUP);
      restore(&calcMode,                           sizeof(calcMode),                           BACKUP);
      restore(&nextChar,                           sizeof(nextChar),                           BACKUP);
      restore(&alphaCase,                          sizeof(alphaCase),                          BACKUP);
      restore(&hourGlassIconEnabled,               sizeof(hourGlassIconEnabled),               BACKUP);
      restore(&watchIconEnabled,                   sizeof(watchIconEnabled),                   BACKUP);
      restore(&serialIOIconEnabled,                sizeof(serialIOIconEnabled),                BACKUP);
      restore(&printerIconEnabled,                 sizeof(printerIconEnabled),                 BACKUP);
      restore(&cursorEnabled,                      sizeof(cursorEnabled),                      BACKUP);
      restore(&cursorFont,                         sizeof(cursorFont),                         BACKUP);
      restore(&rbr1stDigit,                        sizeof(rbr1stDigit),                        BACKUP);
      restore(&shiftF,                             sizeof(shiftF),                             BACKUP);
      restore(&shiftG,                             sizeof(shiftG),                             BACKUP);
      restore(&tamMode,                            sizeof(tamMode),                            BACKUP);
      restore(&rbrMode,                            sizeof(rbrMode),                            BACKUP);
      restore(&showContent,                        sizeof(showContent),                        BACKUP);
      restore(&numScreensNumericFont,              sizeof(numScreensNumericFont),              BACKUP);
      restore(&numLinesNumericFont,                sizeof(numLinesNumericFont),                BACKUP);
      restore(&numLinesStandardFont,               sizeof(numLinesStandardFont),               BACKUP);
      restore(&numScreensStandardFont,             sizeof(numScreensStandardFont),             BACKUP);
      restore(&previousCalcMode,                   sizeof(previousCalcMode),                   BACKUP);
      restore(&lastErrorCode,                      sizeof(lastErrorCode),                      BACKUP);
      restore(&nimNumberPart,                      sizeof(nimNumberPart),                      BACKUP);
      restore(&displayStack,                       sizeof(displayStack),                       BACKUP);
      restore(&hexDigits,                          sizeof(hexDigits),                          BACKUP);
      restore(&errorMessageRegisterLine,           sizeof(errorMessageRegisterLine),           BACKUP);
      restore(&shortIntegerMask,                   sizeof(shortIntegerMask),                   BACKUP);
      restore(&shortIntegerSignBit,                sizeof(shortIntegerSignBit),                BACKUP);
      restore(&temporaryInformation,               sizeof(temporaryInformation),               BACKUP);

      restore(&glyphNotFound,                      sizeof(glyphNotFound),                      BACKUP);
      glyphNotFound.data   = malloc(38);
      xcopy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8", 38);

      restore(&funcOK,                             sizeof(funcOK),                             BACKUP);
      restore(&screenChange,                       sizeof(screenChange),                       BACKUP);
      restore(&exponentSignLocation,               sizeof(exponentSignLocation),               BACKUP);
      restore(&denominatorLocation,                sizeof(denominatorLocation),                BACKUP);
      restore(&imaginaryExponentSignLocation,      sizeof(imaginaryExponentSignLocation),      BACKUP);
      restore(&imaginaryMantissaSignLocation,      sizeof(imaginaryMantissaSignLocation),      BACKUP);
      restore(&lineTWidth,                         sizeof(lineTWidth),                         BACKUP);
      restore(&lastIntegerBase,                    sizeof(lastIntegerBase),                    BACKUP);
      restore(&wp43sMemInBytes,                    sizeof(wp43sMemInBytes),                    BACKUP);
      restore(&gmpMemInBytes,                      sizeof(gmpMemInBytes),                      BACKUP);
      restore(&alphaSelectionMenu,                 sizeof(alphaSelectionMenu),                 BACKUP);
      restore(&lastFcnsMenuPos,                    sizeof(lastFcnsMenuPos),                    BACKUP);
      restore(&lastMenuMenuPos,                    sizeof(lastMenuMenuPos),                    BACKUP);
      restore(&lastCnstMenuPos,                    sizeof(lastCnstMenuPos),                    BACKUP);
      restore(&lastSyFlMenuPos,                    sizeof(lastSyFlMenuPos),                    BACKUP);
      restore(&lastAIntMenuPos,                    sizeof(lastAIntMenuPos),                    BACKUP);
      restore(&lastProgMenuPos,                    sizeof(lastProgMenuPos),                    BACKUP);
      restore(&lgCatalogSelection,                 sizeof(lgCatalogSelection),                 BACKUP);
      restore(displayValueX,                       sizeof(displayValueX),                      BACKUP);
      restore(&pcg32_global,                       sizeof(pcg32_global),                       BACKUP);
      restore(&exponentLimit,                      sizeof(exponentLimit),                      BACKUP);
      restore(&keyActionProcessed,                 sizeof(keyActionProcessed),                 BACKUP);
      restore(&systemFlags,                        sizeof(systemFlags),                        BACKUP);
      restore(&savedSystemFlags,                   sizeof(savedSystemFlags),                   BACKUP);
      restore(&thereIsSomethingToUndo,             sizeof(thereIsSomethingToUndo),             BACKUP);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // beginOfProgramMemory pointer to block
      beginOfProgramMemory = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // beginOfProgramMemory offset within block
      beginOfProgramMemory += ramPtr;
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstFreeProgramByte pointer to block
      firstFreeProgramByte = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstFreeProgramByte offset within block
      firstFreeProgramByte += ramPtr;
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstDisplayedStep pointer to block
      firstDisplayedStep = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstDisplayedStep offset within block
      firstDisplayedStep += ramPtr;
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // currentStep pointer to block
      currentStep = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // currentStep offset within block
      currentStep += ramPtr;
      restore(&freeProgramBytes,                   sizeof(freeProgramBytes),                   BACKUP);
      restore(&firstDisplayedLocalStepNumber,      sizeof(firstDisplayedLocalStepNumber),      BACKUP);
      restore(&currentLocalStepNumber,             sizeof(currentLocalStepNumber),             BACKUP);
      restore(&currentProgramNumber,               sizeof(currentProgramNumber),               BACKUP);
      restore(&lastProgramListEnd,                 sizeof(lastProgramListEnd),                 BACKUP);
      restore(&programListEnd,                     sizeof(programListEnd),                     BACKUP);

      fclose(BACKUP);
      printf("End of calc's restoration\n");

      scanLabelsAndPrograms();
      defineCurrentProgramFromGlobalStepNumber(currentLocalStepNumber + programList[currentProgramNumber - 1].step - 1);

      #if (DEBUG_REGISTER_L == 1)
        refreshRegisterLine(REGISTER_X); // to show L register
      #endif // (DEBUG_REGISTER_L == 1)

      #if (SCREEN_800X480 == 1)
        if(calcMode == CM_NORMAL)                     {}
        else if(calcMode == CM_AIM)                   {cursorEnabled = true;}
        else if(calcMode == CM_TAM)                   {}
        else if(calcMode == CM_TAM_OVER_PEM)          {}
        else if(calcMode == CM_NIM)                   {cursorEnabled = true;}
        else if(calcMode == CM_ASM)                   {}
        else if(calcMode == CM_ASM_OVER_TAM)          {clearSystemFlag(FLAG_ALPHA);}
        else if(calcMode == CM_ASM_OVER_TAM_OVER_PEM) {clearSystemFlag(FLAG_ALPHA);}
        else if(calcMode == CM_ASM_OVER_AIM)          {clearSystemFlag(FLAG_ALPHA);}
        else if(calcMode == CM_ASM_OVER_PEM)          {clearSystemFlag(FLAG_ALPHA);}
        else if(calcMode == CM_REGISTER_BROWSER)      {}
        else if(calcMode == CM_FLAG_BROWSER)          {}
        else if(calcMode == CM_FONT_BROWSER)          {}
        else if(calcMode == CM_PEM)                   {}
        else {
          sprintf(errorMessage, "In function restoreCalc: %" PRIu8 " is an unexpected value for calcMode", calcMode);
          displayBugScreen(errorMessage);
        }
      #else // (SCREEN_800X480 == 0)
        if(calcMode == CM_NORMAL)                      calcModeNormalGui();
        else if(calcMode == CM_AIM)                   {calcModeAimGui(); cursorEnabled = true;}
        else if(calcMode == CM_TAM)                    calcModeTamGui();
        else if(calcMode == CM_TAM_OVER_PEM)           calcModeTamGui();
        else if(calcMode == CM_NIM)                   {calcModeNormalGui(); cursorEnabled = true;}
        else if(calcMode == CM_ASM)                    calcModeAsm();
        else if(calcMode == CM_ASM_OVER_TAM)          {calcModeAsm(); calcMode = CM_ASM_OVER_TAM;          clearSystemFlag(FLAG_ALPHA);}
        else if(calcMode == CM_ASM_OVER_TAM_OVER_PEM) {calcModeAsm(); calcMode = CM_ASM_OVER_TAM_OVER_PEM; clearSystemFlag(FLAG_ALPHA);}
        else if(calcMode == CM_ASM_OVER_AIM)          {calcModeAsm(); calcMode = CM_ASM_OVER_AIM;          clearSystemFlag(FLAG_ALPHA);}
        else if(calcMode == CM_REGISTER_BROWSER)       calcModeNormalGui();
        else if(calcMode == CM_FLAG_BROWSER)           calcModeNormalGui();
        else if(calcMode == CM_FONT_BROWSER)           calcModeNormalGui();
        else if(calcMode == CM_PEM)                    calcModeNormalGui();
        else {
          sprintf(errorMessage, "In function restoreCalc: %" PRIu8 " is an unexpected value for calcMode", calcMode);
          displayBugScreen(errorMessage);
        }
      #endif // (SCREEN_800X480 == 1)

      refreshScreen();
    }
  }
#endif // PC_BUILD


static void registerToSaveString(calcRegister_t regist) {
  longInteger_t lgInt;
  int16_t sign;
  uint64_t value;
  char *str, *cfg;

  switch(getRegisterDataType(regist)) {
    case dtLongInteger:
      convertLongIntegerRegisterToLongInteger(regist, lgInt);
      longIntegerToAllocatedString(lgInt, tmpString + START_REGISTER_VALUE, TMP_STR_LENGTH - START_REGISTER_VALUE - 1);
      longIntegerFree(lgInt);
      strcpy(aimBuffer, "LonI");
      break;

    case dtString:
      stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)(tmpString + START_REGISTER_VALUE));
      strcpy(aimBuffer, "Stri");
      break;

    case dtShortInteger:
      convertShortIntegerRegisterToUInt64(regist, &sign, &value);
      sprintf(tmpString + START_REGISTER_VALUE, "%c%" PRIu64 " %" PRIu32, sign ? '-' : '+', value, getRegisterShortIntegerBase(regist));
      strcpy(aimBuffer, "ShoI");
      break;

    case dtReal34:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpString + START_REGISTER_VALUE);
      switch(getRegisterAngularMode(regist)) {
        case AM_DEGREE:
          strcpy(aimBuffer, "Real:DEG");
          break;

        case AM_GRAD:
          strcpy(aimBuffer, "Real:GRAD");
          break;

        case AM_RADIAN:
          strcpy(aimBuffer, "Real:RAD");
          break;

        case AM_MULTPI:
          strcpy(aimBuffer, "Real:MULTPI");
          break;

        case AM_DMS:
          strcpy(aimBuffer, "Real:DMS");
          break;

        case AM_NONE:
          strcpy(aimBuffer, "Real");
          break;

        default:
          strcpy(aimBuffer, "Real:???");
          break;
      }
      break;

    case dtComplex34:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpString + START_REGISTER_VALUE);
      strcat(tmpString + START_REGISTER_VALUE, " ");
      real34ToString(REGISTER_IMAG34_DATA(regist), tmpString + START_REGISTER_VALUE + strlen(tmpString + START_REGISTER_VALUE));
      strcpy(aimBuffer, "Cplx");
      break;

    case dtConfig:
      for(str=tmpString + START_REGISTER_VALUE, cfg=(char *)REGISTER_CONFIG_DATA(regist), value=0; value<sizeof(dtConfigDescriptor_t); value++, cfg++, str+=2) {
        sprintf(str, "%02X", *cfg);
      }
      strcpy(aimBuffer, "Conf");
      break;

    default:
      strcpy(tmpString + START_REGISTER_VALUE, "???");
      strcpy(aimBuffer, "????");
  }
}



void fnSave(uint16_t unusedButMandatoryParameter) {
  calcRegister_t regist;
  uint32_t i;

  #ifdef DMCP_BUILD
    FRESULT result;

    sys_disk_write_enable(1);
    check_create_dir("SAVFILES");
    result = f_open(BACKUP, "SAVFILES\\wp43s.sav", FA_CREATE_ALWAYS | FA_WRITE);
    if(result != FR_OK) {
      sys_disk_write_enable(0);
      return;
    }
  #else // !DMCP_BUILD
    FILE *ppgm_fp;

    BACKUP = fopen("wp43s.sav", "wb");
    if(BACKUP == NULL) {
      printf("Cannot SAVE in file wp43s.sav!\n");
      return;
    }
  #endif // DMCP_BUILD

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wrestrict"

  // Global registers
  sprintf(tmpString, "GLOBAL_REGISTERS\n%" PRIu16 "\n", FIRST_LOCAL_REGISTER);
  save(tmpString, strlen(tmpString), BACKUP);
  for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
    registerToSaveString(regist);
    sprintf(tmpString, "R%03" PRId16 "\n%s\n%s\n", regist, aimBuffer, tmpString + START_REGISTER_VALUE);
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // Global flags
  strcpy(tmpString, "GLOBAL_FLAGS\n");
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "%" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 "\n",
                       globalFlags[0],
                                   globalFlags[1],
                                               globalFlags[2],
                                                           globalFlags[3],
                                                                       globalFlags[4],
                                                                                   globalFlags[5],
                                                                                               globalFlags[6]);
  save(tmpString, strlen(tmpString), BACKUP);

  // Local registers
  sprintf(tmpString, "LOCAL_REGISTERS\n%" PRIu16 "\n", allLocalRegisterPointer->numberOfLocalRegisters);
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<allLocalRegisterPointer->numberOfLocalRegisters; i++) {
    registerToSaveString(FIRST_LOCAL_REGISTER + i);
    sprintf(tmpString, "R.%02" PRIu32 "\n%s\n%s\n", i, aimBuffer, tmpString + START_REGISTER_VALUE);
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // Local flags
  if(allLocalRegisterPointer->numberOfLocalRegisters) {
    sprintf(tmpString, "LOCAL_FLAGS\n%" PRIu16 "\n", allLocalRegisterPointer->localFlags);
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // Named variables
  sprintf(tmpString, "NAMED_VARIABLES\n%" PRIu16 "\n", allNamedVariablePointer->numberOfNamedVariables);
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<allNamedVariablePointer->numberOfNamedVariables; i++) {
    registerToSaveString(FIRST_NAMED_VARIABLE + i);
    sprintf(tmpString, "%s\n%s\n%s\n", "name", aimBuffer, tmpString + START_REGISTER_VALUE);
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // Statistical sums
  sprintf(tmpString, "STATISTICAL_SUMS\n%" PRIu16 "\n", statisticalSumsPointer ? NUMBER_OF_STATISTICAL_SUMS : 0);
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<(statisticalSumsPointer ? NUMBER_OF_STATISTICAL_SUMS : 0); i++) {
    realToString(statisticalSumsPointer + REAL_SIZE * i , tmpString + START_REGISTER_VALUE);
    sprintf(tmpString, "%s\n", tmpString + START_REGISTER_VALUE);
    save(tmpString, strlen(tmpString), BACKUP);
  }
  #pragma GCC diagnostic pop

  // System flags
  sprintf(tmpString, "SYSTEM_FLAGS\n%" PRIu64 "\n", systemFlags);
  save(tmpString, strlen(tmpString), BACKUP);

  // Keyboard assignments
  sprintf(tmpString, "KEYBOARD_ASSIGNMENTS\n37\n");
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<37; i++) {
    sprintf(tmpString, "%" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 "\n",
                         kbd_usr[i].keyId,
                                     kbd_usr[i].primary,
                                                 kbd_usr[i].fShifted,
                                                             kbd_usr[i].gShifted,
                                                                         kbd_usr[i].keyLblAim,
                                                                                     kbd_usr[i].primaryAim,
                                                                                                 kbd_usr[i].fShiftedAim,
                                                                                                             kbd_usr[i].gShiftedAim,
                                                                                                                         kbd_usr[i].primaryTam);
  save(tmpString, strlen(tmpString), BACKUP);
  }

  // Programs
  uint16_t currentSizeInBlocks = RAM_SIZE - freeMemoryRegions[numberOfFreeMemoryRegions - 1].address - freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks;
  sprintf(tmpString, "PROGRAMS\n%" PRIu16 "\n", currentSizeInBlocks);
  save(tmpString, strlen(tmpString), BACKUP);

  sprintf(tmpString, "%" PRIu32 "\n%" PRIu32 "\n", (uint32_t)TO_WP43SMEMPTR(currentStep), (uint32_t)((void *)currentStep - TO_PCMEMPTR(TO_WP43SMEMPTR(currentStep)))); // currentStep block pointer + offset within block
  save(tmpString, strlen(tmpString), BACKUP);

  sprintf(tmpString, "%" PRIu32 "\n%" PRIu32 "\n", (uint32_t)TO_WP43SMEMPTR(firstFreeProgramByte), (uint32_t)((void *)firstFreeProgramByte - TO_PCMEMPTR(TO_WP43SMEMPTR(firstFreeProgramByte)))); // firstFreeProgramByte block pointer + offset within block
  save(tmpString, strlen(tmpString), BACKUP);

  sprintf(tmpString, "%" PRIu16 "\n", freeProgramBytes);
  save(tmpString, strlen(tmpString), BACKUP);

  for(i=0; i<currentSizeInBlocks; i++) {
    sprintf(tmpString, "%" PRIu32 "\n", *(((uint32_t *)(beginOfProgramMemory)) + i));
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // Other configuration stuff
  sprintf(tmpString, "OTHER_CONFIGURATION_STUFF\n14\n");
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "firstGregorianDay\n1582 10 15\n");
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "denMax\n%" PRIu32 "\n", denMax);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "displayFormat\n%" PRIu8 "\n", displayFormat);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "displayFormatDigits\n%" PRIu8 "\n", displayFormatDigits);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "shortIntegerWordSize\n%" PRIu8 "\n", shortIntegerWordSize);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "shortIntegerMode\n%" PRIu8 "\n", shortIntegerMode);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "significantDigits\n%" PRIu8 "\n", significantDigits);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "currentAngularMode\n%" PRIu8 "\n", currentAngularMode);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "groupingGap\n%" PRIu8 "\n", groupingGap);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "roundingMode\n%" PRIu8 "\n", roundingMode);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "displayStack\n%" PRIu8 "\n", displayStack);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "rngState\n%" PRIu64 " %" PRIu64 "\n", pcg32_global.state, pcg32_global.inc);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "exponentLimit\n%" PRId16 "\n", exponentLimit);
  save(tmpString, strlen(tmpString), BACKUP);

  #ifdef DMCP_BUILD
    f_close(BACKUP);
    sys_disk_write_enable(0);
  #else // !DMCP_BUILD
    fclose(BACKUP);
  #endif // DMCP_BUILD

  temporaryInformation = TI_SAVED;
}



static void readLine(void *stream, char *line) {
  restore(line, 1, stream);
  while(*line == '\n' || *line == '\r') {
    restore(line, 1, stream);
  }

  while(*line != '\n' && *line != '\r') {
    restore(++line, 1, stream);
  }

  *line = 0;
}



uint8_t stringToUint8(const char *str) {
  uint8_t value = 0;


  while('0' <= *str && *str <= '9') {
    value = value*10 + (*(str++) - '0');
  }

  return value;
}



uint16_t stringToUint16(const char *str) {
  uint16_t value = 0;


  while('0' <= *str && *str <= '9') {
    value = value*10 + (*(str++) - '0');
  }

  return value;
}



uint32_t stringToUint32(const char *str) {
  uint32_t value = 0;

  while('0' <= *str && *str <= '9') {
    value = value*10 + (*(str++) - '0');
  }

  return value;
}



uint64_t stringToUint64(const char *str) {
  uint64_t value = 0;

  while('0' <= *str && *str <= '9') {
    value = value*10 + (*(str++) - '0');
  }

  return value;
}



int16_t stringToInt16(const char *str) {
  int16_t value = 0;
  bool_t sign = false;

  if(*str == '-') {
    str++;
    sign = true;
  }
  else if(*str == '+') {
    str++;
  }

  while('0' <= *str && *str <= '9') {
    value = value*10 + (*(str++) - '0');
  }

  if(sign) {
    value = -value;
  }
  return value;
}



int32_t stringToInt32(const char *str) {
  int32_t value = 0;
  bool_t sign = false;

  if(*str == '-') {
    str++;
    sign = true;
  }
  else if(*str == '+') {
    str++;
  }

  while('0' <= *str && *str <= '9') {
    value = value*10 + (*(str++) - '0');
  }

  if(sign) {
    value = -value;
  }
  return value;
}



static void restoreRegister(calcRegister_t regist, char *type, char *value) {
  uint32_t tag = AM_NONE;

  if(type[4] == ':') {
         if(type[5] == 'D' && type[6] == 'E') tag = AM_DEGREE;
    else if(type[5] == 'D' && type[6] == 'M') tag = AM_DMS;
    else if(type[5] == 'G')                   tag = AM_GRAD;
    else if(type[5] == 'R')                   tag = AM_RADIAN;
    else if(type[5] == 'M')                   tag = AM_MULTPI;
    else                                      tag = AM_NONE;

    reallocateRegister(regist, dtReal34, REAL34_SIZE, tag);
    stringToReal34(value, REGISTER_REAL34_DATA(regist));
  }

  else if(strcmp(type, "Real") == 0) {
    reallocateRegister(regist, dtReal34, REAL34_SIZE, tag);
    stringToReal34(value, REGISTER_REAL34_DATA(regist));
  }

  else if(strcmp(type, "LonI") == 0) {
    longInteger_t lgInt;

    longIntegerInit(lgInt);
    stringToLongInteger(value, 10, lgInt);
    convertLongIntegerToLongIntegerRegister(lgInt, regist);
    longIntegerFree(lgInt);
  }

  else if(strcmp(type, "Stri") == 0) {
    int32_t len;

    utf8ToString((uint8_t *)value, errorMessage);
    len = stringByteLength(errorMessage) + 1;
    reallocateRegister(regist, dtString, TO_BLOCKS(len), AM_NONE);
    xcopy(REGISTER_STRING_DATA(regist), errorMessage, len);
  }

  else if(strcmp(type, "ShoI") == 0) {
    uint16_t sign = (value[0] == '-' ? 1 : 0);
    uint64_t val  = stringToUint64(value + 1);

    while(*value != ' ') value++;
    while(*value == ' ') value++;
    uint32_t base = stringToUint32(value);

    convertUInt64ToShortIntegerRegister(sign, val, base, regist);
  }

  else if(strcmp(type, "Cplx") == 0) {
    char *imaginaryPart;

    reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    imaginaryPart = value;
    while(*imaginaryPart != ' ') imaginaryPart++;
    *(imaginaryPart++) = 0;
    stringToReal34(value, REGISTER_REAL34_DATA(regist));
    stringToReal34(imaginaryPart, REGISTER_IMAG34_DATA(regist));
  }

  else if(strcmp(type, "Conf") == 0) {
    char *cfg;

    reallocateRegister(regist, dtConfig, CONFIG_SIZE, AM_NONE);
    for(cfg=(char *)REGISTER_CONFIG_DATA(regist), tag=0; tag<sizeof(dtConfigDescriptor_t); tag++, value+=2, cfg++) {
      *cfg = ((*value >= 'A' ? *value - 'A' + 10 : *value - '0') << 8) | (*(value + 1) >= 'A' ? *(value + 1) - 'A' + 10 : *(value + 1) - '0');
    }
  }

  else {
    sprintf(errorMessage, "In function restoreRegister: Date type %s is to be coded!", type);
    displayBugScreen(errorMessage);
  }
}



static void restoreOneSection(void *stream, uint16_t loadMode) {
  int16_t i, numberOfRegs;
  calcRegister_t regist;
  char *str;

  readLine(stream, tmpString);

  if(strcmp(tmpString, "GLOBAL_REGISTERS") == 0) {
    readLine(stream, tmpString); // Number of global registers
    numberOfRegs = stringToInt16(tmpString);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpString); // Register number
      regist = stringToInt16(tmpString + 1);
      readLine(stream, aimBuffer); // Register data type
      readLine(stream, tmpString); // Register value

      if(loadMode == LM_ALL || (loadMode == LM_REGISTERS && regist < REGISTER_X)) {
        restoreRegister(regist, aimBuffer, tmpString);
      }
    }
  }

  else if(strcmp(tmpString, "GLOBAL_FLAGS") == 0) {
    readLine(stream, tmpString); // Global flags
    if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
      str = tmpString;
      globalFlags[0] = stringToInt16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[1] = stringToInt16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[2] = stringToInt16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[3] = stringToInt16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[4] = stringToInt16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[5] = stringToInt16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[6] = stringToInt16(str);
    }
  }

  else if(strcmp(tmpString, "LOCAL_REGISTERS") == 0) {
    readLine(stream, tmpString); // Number of local registers
    numberOfRegs = stringToInt16(tmpString);
    if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
      allocateLocalRegisters(numberOfRegs);
    }

    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpString); // Register number
      regist = stringToInt16(tmpString + 2) + FIRST_LOCAL_REGISTER;
      readLine(stream, aimBuffer); // Register data type
      readLine(stream, tmpString); // Register value

      if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
        restoreRegister(regist, aimBuffer, tmpString);
      }
    }

    if(numberOfRegs > 0) {
      readLine(stream, tmpString); // LOCAL_FLAGS
      readLine(stream, tmpString); // LOCAL_FLAGS
      if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
        allLocalRegisterPointer->localFlags = stringToUint16(tmpString);
      }
    }
  }

  else if(strcmp(tmpString, "NAMED_VARIABLES") == 0) {
    readLine(stream, tmpString); // Number of named variables
    numberOfRegs = stringToInt16(tmpString);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, errorMessage); // Variable name
      readLine(stream, aimBuffer); // Variable data type
      readLine(stream, tmpString); // Variable value

      if(loadMode == LM_ALL || loadMode == LM_NAMED_VARIABLES) {
        //printf("Variable %s ", errorMessage);
        //printf("%s = ", aimBuffer);
        //printf("%s\n", tmpString);
      }
    }
  }

  else if(strcmp(tmpString, "STATISTICAL_SUMS") == 0) {
    readLine(stream, tmpString); // Number of statistical sums
    numberOfRegs = stringToInt16(tmpString);
    if(numberOfRegs > 0 && (loadMode == LM_ALL || loadMode == LM_SUMS)) {
      initStatisticalSums();
    }

    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpString); // statistical sum
      if(loadMode == LM_ALL || loadMode == LM_SUMS) {
        stringToReal(tmpString, (real_t *)(statisticalSumsPointer + REAL_SIZE * i), &ctxtReal75);
      }
    }
  }

  else if(strcmp(tmpString, "SYSTEM_FLAGS") == 0) {
    readLine(stream, tmpString); // Global flags
    if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
      systemFlags = stringToUint64(tmpString);
    }
  }

  else if(strcmp(tmpString, "KEYBOARD_ASSIGNMENTS") == 0) {
    readLine(stream, tmpString); // Number of keys
    numberOfRegs = stringToInt16(tmpString);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpString); // key
      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        str = tmpString;
        kbd_usr[i].keyId = stringToUint16(str);

        while(*str != ' ') str++;
        while(*str == ' ') str++;
        kbd_usr[i].primary = stringToUint16(str);

        while(*str != ' ') str++;
        while(*str == ' ') str++;
        kbd_usr[i].fShifted = stringToUint16(str);

        while(*str != ' ') str++;
        while(*str == ' ') str++;
        kbd_usr[i].gShifted = stringToUint16(str);

        while(*str != ' ') str++;
        while(*str == ' ') str++;
        kbd_usr[i].keyLblAim = stringToUint16(str);

        while(*str != ' ') str++;
        while(*str == ' ') str++;
        kbd_usr[i].primaryAim = stringToUint16(str);

        while(*str != ' ') str++;
        while(*str == ' ') str++;
        kbd_usr[i].fShiftedAim = stringToUint16(str);

        while(*str != ' ') str++;
        while(*str == ' ') str++;
        kbd_usr[i].gShiftedAim = stringToUint16(str);

        while(*str != ' ') str++;
        while(*str == ' ') str++;
        kbd_usr[i].primaryTam = stringToUint16(str);
      }
    }
  }

  else if(strcmp(tmpString, "PROGRAMS") == 0) {
    uint16_t numberOfBlocks;

    readLine(stream, tmpString); // Number of blocks
    numberOfBlocks = stringToUint16(tmpString);
    if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
      resizeProgramMemory(numberOfBlocks);
    }

    readLine(stream, tmpString); // currentStep (pointer to block)
    if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
      currentStep = TO_PCMEMPTR(stringToUint32(tmpString));
    }
    readLine(stream, tmpString); // currentStep (offset in bytes within block)
    if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
      currentStep += stringToUint32(tmpString);
    }

    readLine(stream, tmpString); // firstFreeProgramByte (pointer to block)
    if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
      firstFreeProgramByte = TO_PCMEMPTR(stringToUint32(tmpString));
    }
    readLine(stream, tmpString); // firstFreeProgramByte (offset in bytes within block)
    if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
      firstFreeProgramByte += stringToUint32(tmpString);
    }

    readLine(stream, tmpString); // freeProgramBytes
    if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
      freeProgramBytes = stringToUint16(tmpString);
    }

    for(i=0; i<numberOfBlocks; i++) {
      readLine(stream, tmpString); // One block
      if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
        *(((uint32_t *)(beginOfProgramMemory)) + i) = stringToUint32(tmpString);
      }
    }

    scanLabelsAndPrograms();
  }

  else if(strcmp(tmpString, "OTHER_CONFIGURATION_STUFF") == 0) {
    readLine(stream, tmpString); // Number params
    numberOfRegs = stringToInt16(tmpString);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, aimBuffer); // param
      readLine(stream, tmpString); // value
      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        if(strcmp(aimBuffer, "firstGregorianDay") == 0) {
        }
        else if(strcmp(aimBuffer, "denMax") == 0) {
          denMax = stringToUint32(tmpString);
          if(denMax < 1 || denMax > MAX_DENMAX) {
            denMax = MAX_DENMAX;
          }
        }
        else if(strcmp(aimBuffer, "displayFormat") == 0) {
          displayFormat = stringToUint8(tmpString);
        }
        else if(strcmp(aimBuffer, "displayFormatDigits") == 0) {
          displayFormatDigits = stringToUint8(tmpString);
        }
        else if(strcmp(aimBuffer, "shortIntegerWordSize") == 0) {
          shortIntegerWordSize = stringToUint8(tmpString);
        }
        else if(strcmp(aimBuffer, "shortIntegerMode") == 0) {
          shortIntegerMode = stringToUint8(tmpString);
        }
        else if(strcmp(aimBuffer, "significantDigits") == 0) {
          significantDigits = stringToUint8(tmpString);
        }
        else if(strcmp(aimBuffer, "currentAngularMode") == 0) {
          currentAngularMode = stringToUint8(tmpString);
        }
        else if(strcmp(aimBuffer, "groupingGap") == 0) {
          groupingGap = stringToUint8(tmpString);
        }
        else if(strcmp(aimBuffer, "roundingMode") == 0) {
          roundingMode = stringToUint8(tmpString);
        }
        else if(strcmp(aimBuffer, "displayStack") == 0) {
          displayStack = stringToUint8(tmpString);
        }
        else if(strcmp(aimBuffer, "rngState") == 0) {
          pcg32_global.state = stringToUint64(tmpString);
          str = tmpString;
          while(*str != ' ') str++;
          while(*str == ' ') str++;
          pcg32_global.inc = stringToUint64(str);
        }
        else if(strcmp(aimBuffer, "exponentLimit") == 0) {
          exponentLimit = stringToInt16(tmpString);
        }
      }
    }
  }
}



void fnLoad(uint16_t loadMode) {
  #ifdef DMCP_BUILD
    if(f_open(BACKUP, "SAVFILES\\wp43s.sav", FA_READ) != FR_OK) {
      displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnLoad: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
        return;
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  #else // !DMCP_BUILD
    FILE *ppgm_fp;

    if((BACKUP = fopen("wp43s.sav", "rb")) == NULL) {
      displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnLoad: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
        return;
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  #endif // DMCP_BUILD

  restoreOneSection(BACKUP, loadMode); // GLOBAL_REGISTERS
  restoreOneSection(BACKUP, loadMode); // GLOBAL_FLAGS
  restoreOneSection(BACKUP, loadMode); // LOCAL_REGISTERS
  restoreOneSection(BACKUP, loadMode); // NAMED_VARIABLES
  restoreOneSection(BACKUP, loadMode); // STATISTICAL_SUMS
  restoreOneSection(BACKUP, loadMode); // SYSTEM_FLAGS
  restoreOneSection(BACKUP, loadMode); // KEYBOARD_ASSIGNMENTS
  restoreOneSection(BACKUP, loadMode); // PROGRAMS
  restoreOneSection(BACKUP, loadMode); // OTHER_CONFIGURATION_STUFF

  #ifdef DMCP_BUILD
    f_close(BACKUP);
  #else // !DMCP_BUILD
    fclose(BACKUP);
  #endif //DMCP_BUILD

  #ifndef TESTSUITE_BUILD
    if(loadMode == LM_ALL) {
      temporaryInformation = TI_BACKUP_RESTORED;
    }
  #endif // TESTSUITE_BUILD
}

#undef BACKUP
