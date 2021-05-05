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

#include "saveRestoreCalcState.h"

#include "charString.h"
#include "config.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "gui.h"
#include "items.h"
#include "matrix.h"
#include "memory.h"
#include "plotstat.h"
#include "programming/manage.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "stats.h"
#include <string.h>

#include "wp43s.h"

#define BACKUP_VERSION         55  // Added x_min
#define START_REGISTER_VALUE 1522
#define BACKUP               ppgm_fp // The FIL *ppgm_fp pointer is provided by DMCP

static char *tmpRegisterString = NULL;

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
    save(globalRegister,                      sizeof(globalRegister),                     BACKUP);
    save(savedStackRegister,                  sizeof(savedStackRegister),                 BACKUP);
    save(kbd_usr,                             sizeof(kbd_usr),                            BACKUP);
    save(&tam.mode,                           sizeof(tam.mode),                           BACKUP);
    save(&tam.function,                       sizeof(tam.function),                       BACKUP);
    save(&tam.alpha,                          sizeof(tam.alpha),                          BACKUP);
    save(&tam.currentOperation,               sizeof(tam.currentOperation),               BACKUP);
    save(&tam.dot,                            sizeof(tam.dot),                            BACKUP);
    save(&tam.indirect,                       sizeof(tam.indirect),                       BACKUP);
    save(&tam.digitsSoFar,                    sizeof(tam.digitsSoFar),                    BACKUP);
    save(&tam.value,                          sizeof(tam.value),                          BACKUP);
    save(&tam.min,                            sizeof(tam.min),                            BACKUP);
    save(&tam.max,                            sizeof(tam.max),                            BACKUP);
    save(&rbrRegister,                        sizeof(rbrRegister),                        BACKUP);
    ramPtr = TO_WP43SMEMPTR(allNamedVariables);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43SMEMPTR(statisticalSumsPointer);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43SMEMPTR(savedStatisticalSumsPointer);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    save(&xCursor,                            sizeof(xCursor),                            BACKUP);
    save(&yCursor,                            sizeof(yCursor),                            BACKUP);
    save(&firstGregorianDay,                  sizeof(firstGregorianDay),                  BACKUP);
    save(&denMax,                             sizeof(denMax),                             BACKUP);
    save(&currentRegisterBrowserScreen,       sizeof(currentRegisterBrowserScreen),       BACKUP);
    save(&currentFntScr,                      sizeof(currentFntScr),                      BACKUP);
    save(&currentFlgScr,                      sizeof(currentFlgScr),                      BACKUP);
    save(&displayFormat,                      sizeof(displayFormat),                      BACKUP);
    save(&displayFormatDigits,                sizeof(displayFormatDigits),                BACKUP);
    save(&timeDisplayFormatDigits,            sizeof(timeDisplayFormatDigits),            BACKUP);
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
    save(&wp43sMemInBlocks,                   sizeof(wp43sMemInBlocks),                   BACKUP);
    save(&gmpMemInBytes,                      sizeof(gmpMemInBytes),                      BACKUP);
    save(&catalog,                            sizeof(catalog),                            BACKUP);
    save(&lastCatalogPosition,                sizeof(lastCatalogPosition),                BACKUP);
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
    save(&numberOfTamMenusToPop,              sizeof(numberOfTamMenusToPop),              BACKUP);
    save(&lrSelection,                        sizeof(lrSelection),                        BACKUP);
    save(&lrChosen,                           sizeof(lrChosen),                           BACKUP);
    save(&lastPlotMode,                       sizeof(lastPlotMode),                       BACKUP);
    save(&plotSelection,                      sizeof(plotSelection),                      BACKUP);

    save(&graph_dx,                           sizeof(graph_dx),                           BACKUP);
    save(&graph_dy,                           sizeof(graph_dy),                           BACKUP);
    save(&extentx,                            sizeof(extentx),                            BACKUP);
    save(&extenty,                            sizeof(extenty),                            BACKUP);
    save(&jm_VECT,                            sizeof(jm_VECT),                            BACKUP);
    save(&jm_NVECT,                           sizeof(jm_NVECT),                           BACKUP);
    save(&jm_SCALE,                           sizeof(jm_SCALE),                           BACKUP);
    save(&Aspect_Square,                      sizeof(Aspect_Square),                      BACKUP);
    save(&PLOT_LINE,                          sizeof(PLOT_LINE),                          BACKUP);
    save(&PLOT_CROSS,                         sizeof(PLOT_CROSS),                         BACKUP);
    save(&PLOT_BOX,                           sizeof(PLOT_BOX),                           BACKUP);
    save(&PLOT_INTG,                          sizeof(PLOT_INTG),                          BACKUP);
    save(&PLOT_DIFF,                          sizeof(PLOT_DIFF),                          BACKUP);
    save(&PLOT_RMS,                           sizeof(PLOT_RMS),                           BACKUP);
    save(&PLOT_SHADE,                         sizeof(PLOT_SHADE),                         BACKUP);
    save(&PLOT_AXIS,                          sizeof(PLOT_AXIS),                          BACKUP);
    save(&PLOT_ZMX,                           sizeof(PLOT_ZMX),                           BACKUP);
    save(&PLOT_ZMY,                           sizeof(PLOT_ZMY),                           BACKUP);
    save(&PLOT_ZOOM,                          sizeof(PLOT_ZOOM),                          BACKUP);
    save(&plotmode,                           sizeof(plotmode),                           BACKUP);
    save(&tick_int_x,                         sizeof(tick_int_x),                         BACKUP);
    save(&tick_int_y,                         sizeof(tick_int_y),                         BACKUP);
    save(&x_min,                              sizeof(x_min),                              BACKUP);
    save(&x_max,                              sizeof(x_max),                              BACKUP);
    save(&y_min,                              sizeof(y_min),                              BACKUP);
    save(&y_max,                              sizeof(y_max),                              BACKUP);
    save(&xzero,                              sizeof(xzero),                              BACKUP);
    save(&yzero,                              sizeof(yzero),                              BACKUP);
    save(gr_x,                                LIM*sizeof(graphtype),                      BACKUP);
    save(gr_y,                                LIM*sizeof(graphtype),                      BACKUP);
    save(&telltale,                           sizeof(telltale),                           BACKUP);
    save(&ix_count,                           sizeof(ix_count),                           BACKUP);
    save(&matrixIndex,                        sizeof(matrixIndex),                        BACKUP);


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
      restore(globalRegister,                      sizeof(globalRegister),                     BACKUP);
      restore(savedStackRegister,                  sizeof(savedStackRegister),                 BACKUP);
      restore(kbd_usr,                             sizeof(kbd_usr),                            BACKUP);
      restore(&tam.mode,                           sizeof(tam.mode),                           BACKUP);
      restore(&tam.function,                       sizeof(tam.function),                       BACKUP);
      restore(&tam.alpha,                          sizeof(tam.alpha),                          BACKUP);
      restore(&tam.currentOperation,               sizeof(tam.currentOperation),               BACKUP);
      restore(&tam.dot,                            sizeof(tam.dot),                            BACKUP);
      restore(&tam.indirect,                       sizeof(tam.indirect),                       BACKUP);
      restore(&tam.digitsSoFar,                    sizeof(tam.digitsSoFar),                    BACKUP);
      restore(&tam.value,                          sizeof(tam.value),                          BACKUP);
      restore(&tam.min,                            sizeof(tam.min),                            BACKUP);
      restore(&tam.max,                            sizeof(tam.max),                            BACKUP);
      restore(&rbrRegister,                        sizeof(rbrRegister),                        BACKUP);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      allNamedVariables = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      statisticalSumsPointer = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      savedStatisticalSumsPointer = TO_PCMEMPTR(ramPtr);
      restore(&xCursor,                            sizeof(xCursor),                            BACKUP);
      restore(&yCursor,                            sizeof(yCursor),                            BACKUP);
      restore(&firstGregorianDay,                  sizeof(firstGregorianDay),                  BACKUP);
      restore(&denMax,                             sizeof(denMax),                             BACKUP);
      restore(&currentRegisterBrowserScreen,       sizeof(currentRegisterBrowserScreen),       BACKUP);
      restore(&currentFntScr,                      sizeof(currentFntScr),                      BACKUP);
      restore(&currentFlgScr,                      sizeof(currentFlgScr),                      BACKUP);
      restore(&displayFormat,                      sizeof(displayFormat),                      BACKUP);
      restore(&displayFormatDigits,                sizeof(displayFormatDigits),                BACKUP);
      restore(&timeDisplayFormatDigits,            sizeof(timeDisplayFormatDigits),            BACKUP);
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
      restore(&wp43sMemInBlocks,                   sizeof(wp43sMemInBlocks),                   BACKUP);
      restore(&gmpMemInBytes,                      sizeof(gmpMemInBytes),                      BACKUP);
      restore(&catalog,                            sizeof(catalog),                            BACKUP);
      restore(&lastCatalogPosition,                sizeof(lastCatalogPosition),                BACKUP);
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
      restore(&numberOfTamMenusToPop,              sizeof(numberOfTamMenusToPop),              BACKUP);
      restore(&lrSelection,                        sizeof(lrSelection),                        BACKUP);
      restore(&lrChosen,                           sizeof(lrChosen),                           BACKUP);
      restore(&lastPlotMode,                       sizeof(lastPlotMode),                       BACKUP);
      restore(&plotSelection,                      sizeof(plotSelection),                      BACKUP);

      restore(&graph_dx,                           sizeof(graph_dx),                           BACKUP);
      restore(&graph_dy,                           sizeof(graph_dy),                           BACKUP);
      restore(&extentx,                            sizeof(extentx),                            BACKUP);
      restore(&extenty,                            sizeof(extenty),                            BACKUP);
      restore(&jm_VECT,                            sizeof(jm_VECT),                            BACKUP);
      restore(&jm_NVECT,                           sizeof(jm_NVECT),                           BACKUP);
      restore(&jm_SCALE,                           sizeof(jm_SCALE),                           BACKUP);
      restore(&Aspect_Square,                      sizeof(Aspect_Square),                      BACKUP);
      restore(&PLOT_LINE,                          sizeof(PLOT_LINE),                          BACKUP);
      restore(&PLOT_CROSS,                         sizeof(PLOT_CROSS),                         BACKUP);
      restore(&PLOT_BOX,                           sizeof(PLOT_BOX),                           BACKUP);
      restore(&PLOT_INTG,                          sizeof(PLOT_INTG),                          BACKUP);
      restore(&PLOT_DIFF,                          sizeof(PLOT_DIFF),                          BACKUP);
      restore(&PLOT_RMS,                           sizeof(PLOT_RMS),                           BACKUP);
      restore(&PLOT_SHADE,                         sizeof(PLOT_SHADE),                         BACKUP);
      restore(&PLOT_AXIS,                          sizeof(PLOT_AXIS),                          BACKUP);
      restore(&PLOT_ZMX,                           sizeof(PLOT_ZMX),                           BACKUP);
      restore(&PLOT_ZMY,                           sizeof(PLOT_ZMY),                           BACKUP);
      restore(&PLOT_ZOOM,                          sizeof(PLOT_ZOOM),                          BACKUP);
      restore(&plotmode,                           sizeof(plotmode),                           BACKUP);
      restore(&tick_int_x,                         sizeof(tick_int_x),                         BACKUP);
      restore(&tick_int_y,                         sizeof(tick_int_y),                         BACKUP);
      restore(&x_min,                              sizeof(x_min),                              BACKUP);
      restore(&x_max,                              sizeof(x_max),                              BACKUP);
      restore(&y_min,                              sizeof(y_min),                              BACKUP);
      restore(&y_max,                              sizeof(y_max),                              BACKUP);
      restore(&xzero,                              sizeof(xzero),                              BACKUP);
      restore(&yzero,                              sizeof(yzero),                              BACKUP);
      restore(gr_x,                                LIM*sizeof(graphtype),                      BACKUP);
      restore(gr_y,                                LIM*sizeof(graphtype),                      BACKUP);
      restore(&telltale,                           sizeof(telltale),                           BACKUP);
      restore(&ix_count,                           sizeof(ix_count),                           BACKUP);
      restore(&matrixIndex,                        sizeof(matrixIndex),                        BACKUP);

      fclose(BACKUP);
      printf("End of calc's restoration\n");

      scanLabelsAndPrograms();
      defineCurrentProgramFromGlobalStepNumber(currentLocalStepNumber + programList[currentProgramNumber - 1].step - 1);
      //defineCurrentLocalRegisters();

      #if (DEBUG_REGISTER_L == 1)
        refreshRegisterLine(REGISTER_X); // to show L register
      #endif // (DEBUG_REGISTER_L == 1)

      #if (SCREEN_800X480 == 1)
        if(calcMode == CM_NORMAL)                     {}
        else if(calcMode == CM_AIM)                   {cursorEnabled = true;}
        else if(calcMode == CM_NIM)                   {cursorEnabled = true;}
        else if(calcMode == CM_REGISTER_BROWSER)      {}
        else if(calcMode == CM_FLAG_BROWSER)          {}
        else if(calcMode == CM_FONT_BROWSER)          {}
        else if(calcMode == CM_PEM)                   {}
        else if(calcMode == CM_PLOT_STAT)             {}
        else if(calcMode == CM_MIM)                   {mimRestore();}
        else {
          sprintf(errorMessage, "In function restoreCalc: %" PRIu8 " is an unexpected value for calcMode", calcMode);
          displayBugScreen(errorMessage);
        }
      #else // (SCREEN_800X480 == 0)
        if(calcMode == CM_NORMAL)                      calcModeNormalGui();
        else if(calcMode == CM_AIM)                   {calcModeAimGui();    cursorEnabled = true;}
        else if(calcMode == CM_NIM)                   {calcModeNormalGui(); cursorEnabled = true;}
        else if(calcMode == CM_REGISTER_BROWSER)       calcModeNormalGui();
        else if(calcMode == CM_FLAG_BROWSER)           calcModeNormalGui();
        else if(calcMode == CM_FONT_BROWSER)           calcModeNormalGui();
        else if(calcMode == CM_PEM)                    calcModeNormalGui();
        else if(calcMode == CM_PLOT_STAT)              calcModeNormalGui();
        else if(calcMode == CM_MIM)                   {calcModeNormalGui(); mimRestore();}
        else {
          sprintf(errorMessage, "In function restoreCalc: %" PRIu8 " is an unexpected value for calcMode", calcMode);
          displayBugScreen(errorMessage);
        }
      #endif // (SCREEN_800X480 == 1)
        if(catalog) {
          clearSystemFlag(FLAG_ALPHA);
        }

      refreshScreen();
    }
  }
#endif // PC_BUILD


static void registerToSaveString(calcRegister_t regist) {
  longInteger_t lgInt;
  int16_t sign;
  uint64_t value;
  char *str, *cfg;

  tmpRegisterString = tmpString + START_REGISTER_VALUE;

  switch(getRegisterDataType(regist)) {
    case dtLongInteger:
      convertLongIntegerRegisterToLongInteger(regist, lgInt);
      longIntegerToAllocatedString(lgInt, tmpRegisterString, TMP_STR_LENGTH - START_REGISTER_VALUE - 1);
      longIntegerFree(lgInt);
      strcpy(aimBuffer, "LonI");
      break;

    case dtString:
      stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)(tmpRegisterString));
      strcpy(aimBuffer, "Stri");
      break;

    case dtShortInteger:
      convertShortIntegerRegisterToUInt64(regist, &sign, &value);
      sprintf(tmpRegisterString, "%c%" PRIu64 " %" PRIu32, sign ? '-' : '+', value, getRegisterShortIntegerBase(regist));
      strcpy(aimBuffer, "ShoI");
      break;

    case dtReal34:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpRegisterString);
      switch(getRegisterAngularMode(regist)) {
        case amDegree:
          strcpy(aimBuffer, "Real:DEG");
          break;

        case amDMS:
          strcpy(aimBuffer, "Real:DMS");
          break;

        case amRadian:
          strcpy(aimBuffer, "Real:RAD");
          break;

        case amMultPi:
          strcpy(aimBuffer, "Real:MULTPI");
          break;

        case amGrad:
          strcpy(aimBuffer, "Real:GRAD");
          break;

        case amNone:
          strcpy(aimBuffer, "Real");
          break;

        default:
          strcpy(aimBuffer, "Real:???");
          break;
      }
      break;

    case dtComplex34:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpRegisterString);
      strcat(tmpRegisterString, " ");
      real34ToString(REGISTER_IMAG34_DATA(regist), tmpRegisterString + strlen(tmpRegisterString));
      strcpy(aimBuffer, "Cplx");
      break;

    case dtTime:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpRegisterString);
      strcpy(aimBuffer, "Time");
      break;

    case dtDate:
      real34ToString(REGISTER_REAL34_DATA(regist), tmpRegisterString);
      strcpy(aimBuffer, "Date");
      break;

#ifndef TESTSUITE_BUILD
    case dtReal34Matrix:
      sprintf(tmpRegisterString, "%" PRIu16 " %" PRIu16, REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows, REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns);
      strcpy(aimBuffer, "Rema");
      break;
#endif // TESTSUITE_BUILD

    case dtConfig:
      for(str=tmpRegisterString, cfg=(char *)REGISTER_CONFIG_DATA(regist), value=0; value<sizeof(dtConfigDescriptor_t); value++, cfg++, str+=2) {
        sprintf(str, "%02X", *cfg);
      }
      strcpy(aimBuffer, "Conf");
      break;

    default:
      strcpy(tmpRegisterString, "???");
      strcpy(aimBuffer, "????");
  }
}


static void saveMatrixElements(calcRegister_t regist, void *stream) {
#ifndef TESTSUITE_BUILD
  if(getRegisterDataType(regist) == dtReal34Matrix) {
    for(uint32_t element = 0; element < REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows * REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns; ++element) {
      real34ToString(REGISTER_REAL34_MATRIX_M_ELEMENTS(regist) + element, tmpString);
      strcat(tmpString, "\n");
      save(tmpString, strlen(tmpString), stream);
    }
  }
#endif // TESTSUITE_BUILD
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

  // Global registers
  sprintf(tmpString, "GLOBAL_REGISTERS\n%" PRIu16 "\n", (uint16_t)(FIRST_LOCAL_REGISTER));
  save(tmpString, strlen(tmpString), BACKUP);
  for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
    registerToSaveString(regist);
    sprintf(tmpString, "R%03" PRId16 "\n%s\n%s\n", regist, aimBuffer, tmpRegisterString);
    save(tmpString, strlen(tmpString), BACKUP);
    saveMatrixElements(regist, BACKUP);
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
  sprintf(tmpString, "LOCAL_REGISTERS\n%" PRIu8 "\n", currentNumberOfLocalRegisters);
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<currentNumberOfLocalRegisters; i++) {
    registerToSaveString(FIRST_LOCAL_REGISTER + i);
    sprintf(tmpString, "R.%02" PRIu32 "\n%s\n%s\n", i, aimBuffer, tmpRegisterString);
    save(tmpString, strlen(tmpString), BACKUP);
    saveMatrixElements(FIRST_LOCAL_REGISTER + i, BACKUP);
  }

  // Local flags
  if(currentLocalRegisters) {
    sprintf(tmpString, "LOCAL_FLAGS\n%" PRIu32 "\n", currentLocalFlags->localFlags);
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // Named variables
  sprintf(tmpString, "NAMED_VARIABLES\n%" PRIu16 "\n", numberOfNamedVariables);
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<numberOfNamedVariables; i++) {
    registerToSaveString(FIRST_NAMED_VARIABLE + i);
    sprintf(tmpString, "%s\n%s\n%s\n", "name", aimBuffer, tmpRegisterString);
    save(tmpString, strlen(tmpString), BACKUP);
    saveMatrixElements(FIRST_NAMED_VARIABLE + i, BACKUP);
  }

  // Statistical sums
  sprintf(tmpString, "STATISTICAL_SUMS\n%" PRIu16 "\n", (uint16_t)(statisticalSumsPointer ? NUMBER_OF_STATISTICAL_SUMS : 0));
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<(statisticalSumsPointer ? NUMBER_OF_STATISTICAL_SUMS : 0); i++) {
    realToString(statisticalSumsPointer + REAL_SIZE * i , tmpRegisterString);
    sprintf(tmpString, "%s\n", tmpRegisterString);
    save(tmpString, strlen(tmpString), BACKUP);
  }

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
  sprintf(tmpString, "firstGregorianDay\n%" PRIu32 "\n", firstGregorianDay);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "denMax\n%" PRIu32 "\n", denMax);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "displayFormat\n%" PRIu8 "\n", displayFormat);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "displayFormatDigits\n%" PRIu8 "\n", displayFormatDigits);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "timeDisplayFormatDigits\n%" PRIu8 "\n", timeDisplayFormatDigits);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "shortIntegerWordSize\n%" PRIu8 "\n", shortIntegerWordSize);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "shortIntegerMode\n%" PRIu8 "\n", shortIntegerMode);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "significantDigits\n%" PRIu8 "\n", significantDigits);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "currentAngularMode\n%" PRIu8 "\n", (uint8_t)currentAngularMode);
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


  // Graph memory                                  //vv GRAPH MEMORY RESTORE
  sprintf(tmpString, "STAT_GRAPH_DATA\n%u\n",LIM*2+2);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "%u\n",ix_count);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "%E\n",telltale);
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<LIM; i++) {
    sprintf(tmpString, "%E\n",gr_x[i]);
    save(tmpString, strlen(tmpString), BACKUP);
    sprintf(tmpString, "%E\n",gr_y[i]);
    save(tmpString, strlen(tmpString), BACKUP);
  }
  // Graph memory                                  //^^ GRAPH MEMORY RESTORE



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
  uint32_t tag = amNone;

  if(type[4] == ':') {
         if(type[5] == 'R')                   tag = amRadian;
    else if(type[5] == 'M')                   tag = amMultPi;
    else if(type[5] == 'G')                   tag = amGrad;
    else if(type[5] == 'D' && type[6] == 'E') tag = amDegree;
    else if(type[5] == 'D' && type[6] == 'M') tag = amDMS;
    else                                      tag = amNone;

    reallocateRegister(regist, dtReal34, REAL34_SIZE, tag);
    stringToReal34(value, REGISTER_REAL34_DATA(regist));
  }

  else if(strcmp(type, "Real") == 0) {
    reallocateRegister(regist, dtReal34, REAL34_SIZE, tag);
    stringToReal34(value, REGISTER_REAL34_DATA(regist));
  }

  else if(strcmp(type, "Time") == 0) {
    reallocateRegister(regist, dtTime, REAL34_SIZE, amNone);
    stringToReal34(value, REGISTER_REAL34_DATA(regist));
  }

  else if(strcmp(type, "Date") == 0) {
    reallocateRegister(regist, dtDate, REAL34_SIZE, amNone);
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
    reallocateRegister(regist, dtString, TO_BLOCKS(len), amNone);
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

    reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, amNone);
    imaginaryPart = value;
    while(*imaginaryPart != ' ') imaginaryPart++;
    *(imaginaryPart++) = 0;
    stringToReal34(value, REGISTER_REAL34_DATA(regist));
    stringToReal34(imaginaryPart, REGISTER_IMAG34_DATA(regist));
  }

#ifndef TESTSUITE_BUILD
  else if(strcmp(type, "Rema") == 0) {
    char *numOfCols;
    uint16_t rows, cols;

    numOfCols = value;
    while(*numOfCols != ' ') numOfCols++;
    *(numOfCols++) = 0;
    rows = stringToUint16(value);
    cols = stringToUint16(numOfCols);
    reallocateRegister(regist, dtReal34Matrix, REAL34_SIZE * rows * cols, amNone);
    REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows = rows;
    REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns = cols;
  }
#endif // TESTSUITE_BUILD

  else if(strcmp(type, "Conf") == 0) {
    char *cfg;

    reallocateRegister(regist, dtConfig, CONFIG_SIZE, amNone);
    for(cfg=(char *)REGISTER_CONFIG_DATA(regist), tag=0; tag<sizeof(dtConfigDescriptor_t); tag++, value+=2, cfg++) {
      *cfg = ((*value >= 'A' ? *value - 'A' + 10 : *value - '0') << 8) | (*(value + 1) >= 'A' ? *(value + 1) - 'A' + 10 : *(value + 1) - '0');
    }
  }

  else {
    sprintf(errorMessage, "In function restoreRegister: Date type %s is to be coded!", type);
    displayBugScreen(errorMessage);
  }
}


static void restoreMatrixData(calcRegister_t regist, void *stream) {
#ifndef TESTSUITE_BUILD
  uint16_t rows, cols;
  uint32_t i;

  if(getRegisterDataType(regist) == dtReal34Matrix) {
    rows = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows;
    cols = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns;

    for(i = 0; i < rows * cols; ++i) {
      readLine(stream, tmpString);
      stringToReal34(tmpString, REGISTER_REAL34_MATRIX_M_ELEMENTS(regist) + i);
    }
  }
#endif // TESTSUITE_BUILD
}


static void skipMatrixData(char *type, char *value, void *stream) {
#ifndef TESTSUITE_BUILD
  uint16_t rows, cols;
  uint32_t i;
  char *numOfCols;

  if(strcmp(type, "Rema") == 0) {
    numOfCols = value;
    while(*numOfCols != ' ') numOfCols++;
    *(numOfCols++) = 0;
    rows = stringToUint16(value);
    cols = stringToUint16(numOfCols);

    for(i = 0; i < rows * cols; ++i) {
      readLine(stream, tmpString);
    }
  }
#endif // TESTSUITE_BUILD
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
        restoreMatrixData(regist, stream);
      }
      else {
        skipMatrixData(aimBuffer, tmpString, stream);
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
        restoreMatrixData(regist, stream);
      }
      else {
        skipMatrixData(aimBuffer, tmpString, stream);
      }
    }

    if(numberOfRegs > 0) {
      readLine(stream, tmpString); // LOCAL_FLAGS
      readLine(stream, tmpString); // LOCAL_FLAGS
      if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
        currentLocalFlags->localFlags = stringToUint32(tmpString);
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
      skipMatrixData(aimBuffer, tmpString, stream);
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
          firstGregorianDay = stringToUint32(tmpString);
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
        else if(strcmp(aimBuffer, "timeDisplayFormatDigits") == 0) {
          timeDisplayFormatDigits = stringToUint8(tmpString);
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

  // Graph memory                                  //vv GRAPH MEMORY RESTORE
  else if(strcmp(tmpString, "STAT_GRAPH_DATA") == 0) {
    char* end;
    readLine(stream, tmpString); // Number of params

    readLine(stream, tmpString); // ix_count
    ix_count = stringToInt16(tmpString);
    readLine(stream, tmpString); // telltale
    telltale = strtod(tmpString, &end);
    graph_setupmemory();
    for(i=0; i<LIM; i++) {
      readLine(stream, tmpString);
      gr_x[i] = strtod(tmpString, &end);
      readLine(stream, tmpString);
      gr_y[i] = strtod(tmpString, &end);
      //printf("^^^^### %u %f %f \n",i,gr_x[i],gr_y[i]);
    }
  }
  // Graph memory                                  //^^ GRAPH MEMORY RESTORE

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
  restoreOneSection(BACKUP, loadMode); // Graph memory

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
