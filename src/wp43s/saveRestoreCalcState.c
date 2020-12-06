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

#define BACKUP_VERSION          1  // removed allowScreenUpdate
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
    refreshScreen();
  }

  printf("Begin of calc's backup\n");


  save(&backupVersion,                      sizeof(backupVersion),                      backup);
  save(&ramSize,                            sizeof(ramSize),                            backup);
  save(ram,                                 TO_BYTES(RAM_SIZE),                         backup);
  save(freeMemoryRegions,                   sizeof(freeMemoryRegions),                  backup);
  save(&numberOfFreeMemoryRegions,          sizeof(numberOfFreeMemoryRegions),          backup);
  save(globalFlags,                         sizeof(globalFlags),                        backup);
  save(tmpStr3000,                          sizeof(tmpStr3000),                         backup);
  save(errorMessage,                        sizeof(errorMessage),                       backup);
  save(aimBuffer,                           sizeof(aimBuffer),                          backup);
  save(nimBufferDisplay,                    sizeof(nimBufferDisplay),                   backup);
  save(tamBuffer,                           sizeof(tamBuffer),                          backup);
  save(asmBuffer,                           sizeof(asmBuffer),                          backup);
  save(oldTime,                             sizeof(oldTime),                            backup);
  save(dateTimeString,                      sizeof(dateTimeString),                     backup);
  save(softmenuStack,                       sizeof(softmenuStack),                      backup);
  save(reg,                                 sizeof(reg),                                backup);
  save(savedStackRegister,                  sizeof(savedStackRegister),                 backup);
  save(kbd_usr,                             sizeof(kbd_usr),                            backup);
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
  ramPtr = TO_WP43SMEMPTR(savedStatisticalSumsPointer);
  save(&ramPtr,                             sizeof(ramPtr),                             backup);
  save(&programCounter,                     sizeof(programCounter),                     backup);
  save(&xCursor,                            sizeof(xCursor),                            backup);
  save(&yCursor,                            sizeof(yCursor),                            backup);
  save(&firstGregorianDay,                  sizeof(firstGregorianDay),                  backup);
  save(&denMax,                             sizeof(denMax),                             backup);
  save(&softmenuStackPointer,               sizeof(softmenuStackPointer),               backup);
  save(&softmenuStackPointerBeforeAIM,      sizeof(softmenuStackPointerBeforeAIM),      backup);
  save(&transitionSystemState,              sizeof(transitionSystemState),              backup);
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
  save(&rbr1stDigit,                        sizeof(rbr1stDigit),                        backup);
  save(&shiftF,                             sizeof(shiftF),                             backup);
  save(&shiftG,                             sizeof(shiftG),                             backup);
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
  save(&shortIntegerMask,                   sizeof(shortIntegerMask),                   backup);
  save(&shortIntegerSignBit,                sizeof(shortIntegerSignBit),                backup);
  save(&temporaryInformation,               sizeof(temporaryInformation),               backup);
  save(&glyphNotFound,                      sizeof(glyphNotFound),                      backup);
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
  save(&savedSystemFlags,                   sizeof(savedSystemFlags),                   backup);
  save(&thereIsSomethingToUndo,             sizeof(thereIsSomethingToUndo),             backup);

  save(&eRPN,                               sizeof(eRPN),                               backup);    //JM vv
  save(&HOME3,                              sizeof(HOME3),                              backup);
  save(&ShiftTimoutMode,                    sizeof(ShiftTimoutMode),                    backup);
  save(&UNITDisplay,                        sizeof(UNITDisplay),                        backup);
  save(&SigFigMode,                         sizeof(SigFigMode),                         backup);
  save(&SH_BASE_HOME,                       sizeof(SH_BASE_HOME  ),                     backup);
  save(&SH_BASE_AHOME,                      sizeof(SH_BASE_AHOME ),                     backup);
  save(&Home3TimerMode,                     sizeof(Home3TimerMode),                     backup);
  save(&Norm_Key_00_VAR,                    sizeof(Norm_Key_00_VAR),                    backup);
  save(&Input_Default,                      sizeof(Input_Default),                      backup);
  save(&jm_FG_LINE,                         sizeof(jm_FG_LINE),                         backup);
  save(&jm_FG_DOTS,                         sizeof(jm_FG_DOTS),                         backup);
  save(&jm_G_DOUBLETAP,                     sizeof(jm_G_DOUBLETAP),                     backup);
  save(&jm_HOME_SUM,                        sizeof(jm_HOME_SUM),                        backup);
  save(&jm_HOME_MIR,                        sizeof(jm_HOME_MIR),                        backup);
  save(&jm_HOME_FIX,                        sizeof(jm_HOME_FIX),                        backup);
  save(&graph_xmin,                         sizeof(graph_xmin),                         backup);
  save(&graph_xmax,                         sizeof(graph_xmax),                         backup);
  save(&graph_ymin,                         sizeof(graph_ymin),                         backup);
  save(&graph_ymax,                         sizeof(graph_ymax),                         backup);
  save(&graph_dx  ,                         sizeof(graph_dx  ),                         backup);
  save(&graph_dy  ,                         sizeof(graph_dy  ),                         backup);
  save(&extentx   ,                         sizeof(extentx   ),                         backup);
  save(&extenty   ,                         sizeof(extenty   ),                         backup);
  save(&jm_VECT,                            sizeof(jm_VECT),                            backup);
  save(&jm_NVECT,                           sizeof(jm_NVECT),                           backup);
  save(&jm_SCALE,                           sizeof(jm_SCALE),                           backup);
  save(&Aspect_Square,                      sizeof(Aspect_Square),                      backup);
  save(&PLOT_LINE    ,                      sizeof(PLOT_LINE    ),                      backup);
  save(&PLOT_CROSS   ,                      sizeof(PLOT_CROSS   ),                      backup);
  save(&PLOT_BOX     ,                      sizeof(PLOT_BOX     ),                      backup);
  save(&PLOT_INTG    ,                      sizeof(PLOT_INTG    ),                      backup);
  save(&PLOT_DIFF    ,                      sizeof(PLOT_DIFF    ),                      backup);
  save(&PLOT_RMS     ,                      sizeof(PLOT_RMS     ),                      backup);
  save(&PLOT_SHADE     ,                    sizeof(PLOT_SHADE   ),                      backup);
  save(&jm_LARGELI,                         sizeof(jm_LARGELI),                         backup);
  save(&running_program_jm,                 sizeof(running_program_jm),                 backup);
  save(&indic_x,                            sizeof(indic_x),                            backup);
  save(&indic_y,                            sizeof(indic_y),                            backup);
  save(&fnXEQMENUpos,                       sizeof(fnXEQMENUpos),                       backup);
  save(&indexOfItemsXEQM,                   sizeof(indexOfItemsXEQM),                   backup);
  save(&T_cursorPos,                        sizeof(T_cursorPos),                        backup);   //JM ^^
  save(&SHOWregis,                          sizeof(SHOWregis),                          backup);   //JM ^^
  save(&mm_MNU_HOME,                        sizeof(mm_MNU_HOME),                        backup);   //JM ^^
  save(&mm_MNU_ALPHA,                       sizeof(mm_MNU_ALPHA),                       backup);   //JM ^^
  save(&MY_ALPHA_MENU,                      sizeof(MY_ALPHA_MENU),                      backup);   //JM ^^
  save(&displayStackSHOIDISP,               sizeof(displayStackSHOIDISP),               backup);   //JM ^^
  save(&ListXYposition,                     sizeof(ListXYposition),                     backup);   //JM ^^
  save(&numLock,                            sizeof(numLock),                            backup);   //JM ^^


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
    restore(freeMemoryRegions,                   sizeof(freeMemoryRegions),                  backup);
    restore(&numberOfFreeMemoryRegions,          sizeof(numberOfFreeMemoryRegions),          backup);
    restore(globalFlags,                         sizeof(globalFlags),                        backup);
    restore(tmpStr3000,                          sizeof(tmpStr3000),                         backup);
    restore(errorMessage,                        sizeof(errorMessage),                       backup);
    restore(aimBuffer,                           sizeof(aimBuffer),                          backup);
    restore(nimBufferDisplay,                    sizeof(nimBufferDisplay),                   backup);
    restore(tamBuffer,                           sizeof(tamBuffer),                          backup);
    restore(asmBuffer,                           sizeof(asmBuffer),                          backup);
    restore(oldTime,                             sizeof(oldTime),                            backup);
    restore(dateTimeString,                      sizeof(dateTimeString),                     backup);
    restore(softmenuStack,                       sizeof(softmenuStack),                      backup);
    restore(reg,                                 sizeof(reg),                                backup);
    restore(savedStackRegister,                  sizeof(savedStackRegister),                 backup);
    restore(kbd_usr,                             sizeof(kbd_usr),                            backup);
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
    restore(&ramPtr,                             sizeof(ramPtr),                             backup);
    savedStatisticalSumsPointer = TO_PCMEMPTR(ramPtr);
    restore(&programCounter,                     sizeof(programCounter),                     backup);
    restore(&xCursor,                            sizeof(xCursor),                            backup);
    restore(&yCursor,                            sizeof(yCursor),                            backup);
    restore(&firstGregorianDay,                  sizeof(firstGregorianDay),                  backup);
    restore(&denMax,                             sizeof(denMax),                             backup);
    restore(&softmenuStackPointer,               sizeof(softmenuStackPointer),               backup);
    restore(&softmenuStackPointerBeforeAIM,      sizeof(softmenuStackPointerBeforeAIM),      backup);
    restore(&transitionSystemState,              sizeof(transitionSystemState),              backup);
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
    restore(&rbr1stDigit,                        sizeof(rbr1stDigit),                        backup);
    restore(&shiftF,                             sizeof(shiftF),                             backup);
    restore(&shiftG,                             sizeof(shiftG),                             backup);
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
    restore(&shortIntegerMask,                   sizeof(shortIntegerMask),                   backup);
    restore(&shortIntegerSignBit,                sizeof(shortIntegerSignBit),                backup);
    restore(&temporaryInformation,               sizeof(temporaryInformation),               backup);

    restore(&glyphNotFound,                      sizeof(glyphNotFound),                      backup);
    glyphNotFound.data   = malloc(38);
    xcopy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8", 38);

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
    restore(&savedSystemFlags,                   sizeof(savedSystemFlags),                   backup);
    restore(&thereIsSomethingToUndo,             sizeof(thereIsSomethingToUndo),             backup);

    restore(&eRPN,                               sizeof(eRPN),                               backup);    //JM vv
    restore(&HOME3,                              sizeof(HOME3),                              backup);
    restore(&ShiftTimoutMode,                    sizeof(ShiftTimoutMode),                    backup);
    restore(&UNITDisplay,                        sizeof(UNITDisplay),                        backup);
    restore(&SigFigMode,                         sizeof(SigFigMode),                         backup);
    restore(&SH_BASE_HOME,                       sizeof(SH_BASE_HOME  ),                     backup);
    restore(&SH_BASE_AHOME,                      sizeof(SH_BASE_AHOME ),                     backup);
    restore(&Home3TimerMode,                     sizeof(Home3TimerMode),                     backup);
    restore(&Norm_Key_00_VAR,                    sizeof(Norm_Key_00_VAR),                    backup);
    restore(&Input_Default,                      sizeof(Input_Default),                      backup);
    restore(&jm_FG_LINE,                         sizeof(jm_FG_LINE),                         backup);
    restore(&jm_FG_DOTS,                         sizeof(jm_FG_DOTS),                         backup);
    restore(&jm_G_DOUBLETAP,                     sizeof(jm_G_DOUBLETAP),                     backup);
    restore(&jm_HOME_SUM,                        sizeof(jm_HOME_SUM),                        backup);
    restore(&jm_HOME_MIR,                        sizeof(jm_HOME_MIR),                        backup);
    restore(&jm_HOME_FIX,                        sizeof(jm_HOME_FIX),                        backup);
    restore(&graph_xmin,                         sizeof(graph_xmin),                         backup);
    restore(&graph_xmax,                         sizeof(graph_xmax),                         backup);
    restore(&graph_ymin,                         sizeof(graph_ymin),                         backup);
    restore(&graph_ymax,                         sizeof(graph_ymax),                         backup);
    restore(&graph_dx  ,                         sizeof(graph_dx  ),                         backup);
    restore(&graph_dy  ,                         sizeof(graph_dy  ),                         backup);
    restore(&extentx   ,                         sizeof(extentx   ),                         backup);
    restore(&extenty   ,                         sizeof(extenty   ),                         backup);
    restore(&jm_VECT,                            sizeof(jm_VECT),                            backup);
    restore(&jm_NVECT,                           sizeof(jm_NVECT),                           backup);
    restore(&jm_SCALE,                           sizeof(jm_SCALE),                           backup);
    restore(&Aspect_Square,                      sizeof(Aspect_Square),                      backup);
    restore(&PLOT_LINE    ,                      sizeof(PLOT_LINE    ),                      backup);
    restore(&PLOT_CROSS   ,                      sizeof(PLOT_CROSS   ),                      backup);
    restore(&PLOT_BOX     ,                      sizeof(PLOT_BOX     ),                      backup);
    restore(&PLOT_INTG    ,                      sizeof(PLOT_INTG    ),                      backup);
    restore(&PLOT_DIFF    ,                      sizeof(PLOT_DIFF    ),                      backup);
    restore(&PLOT_RMS     ,                      sizeof(PLOT_RMS     ),                      backup);
    restore(&PLOT_SHADE   ,                      sizeof(PLOT_SHADE   ),                      backup);
    restore(&jm_LARGELI,                         sizeof(jm_LARGELI),                         backup);
    restore(&running_program_jm,                 sizeof(running_program_jm),                 backup);
    restore(&indic_x,                            sizeof(indic_x),                            backup);
    restore(&indic_y,                            sizeof(indic_y),                            backup);
    restore(&fnXEQMENUpos,                       sizeof(fnXEQMENUpos),                       backup);
    restore(&indexOfItemsXEQM,                   sizeof(indexOfItemsXEQM),                   backup);
    restore(&T_cursorPos,                        sizeof(T_cursorPos),                        backup);   //JM ^^
    restore(&SHOWregis,                          sizeof(SHOWregis),                          backup);   //JM ^^
    restore(&mm_MNU_HOME,                        sizeof(mm_MNU_HOME),                        backup);   //JM ^^
    restore(&mm_MNU_ALPHA,                       sizeof(mm_MNU_ALPHA),                       backup);   //JM ^^
    restore(&MY_ALPHA_MENU,                      sizeof(MY_ALPHA_MENU),                      backup);   //JM ^^
    restore(&displayStackSHOIDISP,               sizeof(displayStackSHOIDISP),               backup);   //JM ^^
    restore(&ListXYposition,                     sizeof(ListXYposition),                     backup);   //JM ^^
    restore(&numLock,                            sizeof(numLock),                            backup);   //JM ^^


    fclose(backup);
    printf("End of calc's restoration\n");

    if(SH_BASE_AHOME) MY_ALPHA_MENU = mm_MNU_ALPHA; else MY_ALPHA_MENU = MY_ALPHA_MENU_CNST;              //JM 
    


    #if (DEBUG_REGISTER_L == 1)
      refreshRegisterLine(REGISTER_X); // to show L register
    #endif

    #if (SCREEN_800X480 == 1)
      if(calcMode == CM_NORMAL)                {}
      else if(calcMode == CM_AIM)              {cursorEnabled = true;}
      else if(calcMode == CM_TAM)              {}
      else if(calcMode == CM_NIM)              {cursorEnabled = true;}
      else if(calcMode == CM_ASM)              {}
      else if(calcMode == CM_ASM_OVER_TAM)     {calcMode = CM_ASM_OVER_TAM; clearSystemFlag(FLAG_ALPHA);}
      else if(calcMode == CM_ASM_OVER_AIM)     {calcMode = CM_ASM_OVER_AIM; clearSystemFlag(FLAG_ALPHA);}
      else if(calcMode == CM_REGISTER_BROWSER) {}
      else if(calcMode == CM_FLAG_BROWSER)     {}
      else if(calcMode == CM_FLAG_BROWSER_OLD) {}             //JM
      else if(calcMode == CM_FONT_BROWSER)     {}
      else if(calcMode == CM_LISTXY)           {}
      else if(calcMode == CM_GRAPH)            {}
      else {
        sprintf(errorMessage, "In function restoreCalc: %" PRIu8 " is an unexpected value for calcMode", calcMode);
        displayBugScreen(errorMessage);
      }
    #else // SCREEN_800X480 == 0
      if(calcMode == CM_NORMAL)                calcModeNormalGui();
      else if(calcMode == CM_AIM)             {calcModeAimGui(); cursorEnabled = true;}
      else if(calcMode == CM_TAM)              calcModeTamGui();
      else if(calcMode == CM_NIM)             {calcModeNormalGui(); cursorEnabled = true;}
      else if(calcMode == CM_ASM)              calcModeAsm();
      else if(calcMode == CM_ASM_OVER_TAM)    {calcModeAsm(); calcMode = CM_ASM_OVER_TAM; clearSystemFlag(FLAG_ALPHA);}
      else if(calcMode == CM_ASM_OVER_AIM)    {calcModeAsm(); calcMode = CM_ASM_OVER_AIM; clearSystemFlag(FLAG_ALPHA);}
      else if(calcMode == CM_REGISTER_BROWSER) calcModeNormalGui();
      else if(calcMode == CM_FLAG_BROWSER)     calcModeNormalGui();
      else if(calcMode == CM_FLAG_BROWSER_OLD) calcModeNormalGui();             //JM
      else if(calcMode == CM_FONT_BROWSER)     calcModeNormalGui();
      else if(calcMode == CM_LISTXY)           calcModeNormalGui();
      else if(calcMode == CM_GRAPH)            calcModeNormalGui();
      else {
        sprintf(errorMessage, "In function restoreCalc: %" PRIu8 " is an unexpected value for calcMode", calcMode);
        displayBugScreen(errorMessage);
      }
    #endif // SCREEN_800X480

    refreshScreen();
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
      strcpy(aimBuffer, "LonI");
      break;

    case dtString:
      stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)(tmpStr3000 + START_REGISTER_VALUE));
      strcpy(aimBuffer, "Stri");
      break;

    case dtShortInteger:
      convertShortIntegerRegisterToUInt64(regist, &sign, &value);
      sprintf(tmpStr3000 + START_REGISTER_VALUE, "%c%" PRIu64 " %" PRIu32, sign ? '-' : '+', value, getRegisterShortIntegerBase(regist));
      strcpy(aimBuffer, "ShoI");
      break;

    case dtReal34:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpStr3000 + START_REGISTER_VALUE);
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
      real34ToString(REGISTER_REAL34_DATA(regist), tmpStr3000 + START_REGISTER_VALUE);
      strcat(tmpStr3000 + START_REGISTER_VALUE, " ");
      real34ToString(REGISTER_IMAG34_DATA(regist), tmpStr3000 + START_REGISTER_VALUE + strlen(tmpStr3000 + START_REGISTER_VALUE));
      strcpy(aimBuffer, "Cplx");
      break;

    case dtConfig:
      for(str=tmpStr3000 + START_REGISTER_VALUE, cfg=(char *)REGISTER_CONFIG_DATA(regist), value=0; value<sizeof(dtConfigDescriptor_t); value++, cfg++, str+=2) {
        sprintf(str, "%02X", *cfg);
      }
      strcpy(aimBuffer, "Conf");
      break;

    default:
      strcpy(tmpStr3000 + START_REGISTER_VALUE, "???");
      strcpy(aimBuffer, "????");
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
  sprintf(tmpStr3000, "GLOBAL_REGISTERS\n%" PRIu16 "\n", FIRST_LOCAL_REGISTER);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
    registerToSaveString(regist);
    sprintf(tmpStr3000, "R%03" PRId16 "\n%s\n%s\n", regist, aimBuffer, tmpStr3000 + START_REGISTER_VALUE);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }

  // Global flags
  strcpy(tmpStr3000, "GLOBAL_FLAGS\n");
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "%" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 " %" PRIu16 "\n",
                       globalFlags[0],
                                   globalFlags[1],
                                               globalFlags[2],
                                                           globalFlags[3],
                                                                       globalFlags[4],
                                                                                   globalFlags[5],
                                                                                               globalFlags[6]);
  save(tmpStr3000, strlen(tmpStr3000), backup);

  // Local registers
  sprintf(tmpStr3000, "LOCAL_REGISTERS\n%" PRIu16 "\n", allLocalRegisterPointer->numberOfLocalRegisters);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(i=0; i<allLocalRegisterPointer->numberOfLocalRegisters; i++) {
    registerToSaveString(FIRST_LOCAL_REGISTER + i);
    sprintf(tmpStr3000, "R.%02" PRIu32 "\n%s\n%s\n", i, aimBuffer, tmpStr3000 + START_REGISTER_VALUE);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }

  // Local flags
  if(allLocalRegisterPointer->numberOfLocalRegisters) {
    sprintf(tmpStr3000, "LOCAL_FLAGS\n%" PRIu16 "\n", allLocalRegisterPointer->localFlags);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }

  // Named variables
  sprintf(tmpStr3000, "NAMED_VARIABLES\n%" PRIu16 "\n", allNamedVariablePointer->numberOfNamedVariables);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(i=0; i<allNamedVariablePointer->numberOfNamedVariables; i++) {
    registerToSaveString(FIRST_NAMED_VARIABLE + i);
    sprintf(tmpStr3000, "%s\n%s\n%s\n", "name", aimBuffer, tmpStr3000 + START_REGISTER_VALUE);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }

  // Statistical sums
  sprintf(tmpStr3000, "STATISTICAL_SUMS\n%" PRIu16 "\n", statisticalSumsPointer ? NUMBER_OF_STATISTICAL_SUMS : 0);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(i=0; i<(statisticalSumsPointer ? NUMBER_OF_STATISTICAL_SUMS : 0); i++) {
    realToString(statisticalSumsPointer + REAL_SIZE * i , tmpStr3000 + START_REGISTER_VALUE);
    sprintf(tmpStr3000, "%s\n", tmpStr3000 + START_REGISTER_VALUE);
    save(tmpStr3000, strlen(tmpStr3000), backup);
  }
  #pragma GCC diagnostic pop

  // System flags
  sprintf(tmpStr3000, "SYSTEM_FLAGS\n%" PRIu64 "\n", systemFlags);
  save(tmpStr3000, strlen(tmpStr3000), backup);

  // Keyboard assignments
  sprintf(tmpStr3000, "KEYBOARD_ASSIGNMENTS\n37\n");
  save(tmpStr3000, strlen(tmpStr3000), backup);
  for(i=0; i<37; i++) {
    sprintf(tmpStr3000, "%" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 "\n",
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
  sprintf(tmpStr3000, "denMax\n%" PRIu32 "\n", denMax);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "displayFormat\n%" PRIu8 "\n", displayFormat);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "displayFormatDigits\n%" PRIu8 "\n", displayFormatDigits);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "shortIntegerWordSize\n%" PRIu8 "\n", shortIntegerWordSize);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "shortIntegerMode\n%" PRIu8 "\n", shortIntegerMode);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "significantDigits\n%" PRIu8 "\n", significantDigits);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "currentAngularMode\n%" PRIu8 "\n", currentAngularMode);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "groupingGap\n%" PRIu8 "\n", groupingGap);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "roundingMode\n%" PRIu8 "\n", roundingMode);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "displayStack\n%" PRIu8 "\n", displayStack);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "rngState\n%" PRIu64 " %" PRIu64 "\n", pcg32_global.state, pcg32_global.inc);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "exponentLimit\n%" PRId16 "\n", exponentLimit);
  save(tmpStr3000, strlen(tmpStr3000), backup);
  sprintf(tmpStr3000, "displayStackSHOIDISP\n%" PRIu8 "\n", displayStackSHOIDISP);   //JM
  save(tmpStr3000, strlen(tmpStr3000), backup);

  #ifdef DMCP_BUILD
    f_close(backup);
    sys_disk_write_enable(0);
  #else
    fclose(backup);
  #endif

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

  readLine(stream, tmpStr3000);

  if(strcmp(tmpStr3000, "GLOBAL_REGISTERS") == 0) {
    readLine(stream, tmpStr3000); // Number of global registers
    numberOfRegs = stringToInt16(tmpStr3000);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpStr3000); // Register number
      regist = stringToInt16(tmpStr3000 + 1);
      readLine(stream, aimBuffer); // Register data type
      readLine(stream, tmpStr3000); // Register value

      if(loadMode == LM_ALL || (loadMode == LM_REGISTERS && regist < REGISTER_X)) {
        restoreRegister(regist, aimBuffer, tmpStr3000);
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
      readLine(stream, aimBuffer); // Register data type
      readLine(stream, tmpStr3000); // Register value

      if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
        restoreRegister(regist, aimBuffer, tmpStr3000);
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
      readLine(stream, aimBuffer); // Variable data type
      readLine(stream, tmpStr3000); // Variable value

      if(loadMode == LM_ALL || loadMode == LM_NAMED_VARIABLES) {
        printf("Variable %s ", tmpStr3000 + 2900);
        printf("%s = ", aimBuffer);
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
      readLine(stream, aimBuffer); // param
      readLine(stream, tmpStr3000); // value
      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        if(strcmp(aimBuffer, "firstGregorianDay") == 0) {
        }
        else if(strcmp(aimBuffer, "denMax") == 0) {
          denMax = stringToUint32(tmpStr3000);
          if(denMax < 1 || denMax > MAX_DENMAX) {
            denMax = MAX_DENMAX;
          }
        }
        else if(strcmp(aimBuffer, "displayFormat") == 0) {
          displayFormat = stringToUint8(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "displayFormatDigits") == 0) {
          displayFormatDigits = stringToUint8(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "shortIntegerWordSize") == 0) {
          shortIntegerWordSize = stringToUint8(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "shortIntegerMode") == 0) {
          shortIntegerMode = stringToUint8(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "significantDigits") == 0) {
          significantDigits = stringToUint8(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "currentAngularMode") == 0) {
          currentAngularMode = stringToUint8(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "groupingGap") == 0) {
          groupingGap = stringToUint8(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "roundingMode") == 0) {
          roundingMode = stringToUint8(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "displayStack") == 0) {
          displayStack = stringToUint8(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "rngState") == 0) {
          pcg32_global.state = stringToUint64(tmpStr3000);
          str = tmpStr3000;
          while(*str != ' ') str++;
          while(*str == ' ') str++;
          pcg32_global.inc = stringToUint64(str);
        }
        else if(strcmp(aimBuffer, "exponentLimit") == 0) {
          exponentLimit = stringToInt16(tmpStr3000);
        }
        else if(strcmp(aimBuffer, "displayStackSHOIDISP") == 0) {         //JM SHOIDISP
          displayStackSHOIDISP = stringToUint8(tmpStr3000);
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
        moreInfoOnError("In function fnLoad: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
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
    if(loadMode == LM_ALL) {
      temporaryInformation = TI_BACKUP_RESTORED;
    }
  #endif
}
