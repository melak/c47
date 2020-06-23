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

#define BACKUP_VERSION         40  // 40 = added x max and x min
#define START_REGISTER_VALUE 1522

static void save(const void *buffer, uint32_t size, void *stream) {
  #ifdef DMCP_BUILD
    UINT bytesWritten;
    f_write(stream, buffer, size, &bytesWritten);
  #else
    fwrite(buffer, 1, size, stream);
  #endif
}



static uint32_t restore(void *buffer, uint32_t size, void *stream) {
  #ifdef DMCP_BUILD
    UINT bytesRead;
    f_read(stream, buffer, size, &bytesRead);
    return(bytesRead);
  #else
    return(fread(buffer, 1, size, stream));
  #endif
}



#ifdef PC_BUILD
void saveCalc(void) {
  uint32_t backupVersion = BACKUP_VERSION;
  uint32_t ramSize       = RAM_SIZE;
  uint32_t ramPtr;
  FILE *backup;

  backup = fopen("backup.bin", "wb");
  if(backup == NULL) {
    printf("Cannot save calc's memory in file backup.bin!\n");
    exit(0);
  }

  if(calcMode == CM_CONFIRMATION) {
    calcMode = previousCalcMode;
    refreshRegisterLine(REGISTER_X);
  }

  printf("Begin of calc's backup\n");


  save(&backupVersion,                      sizeof(backupVersion),                      backup);
  save(&ramSize,                            sizeof(ramSize),                            backup);
  save(ram,                                 TO_BYTES(RAM_SIZE),                         backup);
  save(freeBlocks,                          MAX_FREE_BLOCKS * sizeof(freeBlock_t),      backup);
  save(&numberOfFreeBlocks,                 sizeof(numberOfFreeBlocks),                 backup);
  save(globalFlags,                         sizeof(globalFlags),                        backup);
  save(tmpStr3000,                          TMP_STR_LENGTH,                             backup);
  save(errorMessage,                        ERROR_MESSAGE_LENGTH,                       backup);
  save(aimBuffer,                           AIM_BUFFER_LENGTH,                          backup);
  save(nimBuffer,                           NIM_BUFFER_LENGTH,                          backup);
  save(nimBufferDisplay,                    NIM_BUFFER_LENGTH,                          backup);
  save(tamBuffer,                           TAM_BUFFER_LENGTH,                          backup);
  save(asmBuffer,                           sizeof(asmBuffer),                          backup);
  save(oldTime,                             8,                                          backup);
  save(dateTimeString,                      12,                                         backup);
  save(softmenuStack,                       sizeof(softmenuStack),                      backup);
  save(reg,                                 sizeof(reg),                                backup);
  save(savedStackRegister,                  sizeof(savedStackRegister),                 backup);
  save(kbd_usr,                             sizeof(kbd_usr),                            backup);
  save(transitionSystemOperation,           sizeof(transitionSystemOperation),          backup);
  save(&tamFunction,                        sizeof(tamFunction),                        backup);
  save(&tamNumber,                          sizeof(tamNumber),                          backup);
  save(&tamNumberMin,                       sizeof(tamNumberMin),                       backup);
  save(&tamNumberMax,                       sizeof(tamNumberMax),                       backup);
  save(&tamDigit,                           sizeof(tamDigit),                           backup);
  save(&tamOperation,                       sizeof(tamOperation),                       backup);
  save(&tamLetteredRegister,                sizeof(tamLetteredRegister),                backup);
  save(&tamCurrentOperation,                sizeof(tamCurrentOperation),                backup);
  save(&rbrRegister,                        sizeof(rbrRegister),                        backup);
  save(&numberOfLocalFlags,                 sizeof(numberOfLocalFlags),                 backup);
  ramPtr = TO_WP43SMEMPTR(allLocalRegisterPointer);
  save(&ramPtr,                             sizeof(ramPtr),                             backup);
  ramPtr = TO_WP43SMEMPTR(allNamedVariablePointer);
  save(&ramPtr,                             sizeof(ramPtr),                             backup);
  ramPtr = TO_WP43SMEMPTR(statisticalSumsPointer);
  save(&ramPtr,                             sizeof(ramPtr),                             backup);
  save(&programCounter,                     sizeof(programCounter),                     backup);
  save(&xCursor,                            sizeof(xCursor),                            backup);
  save(&yCursor,                            sizeof(yCursor),                            backup);
  save(&firstGregorianDay,                  sizeof(firstGregorianDay),                  backup);
  save(&denMax,                             sizeof(denMax),                             backup);
  save(&softmenuStackPointer,               sizeof(softmenuStackPointer),               backup);
  save(&softmenuStackPointerBeforeAIM,      sizeof(softmenuStackPointerBeforeAIM),      backup);
  save(&softmenuStackPointerBeforeBrowser,  sizeof(softmenuStackPointerBeforeBrowser),  backup);
  save(&transitionSystemState,              sizeof(transitionSystemState),              backup);
  save(&cursorBlinkCounter,                 sizeof(cursorBlinkCounter),                 backup);
  save(&currentRegisterBrowserScreen,       sizeof(currentRegisterBrowserScreen),       backup);
  save(&currentFntScr,                      sizeof(currentFntScr),                      backup);
  save(&currentFlgScr,                      sizeof(currentFlgScr),                      backup);
  save(&displayFormat,                      sizeof(displayFormat),                      backup);
  save(&displayFormatDigits,                sizeof(displayFormatDigits),                backup);
  save(&shortIntegerWordSize,               sizeof(shortIntegerWordSize),               backup);
  save(&significantDigits,                  sizeof(significantDigits),                  backup);
  save(&shortIntegerMode,                   sizeof(shortIntegerMode),                   backup);
  save(&currentAngularMode,                 sizeof(currentAngularMode),                 backup);
  save(&groupingGap,                        sizeof(groupingGap),                        backup);
  save(&curveFitting,                       sizeof(curveFitting),                       backup);
  save(&roundingMode,                       sizeof(roundingMode),                       backup);
  save(&calcMode,                           sizeof(calcMode),                           backup);
  save(&nextChar,                           sizeof(nextChar),                           backup);
  save(&alphaCase,                          sizeof(alphaCase),                          backup);
  save(&hourGlassIconEnabled,               sizeof(hourGlassIconEnabled),               backup);
  save(&watchIconEnabled,                   sizeof(watchIconEnabled),                   backup);
  save(&serialIOIconEnabled,                sizeof(serialIOIconEnabled),                backup);
  save(&printerIconEnabled,                 sizeof(printerIconEnabled),                 backup);
  save(&cursorEnabled,                      sizeof(cursorEnabled),                      backup);
  save(&cursorFont,                         sizeof(cursorFont),                         backup);
  save(&savedStackLiftEnabled,              sizeof(savedStackLiftEnabled),              backup);
  save(&rbr1stDigit,                        sizeof(rbr1stDigit),                        backup);
  save(&shiftF,                             sizeof(shiftF),                             backup);
  save(&shiftG,                             sizeof(shiftG),                             backup);
  save(&shiftStateChanged,                  sizeof(shiftStateChanged),                  backup);
  save(&tamMode,                            sizeof(tamMode),                            backup);
  save(&rbrMode,                            sizeof(rbrMode),                            backup);
  save(&showContent,                        sizeof(showContent),                        backup);
  save(&numScreensNumericFont,              sizeof(numScreensNumericFont),              backup);
  save(&numLinesNumericFont,                sizeof(numLinesNumericFont),                backup);
  save(&numLinesStandardFont,               sizeof(numLinesStandardFont),               backup);
  save(&numScreensStandardFont,             sizeof(numScreensStandardFont),             backup);
  save(&previousCalcMode,                   sizeof(previousCalcMode),                   backup);
  save(&lastErrorCode,                      sizeof(lastErrorCode),                      backup);
  save(&nimNumberPart,                      sizeof(nimNumberPart),                      backup);
  save(&displayStack,                       sizeof(displayStack),                       backup);
  save(&hexDigits,                          sizeof(hexDigits),                          backup);
  save(&errorMessageRegisterLine,           sizeof(errorMessageRegisterLine),           backup);
  save(&errorRegisterLine,                  sizeof(errorRegisterLine),                  backup);
  save(&shortIntegerMask,                   sizeof(shortIntegerMask),                   backup);
  save(&shortIntegerSignBit,                sizeof(shortIntegerSignBit),                backup);
  save(&temporaryInformation,               sizeof(temporaryInformation),               backup);
  save(&glyphNotFound,                      sizeof(glyphNotFound),                      backup);
  save(&allowScreenUpdate,                  sizeof(allowScreenUpdate),                  backup);
  save(&funcOK,                             sizeof(funcOK),                             backup);
  save(&screenChange,                       sizeof(screenChange),                       backup);
  save(&exponentSignLocation,               sizeof(exponentSignLocation),               backup);
  save(&denominatorLocation,                sizeof(denominatorLocation),                backup);
  save(&imaginaryExponentSignLocation,      sizeof(imaginaryExponentSignLocation),      backup);
  save(&imaginaryMantissaSignLocation,      sizeof(imaginaryMantissaSignLocation),      backup);
  save(&lineTWidth,                         sizeof(lineTWidth),                         backup);
  save(&lastIntegerBase,                    sizeof(lastIntegerBase),                    backup);
  save(&wp43sMemInBytes,                    sizeof(wp43sMemInBytes),                    backup);
  save(&gmpMemInBytes,                      sizeof(gmpMemInBytes),                      backup);
  save(&alphaSelectionMenu,                 sizeof(alphaSelectionMenu),                 backup);
  save(&lastFcnsMenuPos,                    sizeof(lastFcnsMenuPos),                    backup);
  save(&lastMenuMenuPos,                    sizeof(lastMenuMenuPos),                    backup);
  save(&lastCnstMenuPos,                    sizeof(lastCnstMenuPos),                    backup);
  save(&lastSyFlMenuPos,                    sizeof(lastSyFlMenuPos),                    backup);
  save(&lastAIntMenuPos,                    sizeof(lastAIntMenuPos),                    backup);
  save(&lgCatalogSelection,                 sizeof(lgCatalogSelection),                 backup);
  save(displayValueX,                       sizeof(displayValueX),                      backup);
  save(&pcg32_global,                       sizeof(pcg32_global),                       backup);
  save(&exponentLimit,                      sizeof(exponentLimit),                      backup);
  save(&keyActionProcessed,                 sizeof(keyActionProcessed),                 backup);
  save(&systemFlags,                        sizeof(systemFlags),                        backup);

  fclose(backup);
  printf("End of calc's backup\n");
}



void restoreCalc(void) {
  uint32_t backupVersion, ramSize, ramPtr;
  FILE *backup;

  backup = fopen("backup.bin", "rb");
  if(backup == NULL) {
    printf("Cannot restore calc's memory from file backup.bin! Performing RESET\n");
    fnReset(CONFIRMED);
    return;
  }

  restore(&backupVersion,                      sizeof(backupVersion),                      backup);
  restore(&ramSize,                            sizeof(ramSize),                            backup);
  if(backupVersion != BACKUP_VERSION || ramSize != RAM_SIZE) {
    fclose(backup);

    printf("Cannot restore calc's memory from file backup.bin! File backup.bin is from another backup version. Performing RESET\n");
    printf("               Backup file      Program\n");
    printf("backupVersion  %6u           %6u\n", backupVersion, BACKUP_VERSION);
    printf("ramSize blocks %6u           %6u\n", ramSize, RAM_SIZE);
    printf("ramSize bytes  %6u           %6u\n", TO_BYTES(ramSize), TO_BYTES(RAM_SIZE));

    fnReset(CONFIRMED);
    return;
  }
  else {
    printf("Begin of calc's restoration\n");

    restore(ram,                                 TO_BYTES(RAM_SIZE),                         backup);
    restore(freeBlocks,                          MAX_FREE_BLOCKS * sizeof(freeBlock_t),      backup);
    restore(&numberOfFreeBlocks,                 sizeof(numberOfFreeBlocks),                 backup);
    restore(globalFlags,                         sizeof(globalFlags),                        backup);
    restore(tmpStr3000,                          TMP_STR_LENGTH,                             backup);
    restore(errorMessage,                        ERROR_MESSAGE_LENGTH,                       backup);
    restore(aimBuffer,                           AIM_BUFFER_LENGTH,                          backup);
    restore(nimBuffer,                           NIM_BUFFER_LENGTH,                          backup);
    restore(nimBufferDisplay,                    NIM_BUFFER_LENGTH,                          backup);
    restore(tamBuffer,                           TAM_BUFFER_LENGTH,                          backup);
    restore(asmBuffer,                           sizeof(asmBuffer),                          backup);
    restore(oldTime,                             8,                                          backup);
    restore(dateTimeString,                      12,                                         backup);
    restore(softmenuStack,                       sizeof(softmenuStack),                      backup);
    restore(reg,                                 sizeof(reg),                                backup);
    restore(savedStackRegister,                  sizeof(savedStackRegister),                 backup);
    restore(kbd_usr,                             sizeof(kbd_usr),                            backup);
    restore(transitionSystemOperation,           sizeof(transitionSystemOperation),          backup);
    restore(&tamFunction,                        sizeof(tamFunction),                        backup);
    restore(&tamNumber,                          sizeof(tamNumber),                          backup);
    restore(&tamNumberMin,                       sizeof(tamNumberMin),                       backup);
    restore(&tamNumberMax,                       sizeof(tamNumberMax),                       backup);
    restore(&tamDigit,                           sizeof(tamDigit),                           backup);
    restore(&tamOperation,                       sizeof(tamOperation),                       backup);
    restore(&tamLetteredRegister,                sizeof(tamLetteredRegister),                backup);
    restore(&tamCurrentOperation,                sizeof(tamCurrentOperation),                backup);
    restore(&rbrRegister,                        sizeof(rbrRegister),                        backup);
    restore(&numberOfLocalFlags,                 sizeof(numberOfLocalFlags),                 backup);
    restore(&ramPtr,                             sizeof(ramPtr),                             backup);
    allLocalRegisterPointer = TO_PCMEMPTR(ramPtr);
    restore(&ramPtr,                             sizeof(ramPtr),                             backup);
    allNamedVariablePointer = TO_PCMEMPTR(ramPtr);
    restore(&ramPtr,                             sizeof(ramPtr),                             backup);
    statisticalSumsPointer = TO_PCMEMPTR(ramPtr);
    restore(&programCounter,                     sizeof(programCounter),                     backup);
    restore(&xCursor,                            sizeof(xCursor),                            backup);
    restore(&yCursor,                            sizeof(yCursor),                            backup);
    restore(&firstGregorianDay,                  sizeof(firstGregorianDay),                  backup);
    restore(&denMax,                             sizeof(denMax),                             backup);
    restore(&softmenuStackPointer,               sizeof(softmenuStackPointer),               backup);
    restore(&softmenuStackPointerBeforeAIM,      sizeof(softmenuStackPointerBeforeAIM),      backup);
    restore(&softmenuStackPointerBeforeBrowser,  sizeof(softmenuStackPointerBeforeBrowser),  backup);
    restore(&transitionSystemState,              sizeof(transitionSystemState),              backup);
    restore(&cursorBlinkCounter,                 sizeof(cursorBlinkCounter),                 backup);
    restore(&currentRegisterBrowserScreen,       sizeof(currentRegisterBrowserScreen),       backup);
    restore(&currentFntScr,                      sizeof(currentFntScr),                      backup);
    restore(&currentFlgScr,                      sizeof(currentFlgScr),                      backup);
    restore(&displayFormat,                      sizeof(displayFormat),                      backup);
    restore(&displayFormatDigits,                sizeof(displayFormatDigits),                backup);
    restore(&shortIntegerWordSize,               sizeof(shortIntegerWordSize),               backup);
    restore(&significantDigits,                  sizeof(significantDigits),                  backup);
    restore(&shortIntegerMode,                   sizeof(shortIntegerMode),                   backup);
    restore(&currentAngularMode,                 sizeof(currentAngularMode),                 backup);
    restore(&groupingGap,                        sizeof(groupingGap),                        backup);
    restore(&curveFitting,                       sizeof(curveFitting),                       backup);
    restore(&roundingMode,                       sizeof(roundingMode),                       backup);
    restore(&calcMode,                           sizeof(calcMode),                           backup);
    restore(&nextChar,                           sizeof(nextChar),                           backup);
    restore(&alphaCase,                          sizeof(alphaCase),                          backup);
    restore(&hourGlassIconEnabled,               sizeof(hourGlassIconEnabled),               backup);
    restore(&watchIconEnabled,                   sizeof(watchIconEnabled),                   backup);
    restore(&serialIOIconEnabled,                sizeof(serialIOIconEnabled),                backup);
    restore(&printerIconEnabled,                 sizeof(printerIconEnabled),                 backup);
    restore(&cursorEnabled,                      sizeof(cursorEnabled),                      backup);
    restore(&cursorFont,                         sizeof(cursorFont),                         backup);
    restore(&savedStackLiftEnabled,              sizeof(savedStackLiftEnabled),              backup);
    restore(&rbr1stDigit,                        sizeof(rbr1stDigit),                        backup);
    restore(&shiftF,                             sizeof(shiftF),                             backup);
    restore(&shiftG,                             sizeof(shiftG),                             backup);
    restore(&shiftStateChanged,                  sizeof(shiftStateChanged),                  backup);
    restore(&tamMode,                            sizeof(tamMode),                            backup);
    restore(&rbrMode,                            sizeof(rbrMode),                            backup);
    restore(&showContent,                        sizeof(showContent),                        backup);
    restore(&numScreensNumericFont,              sizeof(numScreensNumericFont),              backup);
    restore(&numLinesNumericFont,                sizeof(numLinesNumericFont),                backup);
    restore(&numLinesStandardFont,               sizeof(numLinesStandardFont),               backup);
    restore(&numScreensStandardFont,             sizeof(numScreensStandardFont),             backup);
    restore(&previousCalcMode,                   sizeof(previousCalcMode),                   backup);
    restore(&lastErrorCode,                      sizeof(lastErrorCode),                      backup);
    restore(&nimNumberPart,                      sizeof(nimNumberPart),                      backup);
    restore(&displayStack,                       sizeof(displayStack),                       backup);
    restore(&hexDigits,                          sizeof(hexDigits),                          backup);
    restore(&errorMessageRegisterLine,           sizeof(errorMessageRegisterLine),           backup);
    restore(&errorRegisterLine,                  sizeof(errorRegisterLine),                  backup);
    restore(&shortIntegerMask,                   sizeof(shortIntegerMask),                   backup);
    restore(&shortIntegerSignBit,                sizeof(shortIntegerSignBit),                backup);
    restore(&temporaryInformation,               sizeof(temporaryInformation),               backup);

    restore(&glyphNotFound,                      sizeof(glyphNotFound),                      backup);
    glyphNotFound.data   = malloc(38);
    xcopy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8", 38);

    restore(&allowScreenUpdate,                  sizeof(allowScreenUpdate),                  backup);
    restore(&funcOK,                             sizeof(funcOK),                             backup);
    restore(&screenChange,                       sizeof(screenChange),                       backup);
    restore(&exponentSignLocation,               sizeof(exponentSignLocation),               backup);
    restore(&denominatorLocation,                sizeof(denominatorLocation),                backup);
    restore(&imaginaryExponentSignLocation,      sizeof(imaginaryExponentSignLocation),      backup);
    restore(&imaginaryMantissaSignLocation,      sizeof(imaginaryMantissaSignLocation),      backup);
    restore(&lineTWidth,                         sizeof(lineTWidth),                         backup);
    restore(&lastIntegerBase,                    sizeof(lastIntegerBase),                    backup);
    restore(&wp43sMemInBytes,                    sizeof(wp43sMemInBytes),                    backup);
    restore(&gmpMemInBytes,                      sizeof(gmpMemInBytes),                      backup);
    restore(&alphaSelectionMenu,                 sizeof(alphaSelectionMenu),                 backup);
    restore(&lastFcnsMenuPos,                    sizeof(lastFcnsMenuPos),                    backup);
    restore(&lastMenuMenuPos,                    sizeof(lastMenuMenuPos),                    backup);
    restore(&lastCnstMenuPos,                    sizeof(lastCnstMenuPos),                    backup);
    restore(&lastSyFlMenuPos,                    sizeof(lastSyFlMenuPos),                    backup);
    restore(&lastAIntMenuPos,                    sizeof(lastAIntMenuPos),                    backup);
    restore(&lgCatalogSelection,                 sizeof(lgCatalogSelection),                 backup);
    restore(displayValueX,                       sizeof(displayValueX),                      backup);
    restore(&pcg32_global,                       sizeof(pcg32_global),                       backup);
    restore(&exponentLimit,                      sizeof(exponentLimit),                      backup);
    restore(&keyActionProcessed,                 sizeof(keyActionProcessed),                 backup);
    restore(&systemFlags,                        sizeof(systemFlags),                        backup);

    fclose(backup);
    printf("End of calc's restoration\n");

    #if (DEBUG_REGISTER_L == 1)
      //if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
        refreshRegisterLine(REGISTER_X); // to show L register
      //}
    #endif

    if(calcMode == CM_NORMAL)                calcModeNormalGui();
    else if(calcMode == CM_AIM)              calcModeAimGui();
    else if(calcMode == CM_TAM)              calcModeTamGui();
    else if(calcMode == CM_NIM)              calcModeNormalGui();
    else if(calcMode == CM_ASM)              calcModeAsm();
    else if(calcMode == CM_ASM_OVER_TAM)    {calcModeAsm(); calcMode = CM_ASM_OVER_TAM; clearSystemFlag(FLAG_ALPHA);}
    else if(calcMode == CM_ASM_OVER_AIM)    {calcModeAsm(); calcMode = CM_ASM_OVER_AIM; clearSystemFlag(FLAG_ALPHA);}
    else if(calcMode == CM_REGISTER_BROWSER) calcModeNormalGui();
    else if(calcMode == CM_FLAG_BROWSER)     calcModeNormalGui();
    else if(calcMode == CM_FONT_BROWSER)     calcModeNormalGui();
    else {
      sprintf(errorMessage, "In function restoreCalc: %" FMT8U " is an unexpected value for calcMode", calcMode);
      displayBugScreen(errorMessage);
    }

    clearScreen(true, true, true);
    refreshStatusBar();
    refreshStack();
    showSoftmenuCurrentPart();

    if(getSystemFlag(FLAG_ASLIFT)) {
      STACK_LIFT_ENABLE;
    }
    else {
     STACK_LIFT_DISABLE;
    }
  }
}
#endif


static void registerToSaveString(calcRegister_t regist) {
  longInteger_t lgInt;
  int16_t sign;
  uint64_t value;
  char *str, *cfg;

  switch(getRegisterDataType(regist)) {
    case dtLongInteger:
      convertLongIntegerRegisterToLongInteger(regist, lgInt);
      longIntegerToAllocatedString(lgInt, tmpStr3000 + START_REGISTER_VALUE, TMP_STR_LENGTH - START_REGISTER_VALUE - 1);
      longIntegerFree(lgInt);
      strcpy(nimBuffer, "LonI");
      break;

    case dtString:
      stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)(tmpStr3000 + START_REGISTER_VALUE));
      strcpy(nimBuffer, "Stri");
      break;

    case dtShortInteger:
      convertShortIntegerRegisterToUInt64(regist, &sign, &value);
      sprintf(tmpStr3000 + START_REGISTER_VALUE, "%c%" FMT64U " %" FMT32U, sign ? '-' : '+', value, getRegisterShortIntegerBase(regist));
      strcpy(nimBuffer, "ShoI");
      break;

    case dtReal34:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpStr3000 + START_REGISTER_VALUE);
      switch(getRegisterAngularMode(regist)) {
        case AM_DEGREE:
          strcpy(nimBuffer, "Real:DEG");
          break;

        case AM_GRAD:
          strcpy(nimBuffer, "Real:GRAD");
          break;

        case AM_RADIAN:
          strcpy(nimBuffer, "Real:RAD");
          break;

        case AM_MULTPI:
          strcpy(nimBuffer, "Real:MULTPI");
          break;

        case AM_DMS:
          strcpy(nimBuffer, "Real:DMS");
          break;

        case AM_NONE:
          strcpy(nimBuffer, "Real");
          break;

        default:
          strcpy(nimBuffer, "Real:???");
          break;
      }
      break;

    case dtComplex34:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpStr3000 + START_REGISTER_VALUE);
      strcat(tmpStr3000 + START_REGISTER_VALUE, " ");
      real34ToString(REGISTER_IMAG34_DATA(regist), tmpStr3000 + START_REGISTER_VALUE + strlen(tmpStr3000 + START_REGISTER_VALUE));
      strcpy(nimBuffer, "Cplx");
      break;

    case dtConfig:
      for(str=tmpStr3000 + START_REGISTER_VALUE, cfg=(char *)REGISTER_CONFIG_DATA(regist), value=0; value<sizeof(dtConfigDescriptor_t); value++, cfg++, str+=2) {
        sprintf(str, "%02X", *cfg);
      }
      strcpy(nimBuffer, "Conf");
      break;

    default:
      strcpy(tmpStr3000 + START_REGISTER_VALUE, "???");
      strcpy(nimBuffer, "????");
  }
}



void fnSave(uint16_t unusedParamButMandatory) {
  calcRegister_t regist;
  uint32_t i;

  #ifdef DMCP_BUILD
    FIL backupStruct;
    FIL *backup = &backupStruct;
    FRESULT result;

    sys_disk_write_enable(1);
    check_create_dir("SAVFILES");
    result = f_open(backup, "SAVFILES\\wp43s.sav", FA_CREATE_ALWAYS | FA_WRITE);
    if(result != FR_OK) {
      sys_disk_write_enable(0);
  #else
    FILE *backup;

    backup = fopen("wp43s.sav", "wb");
    if(backup == NULL) {
      printf("Cannot SAVE in file wp43s.sav!\n");
  #endif
      return;
    }

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wrestrict"

  // Global registers
  sprintf(tmpStr3000, "GLOBAL_REGISTERS\n%" FMT16U "\n", FIRST_LOCAL_REGISTER);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
    registerToSaveString(regist);
    sprintf(tmpStr3000, "R%03" FMT16S "\n%s\n%s\n", regist, nimBuffer, tmpStr3000 + START_REGISTER_VALUE);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }

  // Global flags
  strcpy(tmpStr3000, "GLOBAL_FLAGS\n");
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "%" FMT16U " %" FMT16U " %" FMT16U " %" FMT16U " %" FMT16U " %" FMT16U " %" FMT16U "\n",
                       globalFlags[0],
                                   globalFlags[1],
                                               globalFlags[2],
                                                           globalFlags[3],
                                                                       globalFlags[4],
                                                                                   globalFlags[5],
                                                                                               globalFlags[6]);
  save(tmpStr3000, strlen(tmpStr3000), backup);

  // Local registers
  sprintf(tmpStr3000, "LOCAL_REGISTERS\n%" FMT16U "\n", allLocalRegisterPointer->numberOfLocalRegisters);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(i=0; i<allLocalRegisterPointer->numberOfLocalRegisters; i++) {
    registerToSaveString(FIRST_LOCAL_REGISTER + i);
    sprintf(tmpStr3000, "R.%02" FMT32U "\n%s\n%s\n", i, nimBuffer, tmpStr3000 + START_REGISTER_VALUE);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }

  // Local flags
  if(allLocalRegisterPointer->numberOfLocalRegisters) {
    sprintf(tmpStr3000, "LOCAL_FLAGS\n%" FMT16U "\n", allLocalRegisterPointer->localFlags);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }

  // Named variables
  sprintf(tmpStr3000, "NAMED_VARIABLES\n%" FMT16U "\n", allNamedVariablePointer->numberOfNamedVariables);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(i=0; i<allNamedVariablePointer->numberOfNamedVariables; i++) {
    registerToSaveString(FIRST_NAMED_VARIABLE + i);
    sprintf(tmpStr3000, "%s\n%s\n%s\n", "name", nimBuffer, tmpStr3000 + START_REGISTER_VALUE);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }

  // Statistical sums
  sprintf(tmpStr3000, "STATISTICAL_SUMS\n%" FMT16U "\n", statisticalSumsPointer ? NUMBER_OF_STATISTICAL_SUMS : 0);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(i=0; i<(statisticalSumsPointer ? NUMBER_OF_STATISTICAL_SUMS : 0); i++) {
    realToString(statisticalSumsPointer + i, tmpStr3000 + START_REGISTER_VALUE);
    sprintf(tmpStr3000, "%s\n", tmpStr3000 + START_REGISTER_VALUE);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }
  #pragma GCC diagnostic pop

  // System flags
  sprintf(tmpStr3000, "SYSTEM_FLAGS\n%" FMT64U "\n", systemFlags);
  save(tmpStr3000, strlen(tmpStr3000), backup);

  // Keyboard assignments
  sprintf(tmpStr3000, "KEYBOARD_ASSIGNMENTS\n37\n");
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(i=0; i<37; i++) {
    sprintf(tmpStr3000, "%" FMT16S " %" FMT16S " %" FMT16S " %" FMT16S " %" FMT16S " %" FMT16S " %" FMT16S " %" FMT16S " %" FMT16S "\n",
                         kbd_usr[i].keyId,
                                     kbd_usr[i].primary,
                                                 kbd_usr[i].fShifted,
                                                             kbd_usr[i].gShifted,
                                                                         kbd_usr[i].keyLblAim,
                                                                                     kbd_usr[i].primaryAim,
                                                                                                 kbd_usr[i].fShiftedAim,
                                                                                                             kbd_usr[i].gShiftedAim,
                                                                                                                         kbd_usr[i].primaryTam);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  }

  // Other configuration stuff
  sprintf(tmpStr3000, "OTHER_CONFIGURATION_STUFF\n14\n");
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "firstGregorianDay\n1582 10 15\n");
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "denMax\n%" FMT32U "\n", denMax);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "displayFormat\n%" FMT8U "\n", displayFormat);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "displayFormatDigits\n%" FMT8U "\n", displayFormatDigits);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "shortIntegerWordSize\n%" FMT8U "\n", shortIntegerWordSize);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "shortIntegerMode\n%" FMT8U "\n", shortIntegerMode);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "significantDigits\n%" FMT8U "\n", significantDigits);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "currentAngularMode\n%" FMT8U "\n", currentAngularMode);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "groupingGap\n%" FMT8U "\n", groupingGap);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "curveFitting\n%" FMT8U "\n", curveFitting);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "roundingMode\n%" FMT8U "\n", roundingMode);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "displayStack\n%" FMT8U "\n", displayStack);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "rngState\n%" FMT64U " %" FMT64U "\n", pcg32_global.state, pcg32_global.inc);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "exponentLimit\n%" FMT16S "\n", exponentLimit);
  save(tmpStr3000, strlen(tmpStr3000), backup);

  #ifdef DMCP_BUILD
    f_close(backup);
    sys_disk_write_enable(0);
  #else
    fclose(backup);
  #endif

  temporaryInformation = TI_SAVED;
  refreshStack();
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

  readLine(stream, tmpStr3000);

  if(strcmp(tmpStr3000, "GLOBAL_REGISTERS") == 0) {
    readLine(stream, tmpStr3000); // Number of global registers
    numberOfRegs = stringToInt16(tmpStr3000);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpStr3000); // Register number
      regist = stringToInt16(tmpStr3000 + 1);
      readLine(stream, nimBuffer); // Register data type
      readLine(stream, tmpStr3000); // Register value

      if(loadMode == LM_ALL || (loadMode == LM_REGISTERS && regist < REGISTER_X) || (loadMode == LM_SYSTEM_STATE && regist >= REGISTER_X)) {
        restoreRegister(regist, nimBuffer, tmpStr3000);
      }
    }
  }

  else if(strcmp(tmpStr3000, "GLOBAL_FLAGS") == 0) {
    readLine(stream, tmpStr3000); // Global flags
    if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
      str = tmpStr3000;
      globalFlags[0] = stringToUint16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[1] = stringToUint16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[2] = stringToUint16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[3] = stringToUint16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[4] = stringToUint16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[5] = stringToUint16(str);

      while(*str != ' ') str++;
      while(*str == ' ') str++;
      globalFlags[6] = stringToUint16(str);
    }
  }

  else if(strcmp(tmpStr3000, "LOCAL_REGISTERS") == 0) {
    readLine(stream, tmpStr3000); // Number of local registers
    numberOfRegs = stringToInt16(tmpStr3000);
    if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
      allocateLocalRegisters(numberOfRegs);
    }

    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpStr3000); // Register number
      regist = stringToInt16(tmpStr3000 + 2) + FIRST_LOCAL_REGISTER;
      readLine(stream, nimBuffer); // Register data type
      readLine(stream, tmpStr3000); // Register value

      if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
        restoreRegister(regist, nimBuffer, tmpStr3000);
      }
    }

    if(numberOfRegs > 0) {
      readLine(stream, tmpStr3000); // LOCAL_FLAGS
      readLine(stream, tmpStr3000); // LOCAL_FLAGS
      if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
        allLocalRegisterPointer->localFlags = stringToUint16(tmpStr3000);
      }
    }
  }

  else if(strcmp(tmpStr3000, "NAMED_VARIABLES") == 0) {
    readLine(stream, tmpStr3000); // Number of named variables
    numberOfRegs = stringToInt16(tmpStr3000);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpStr3000 + 2900); // Variable name
      readLine(stream, nimBuffer); // Variable data type
      readLine(stream, tmpStr3000); // Variable value

      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        printf("Variable %s ", tmpStr3000 + 2900);
        printf("%s = ", nimBuffer);
        printf("%s\n", tmpStr3000);
      }
    }
  }

  else if(strcmp(tmpStr3000, "STATISTICAL_SUMS") == 0) {
    readLine(stream, tmpStr3000); // Number of statistical sums
    numberOfRegs = stringToInt16(tmpStr3000);
    if(numberOfRegs > 0 && (loadMode == LM_ALL || loadMode == LM_SUMS)) {
      initStatisticalSums();
    }

    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpStr3000); // statistical sum
      if(loadMode == LM_ALL || loadMode == LM_SUMS) {
        stringToReal(tmpStr3000, (real_t *)(statisticalSumsPointer + REAL_SIZE * i), &ctxtReal75);
      }
    }
  }

  else if(strcmp(tmpStr3000, "SYSTEM_FLAGS") == 0) {
    readLine(stream, tmpStr3000); // Global flags
    if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
      systemFlags = stringToUint64(tmpStr3000);
    }
  }

  else if(strcmp(tmpStr3000, "KEYBOARD_ASSIGNMENTS") == 0) {
    readLine(stream, tmpStr3000); // Number of keys
    numberOfRegs = stringToInt16(tmpStr3000);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpStr3000); // key
      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        str = tmpStr3000;
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

  else if(strcmp(tmpStr3000, "OTHER_CONFIGURATION_STUFF") == 0) {
    readLine(stream, tmpStr3000); // Number params
    numberOfRegs = stringToInt16(tmpStr3000);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, nimBuffer); // param
      readLine(stream, tmpStr3000); // value
      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        if(strcmp(nimBuffer, "firstGregorianDay") == 0) {
        }
        else if(strcmp(nimBuffer, "denMax") == 0) {
          denMax = stringToUint32(tmpStr3000);
          if(denMax < 1 || denMax > MAX_DENMAX) {
            denMax = MAX_DENMAX;
          }
        }
        else if(strcmp(nimBuffer, "displayFormat") == 0) {
          displayFormat = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "displayFormatDigits") == 0) {
          displayFormatDigits = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "shortIntegerWordSize") == 0) {
          shortIntegerWordSize = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "shortIntegerMode") == 0) {
          shortIntegerMode = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "significantDigits") == 0) {
          significantDigits = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "currentAngularMode") == 0) {
          currentAngularMode = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "groupingGap") == 0) {
          groupingGap = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "curveFitting") == 0) {
          curveFitting = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "roundingMode") == 0) {
          roundingMode = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "displayStack") == 0) {
          displayStack = stringToUint8(tmpStr3000);
        }
        else if(strcmp(nimBuffer, "rngState") == 0) {
          pcg32_global.state = stringToUint64(tmpStr3000);
          str = tmpStr3000;
          while(*str != ' ') str++;
          while(*str == ' ') str++;
          pcg32_global.inc = stringToUint64(str);
        }
        else if(strcmp(nimBuffer, "exponentLimit") == 0) {
          exponentLimit = stringToInt16(tmpStr3000);
        }
      }
    }
  }
}



void fnLoad(uint16_t loadMode) {
  #ifdef DMCP_BUILD
    FIL backupStruct;
    FIL *backup = &backupStruct;

    if(f_open(backup, "SAVFILES\\wp43s.sav", FA_READ) != FR_OK) {
  #else
    FILE *backup;

    if((backup = fopen("wp43s.sav", "rb")) == NULL) {
  #endif
      displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnLoad: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
        return;
      #endif
    }

  restoreOneSection(backup, loadMode); // GLOBAL_REGISTERS
  restoreOneSection(backup, loadMode); // GLOBAL_FLAGS
  restoreOneSection(backup, loadMode); // LOCAL_REGISTERS
  restoreOneSection(backup, loadMode); // NAMED_VARIABLES
  restoreOneSection(backup, loadMode); // STATISTICAL_SUMS
  restoreOneSection(backup, loadMode); // SYSTEM_FLAGS
  restoreOneSection(backup, loadMode); // KEYBOARD_ASSIGNMENTS
  restoreOneSection(backup, loadMode); // OTHER_CONFIGURATION_STUFF

  #ifdef DMCP_BUILD
    f_close(backup);
  #else
    fclose(backup);
  #endif

  #ifndef TESTSUITE_BUILD
    clearScreen(true, true, true);
    refreshStatusBar();
    if(loadMode == LM_ALL) {
      temporaryInformation = TI_BACKUP_RESTORED;
    }
    refreshStack();
    showSoftmenuCurrentPart();
  #endif

  if(getSystemFlag(FLAG_ASLIFT)) {
    STACK_LIFT_ENABLE;
  }
  else {
   STACK_LIFT_DISABLE;
  }
}
