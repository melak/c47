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

#include "assign.h"
#include "charString.h"
#include "config.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "hal/gui.h"
#include "items.h"
#include "c43Extensions/xeqm.h"
#include "c43Extensions/jm.h"
#include "mathematics/matrix.h"
#include "memory.h"
#include "plotstat.h"
#include "programming/flash.h"
#include "programming/lblGtoXeq.h"
#include "programming/manage.h"
#include "programming/nextStep.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "softmenus.h"
#include "solver/equation.h"
#include "solver/graph.h"
#include "sort.h"
#include "stats.h"
#include <string.h>
#if defined(PC_BUILD)
#include <stdio.h>
#include <errno.h>
#endif

#include "wp43.h"

#define BACKUP_VERSION       778  // file version number
#define configFileVersion    10000002 // arbitrary starting point version 10 000 001
#define START_REGISTER_VALUE 1000  // was 1522, why?
#define BACKUP               ppgm_fp // The FIL *ppgm_fp pointer is provided by DMCP

static uint32_t loadedVersion = 0;
static char *tmpRegisterString = NULL;

static void save(const void *buffer, uint32_t size, void *stream) {
  #if defined(DMCP_BUILD)
    UINT bytesWritten;
    f_write(stream, buffer, size, &bytesWritten);
  #else // !DMCP_BUILD
    fwrite(buffer, 1, size, stream);
  #endif // DMCP_BUILD
}



static uint32_t restore(void *buffer, uint32_t size, void *stream) {
  #if defined(DMCP_BUILD)
    UINT bytesRead;
    f_read(stream, buffer, size, &bytesRead);
    return(bytesRead);
  #else // !DMCP_BUILD
    return(fread(buffer, 1, size, stream));
  #endif // DMCP_BUILD
}



#if defined(PC_BUILD)
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
    save(errorMessage,                        ERROR_MESSAGE_LENGTH,                       BACKUP);
    save(aimBuffer,                           AIM_BUFFER_LENGTH,                          BACKUP);
    save(nimBufferDisplay,                    NIM_BUFFER_LENGTH,                          BACKUP);
    save(tamBuffer,                           TAM_BUFFER_LENGTH,                          BACKUP);
    save(asmBuffer,                           sizeof(asmBuffer),                          BACKUP);
    save(oldTime,                             sizeof(oldTime),                            BACKUP);
    save(dateTimeString,                      sizeof(dateTimeString),                     BACKUP);
    save(softmenuStack,                       sizeof(softmenuStack),                      BACKUP);
    save(globalRegister,                      sizeof(globalRegister),                     BACKUP);
    save(savedStackRegister,                  sizeof(savedStackRegister),                 BACKUP);
    save(kbd_usr,                             sizeof(kbd_usr),                            BACKUP);
    save(userMenuItems,                       sizeof(userMenuItems),                      BACKUP);
    save(userAlphaItems,                      sizeof(userAlphaItems),                     BACKUP);
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
    save(&numberOfNamedVariables,             sizeof(numberOfNamedVariables),             BACKUP);
    ramPtr = TO_WP43MEMPTR(allNamedVariables);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(allFormulae);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(userMenus);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(userKeyLabel);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(statisticalSumsPointer);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(savedStatisticalSumsPointer);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(labelList);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(flashLabelList);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(programList);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(flashProgramList);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
    ramPtr = TO_WP43MEMPTR(currentSubroutineLevelData);
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
    save(&programRunStop,                     sizeof(programRunStop),                     BACKUP);
    save(&entryStatus,                        sizeof(entryStatus),                        BACKUP);
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
    save(&wp43MemInBlocks,                    sizeof(wp43MemInBlocks),                    BACKUP);
    save(&gmpMemInBytes,                      sizeof(gmpMemInBytes),                      BACKUP);
    save(&catalog,                            sizeof(catalog),                            BACKUP);
    save(&lastCatalogPosition,                sizeof(lastCatalogPosition),                BACKUP);
    save(&lgCatalogSelection,                 sizeof(lgCatalogSelection),                 BACKUP);
    save(displayValueX,                       sizeof(displayValueX),                      BACKUP);
    save(&pcg32_global,                       sizeof(pcg32_global),                       BACKUP);
    save(&exponentLimit,                      sizeof(exponentLimit),                      BACKUP);
    save(&exponentHideLimit,                  sizeof(exponentHideLimit),                  BACKUP);
    save(&keyActionProcessed,                 sizeof(keyActionProcessed),                 BACKUP);
    save(&systemFlags,                        sizeof(systemFlags),                        BACKUP);
    save(&savedSystemFlags,                   sizeof(savedSystemFlags),                   BACKUP);
    save(&thereIsSomethingToUndo,             sizeof(thereIsSomethingToUndo),             BACKUP);
    ramPtr = TO_WP43MEMPTR(beginOfProgramMemory);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // beginOfProgramMemory pointer to block
    ramPtr = (uint32_t)((void *)beginOfProgramMemory -        TO_PCMEMPTR(TO_WP43MEMPTR(beginOfProgramMemory)));
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // beginOfProgramMemory offset within block
    ramPtr = TO_WP43MEMPTR(firstFreeProgramByte);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstFreeProgramByte pointer to block
    ramPtr = (uint32_t)((void *)firstFreeProgramByte - TO_PCMEMPTR(TO_WP43MEMPTR(firstFreeProgramByte)));
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstFreeProgramByte offset within block
    ramPtr = TO_WP43MEMPTR(firstDisplayedStep.ram);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstDisplayedStep pointer to block
    ramPtr = (uint32_t)((void *)firstDisplayedStep.ram - TO_PCMEMPTR(TO_WP43MEMPTR(firstDisplayedStep.ram)));
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstDisplayedStep offset within block
    ramPtr = TO_WP43MEMPTR(currentStep.ram);
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // currentStep pointer to block
    ramPtr = (uint32_t)((void *)currentStep.ram - TO_PCMEMPTR(TO_WP43MEMPTR(currentStep.ram)));
    save(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // currentStep offset within block
    save(&freeProgramBytes,                   sizeof(freeProgramBytes),                   BACKUP);
    save(&firstDisplayedLocalStepNumber,      sizeof(firstDisplayedLocalStepNumber),      BACKUP);
    save(&numberOfLabels,                     sizeof(numberOfLabels),                     BACKUP);
    save(&numberOfLabelsInFlash,              sizeof(numberOfLabelsInFlash),              BACKUP);
    save(&numberOfPrograms,                   sizeof(numberOfPrograms),                   BACKUP);
    save(&numberOfProgramsInFlash,            sizeof(numberOfProgramsInFlash),            BACKUP);
    save(&currentLocalStepNumber,             sizeof(currentLocalStepNumber),             BACKUP);
    save(&currentProgramNumber,               sizeof(currentProgramNumber),               BACKUP);
    save(&lastProgramListEnd,                 sizeof(lastProgramListEnd),                 BACKUP);
    save(&programListEnd,                     sizeof(programListEnd),                     BACKUP);
    save(&allSubroutineLevels,                sizeof(allSubroutineLevels),                BACKUP);
    save(&pemCursorIsZerothStep,              sizeof(pemCursorIsZerothStep),              BACKUP);
    save(&numberOfTamMenusToPop,              sizeof(numberOfTamMenusToPop),              BACKUP);
    save(&lrSelection,                        sizeof(lrSelection),                        BACKUP);
    save(&lrSelectionUndo,                    sizeof(lrSelectionUndo),                    BACKUP);
    save(&lrChosen,                           sizeof(lrChosen),                           BACKUP);
    save(&lrChosenUndo,                       sizeof(lrChosenUndo),                       BACKUP);
    save(&lastPlotMode,                       sizeof(lastPlotMode),                       BACKUP);
    save(&plotSelection,                      sizeof(plotSelection),                      BACKUP);

    save(&graph_dx,                           sizeof(graph_dx),                           BACKUP);
    save(&graph_dy,                           sizeof(graph_dy),                           BACKUP);
    save(&roundedTicks,                       sizeof(roundedTicks),                       BACKUP);
    save(&extentx,                            sizeof(extentx),                            BACKUP);
    save(&extenty,                            sizeof(extenty),                            BACKUP);
    save(&PLOT_VECT,                          sizeof(PLOT_VECT),                          BACKUP);
    save(&PLOT_NVECT,                         sizeof(PLOT_NVECT),                         BACKUP);
    save(&PLOT_SCALE,                         sizeof(PLOT_SCALE),                         BACKUP);
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
    save(&matrixIndex,                        sizeof(matrixIndex),                        BACKUP);
    save(&currentViewRegister,                sizeof(currentViewRegister),                BACKUP);
    save(&currentSolverStatus,                sizeof(currentSolverStatus),                BACKUP);
    save(&currentSolverProgram,               sizeof(currentSolverProgram),               BACKUP);
    save(&currentSolverVariable,              sizeof(currentSolverVariable),              BACKUP);
    save(&numberOfFormulae,                   sizeof(numberOfFormulae),                   BACKUP);
    save(&currentFormula,                     sizeof(currentFormula),                     BACKUP);
    save(&numberOfUserMenus,                  sizeof(numberOfUserMenus),                  BACKUP);
    save(&currentUserMenu,                    sizeof(currentUserMenu),                    BACKUP);
    save(&userKeyLabelSize,                   sizeof(userKeyLabelSize),                   BACKUP);
    save(&timerCraAndDeciseconds,             sizeof(timerCraAndDeciseconds),             BACKUP);
    save(&timerValue,                         sizeof(timerValue),                         BACKUP);
    save(&timerTotalTime,                     sizeof(timerTotalTime),                     BACKUP);
    save(&currentInputVariable,               sizeof(currentInputVariable),               BACKUP);
    save(&SAVED_SIGMA_LASTX,                  sizeof(SAVED_SIGMA_LASTX),                  BACKUP);
    save(&SAVED_SIGMA_LASTY,                  sizeof(SAVED_SIGMA_LASTY),                  BACKUP);
    save(&SAVED_SIGMA_LAct,                   sizeof(SAVED_SIGMA_LAct),                   BACKUP);
    save(&currentMvarLabel,                   sizeof(currentMvarLabel),                   BACKUP);
    save(&graphVariable,                      sizeof(graphVariable),                      BACKUP);
    save(&plotStatMx,                         sizeof(plotStatMx),                         BACKUP);
    save(&drawHistogram,                      sizeof(drawHistogram),                      BACKUP);
    save(&statMx,                             sizeof(statMx),                             BACKUP);
    save(&lrSelectionHistobackup,             sizeof(lrSelectionHistobackup),             BACKUP);
    save(&lrChosenHistobackup,                sizeof(lrChosenHistobackup),                BACKUP);
    save(&loBinR,                             sizeof(loBinR),                             BACKUP);
    save(&nBins ,                             sizeof(nBins ),                             BACKUP);
    save(&hiBinR,                             sizeof(hiBinR),                             BACKUP);
    save(&histElementXorY,                    sizeof(histElementXorY),                    BACKUP);

    save(&screenUpdatingMode,                 sizeof(screenUpdatingMode),                 BACKUP);
    for(int y = 0; y < SCREEN_HEIGHT; ++y) {
      uint8_t bmpdata = 0;
      for(int x = 0; x < SCREEN_WIDTH; ++x) {
        bmpdata <<= 1;
        if(*(screenData + y*screenStride + x) == ON_PIXEL) {
          bmpdata |= 1;
        }
        if((x % 8) == 7) {
          save(&bmpdata,                      sizeof(bmpdata),                            BACKUP);
          bmpdata = 0;
        }
      }
    }

    save(&eRPN,                               sizeof(eRPN),                               BACKUP);    //JM vv
    save(&HOME3,                              sizeof(HOME3),                              BACKUP);
    save(&ShiftTimoutMode,                    sizeof(ShiftTimoutMode),                    BACKUP);
    save(&UNITDisplay,                        sizeof(UNITDisplay),                        BACKUP);
    save(&SigFigMode,                         sizeof(SigFigMode),                         BACKUP);
    save(&SH_BASE_HOME,                       sizeof(SH_BASE_HOME  ),                     BACKUP);
    save(&Norm_Key_00_VAR,                    sizeof(Norm_Key_00_VAR),                    BACKUP);
    save(&Input_Default,                      sizeof(Input_Default),                      BACKUP);
    save(&jm_FG_LINE,                         sizeof(jm_FG_LINE),                         BACKUP);
    save(&jm_BASE_SCREEN,                     sizeof(jm_BASE_SCREEN),                     BACKUP);
    save(&jm_G_DOUBLETAP,                     sizeof(jm_G_DOUBLETAP),                     BACKUP);
    save(&jm_HOME_SUM,                        sizeof(jm_HOME_SUM),                        BACKUP);
    save(&graph_xmin,                         sizeof(graph_xmin),                         BACKUP);
    save(&graph_xmax,                         sizeof(graph_xmax),                         BACKUP);
    save(&graph_ymin,                         sizeof(graph_ymin),                         BACKUP);
    save(&graph_ymax,                         sizeof(graph_ymax),                         BACKUP);
    save(&jm_LARGELI,                         sizeof(jm_LARGELI),                         BACKUP);
    save(&constantFractions,                  sizeof(constantFractions),                  BACKUP);
    save(&constantFractionsMode,              sizeof(constantFractionsMode),              BACKUP);
    save(&constantFractionsOn,                sizeof(constantFractionsOn),                BACKUP);
    save(&running_program_jm,                 sizeof(running_program_jm),                 BACKUP);
    save(&indic_x,                            sizeof(indic_x),                            BACKUP);
    save(&indic_y,                            sizeof(indic_y),                            BACKUP);
    save(&fnXEQMENUpos,                       sizeof(fnXEQMENUpos),                       BACKUP);
    save(&indexOfItemsXEQM,                   sizeof(indexOfItemsXEQM),                   BACKUP);
    save(&T_cursorPos,                        sizeof(T_cursorPos),                        BACKUP);   //JM ^^
    save(&SHOWregis,                          sizeof(SHOWregis),                          BACKUP);   //JM ^^
    save(&mm_MNU_HOME,                        sizeof(mm_MNU_HOME),                        BACKUP);   //JM ^^
    save(&mm_MNU_ALPHA,                       sizeof(mm_MNU_ALPHA),                       BACKUP);   //JM ^^
    save(&displayStackSHOIDISP,               sizeof(displayStackSHOIDISP),               BACKUP);   //JM ^^
    save(&ListXYposition,                     sizeof(ListXYposition),                     BACKUP);   //JM ^^
    save(&numLock,                            sizeof(numLock),                            BACKUP);   //JM ^^
    save(&DRG_Cycling,                        sizeof(DRG_Cycling),                        BACKUP);   //JM
    save(&lastFlgScr,                         sizeof(lastFlgScr),                         BACKUP);   //C43 JM
    save(&displayAIMbufferoffset,             sizeof(displayAIMbufferoffset),             BACKUP);   //C43 JM
    save(&bcdDisplay,                         sizeof(bcdDisplay),                         BACKUP);   //C43 JM
    save(&topHex,                             sizeof(topHex),                             BACKUP);   //C43 JM
    save(&bcdDisplaySign,                     sizeof(bcdDisplaySign),                     BACKUP);   //C43 JM
    save(&DM_Cycling,                         sizeof(DM_Cycling),                         BACKUP);   //JM
    save(&SI_All,                             sizeof(SI_All),                             BACKUP);   //JM

    fclose(BACKUP);
    printf("End of calc's backup\n");
  }



  void restoreCalc(void) {
    uint32_t backupVersion, ramSize, ramPtr;
    FILE *ppgm_fp;
    uint8_t *loadedScreen = malloc(SCREEN_WIDTH * SCREEN_HEIGHT / 8);

    fnReset(CONFIRMED);
    BACKUP = fopen("backup.bin", "rb");
    if(BACKUP == NULL) {
      printf("Cannot restore calc's memory from file backup.bin! Performing RESET\n");
      refreshScreen();
      return;
    }

    restore(&backupVersion,                      sizeof(backupVersion),                      BACKUP);
    restore(&ramSize,                            sizeof(ramSize),                            BACKUP);
    if(backupVersion != BACKUP_VERSION || ramSize != RAM_SIZE) {
      fclose(BACKUP);
      refreshScreen();

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
      restore(errorMessage,                        ERROR_MESSAGE_LENGTH,                       BACKUP);
      restore(aimBuffer,                           AIM_BUFFER_LENGTH,                          BACKUP);
      restore(nimBufferDisplay,                    NIM_BUFFER_LENGTH,                          BACKUP);
      restore(tamBuffer,                           TAM_BUFFER_LENGTH,                          BACKUP);
      restore(asmBuffer,                           sizeof(asmBuffer),                          BACKUP);
      restore(oldTime,                             sizeof(oldTime),                            BACKUP);
      restore(dateTimeString,                      sizeof(dateTimeString),                     BACKUP);
      restore(softmenuStack,                       sizeof(softmenuStack),                      BACKUP);
      restore(globalRegister,                      sizeof(globalRegister),                     BACKUP);
      restore(savedStackRegister,                  sizeof(savedStackRegister),                 BACKUP);
      restore(kbd_usr,                             sizeof(kbd_usr),                            BACKUP);
      restore(userMenuItems,                       sizeof(userMenuItems),                      BACKUP);
      restore(userAlphaItems,                      sizeof(userAlphaItems),                     BACKUP);
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
      restore(&numberOfNamedVariables,             sizeof(numberOfNamedVariables),             BACKUP);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      allNamedVariables = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      allFormulae = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      userMenus = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      userKeyLabel = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      statisticalSumsPointer = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      savedStatisticalSumsPointer = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      labelList = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      flashLabelList = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      programList = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      flashProgramList = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP);
      currentSubroutineLevelData = TO_PCMEMPTR(ramPtr);
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
      restore(&programRunStop,                     sizeof(programRunStop),                     BACKUP);
      restore(&entryStatus,                        sizeof(entryStatus),                        BACKUP);
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
      restore(&wp43MemInBlocks,                    sizeof(wp43MemInBlocks),                    BACKUP);
      restore(&gmpMemInBytes,                      sizeof(gmpMemInBytes),                      BACKUP);
      restore(&catalog,                            sizeof(catalog),                            BACKUP);
      restore(&lastCatalogPosition,                sizeof(lastCatalogPosition),                BACKUP);
      restore(&lgCatalogSelection,                 sizeof(lgCatalogSelection),                 BACKUP);
      restore(displayValueX,                       sizeof(displayValueX),                      BACKUP);
      restore(&pcg32_global,                       sizeof(pcg32_global),                       BACKUP);
      restore(&exponentLimit,                      sizeof(exponentLimit),                      BACKUP);
      restore(&exponentHideLimit,                  sizeof(exponentHideLimit),                  BACKUP);
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
      firstDisplayedStep.ram = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // firstDisplayedStep offset within block
      firstDisplayedStep.ram += ramPtr;
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // currentStep pointer to block
      currentStep.ram = TO_PCMEMPTR(ramPtr);
      restore(&ramPtr,                             sizeof(ramPtr),                             BACKUP); // currentStep offset within block
      currentStep.ram += ramPtr;
      restore(&freeProgramBytes,                   sizeof(freeProgramBytes),                   BACKUP);
      restore(&firstDisplayedLocalStepNumber,      sizeof(firstDisplayedLocalStepNumber),      BACKUP);
      restore(&numberOfLabels,                     sizeof(numberOfLabels),                     BACKUP);
      restore(&numberOfLabelsInFlash,              sizeof(numberOfLabelsInFlash),              BACKUP);
      restore(&numberOfPrograms,                   sizeof(numberOfPrograms),                   BACKUP);
      restore(&numberOfProgramsInFlash,            sizeof(numberOfProgramsInFlash),            BACKUP);
      restore(&currentLocalStepNumber,             sizeof(currentLocalStepNumber),             BACKUP);
      restore(&currentProgramNumber,               sizeof(currentProgramNumber),               BACKUP);
      restore(&lastProgramListEnd,                 sizeof(lastProgramListEnd),                 BACKUP);
      restore(&programListEnd,                     sizeof(programListEnd),                     BACKUP);
      restore(&allSubroutineLevels,                sizeof(allSubroutineLevels),                BACKUP);
      restore(&pemCursorIsZerothStep,              sizeof(pemCursorIsZerothStep),              BACKUP);
      restore(&numberOfTamMenusToPop,              sizeof(numberOfTamMenusToPop),              BACKUP);
      restore(&lrSelection,                        sizeof(lrSelection),                        BACKUP);
      restore(&lrSelectionUndo,                    sizeof(lrSelectionUndo),                    BACKUP);
      restore(&lrChosen,                           sizeof(lrChosen),                           BACKUP);
      restore(&lrChosenUndo,                       sizeof(lrChosenUndo),                       BACKUP);
      restore(&lastPlotMode,                       sizeof(lastPlotMode),                       BACKUP);
      restore(&plotSelection,                      sizeof(plotSelection),                      BACKUP);

      restore(&graph_dx,                           sizeof(graph_dx),                           BACKUP);
      restore(&graph_dy,                           sizeof(graph_dy),                           BACKUP);
      restore(&roundedTicks,                       sizeof(roundedTicks),                       BACKUP);
      restore(&extentx,                            sizeof(extentx),                            BACKUP);
      restore(&extenty,                            sizeof(extenty),                            BACKUP);
      restore(&PLOT_VECT,                          sizeof(PLOT_VECT),                          BACKUP);
      restore(&PLOT_NVECT,                         sizeof(PLOT_NVECT),                         BACKUP);
      restore(&PLOT_SCALE,                         sizeof(PLOT_SCALE),                         BACKUP);
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
      restore(&matrixIndex,                        sizeof(matrixIndex),                        BACKUP);
      restore(&currentViewRegister,                sizeof(currentViewRegister),                BACKUP);
      restore(&currentSolverStatus,                sizeof(currentSolverStatus),                BACKUP);
      restore(&currentSolverProgram,               sizeof(currentSolverProgram),               BACKUP);
      restore(&currentSolverVariable,              sizeof(currentSolverVariable),              BACKUP);
      restore(&numberOfFormulae,                   sizeof(numberOfFormulae),                   BACKUP);
      restore(&currentFormula,                     sizeof(currentFormula),                     BACKUP);
      restore(&numberOfUserMenus,                  sizeof(numberOfUserMenus),                  BACKUP);
      restore(&currentUserMenu,                    sizeof(currentUserMenu),                    BACKUP);
      restore(&userKeyLabelSize,                   sizeof(userKeyLabelSize),                   BACKUP);
      restore(&timerCraAndDeciseconds,             sizeof(timerCraAndDeciseconds),             BACKUP);
      restore(&timerValue,                         sizeof(timerValue),                         BACKUP);
      restore(&timerTotalTime,                     sizeof(timerTotalTime),                     BACKUP);
      restore(&currentInputVariable,               sizeof(currentInputVariable),               BACKUP);
      restore(&SAVED_SIGMA_LASTX,                  sizeof(SAVED_SIGMA_LASTX),                  BACKUP);
      restore(&SAVED_SIGMA_LASTY,                  sizeof(SAVED_SIGMA_LASTY),                  BACKUP);
      restore(&SAVED_SIGMA_LAct,                   sizeof(SAVED_SIGMA_LAct),                   BACKUP);
      restore(&currentMvarLabel,                   sizeof(currentMvarLabel),                   BACKUP);
      restore(&graphVariable,                      sizeof(graphVariable),                      BACKUP);
      restore(&plotStatMx,                         sizeof(plotStatMx),                         BACKUP);
      restore(&drawHistogram,                      sizeof(drawHistogram),                      BACKUP);
      restore(&statMx,                             sizeof(statMx),                             BACKUP);
      restore(&lrSelectionHistobackup,             sizeof(lrSelectionHistobackup),             BACKUP);
      restore(&lrChosenHistobackup,                sizeof(lrChosenHistobackup),                BACKUP);
      restore(&loBinR,                             sizeof(loBinR),                             BACKUP);
      restore(&nBins ,                             sizeof(nBins ),                             BACKUP);
      restore(&hiBinR,                             sizeof(hiBinR),                             BACKUP);
      restore(&histElementXorY,                    sizeof(histElementXorY),                    BACKUP);

      restore(&screenUpdatingMode,                 sizeof(screenUpdatingMode),                 BACKUP);
      restore(loadedScreen,                        SCREEN_WIDTH * SCREEN_HEIGHT / 8,           BACKUP);

      restore(&eRPN,                               sizeof(eRPN),                               BACKUP);    //JM vv
      restore(&HOME3,                              sizeof(HOME3),                              BACKUP);
      restore(&ShiftTimoutMode,                    sizeof(ShiftTimoutMode),                    BACKUP);
      restore(&UNITDisplay,                        sizeof(UNITDisplay),                        BACKUP);
      restore(&SigFigMode,                         sizeof(SigFigMode),                         BACKUP);
      restore(&SH_BASE_HOME,                       sizeof(SH_BASE_HOME  ),                     BACKUP);
      restore(&Norm_Key_00_VAR,                    sizeof(Norm_Key_00_VAR),                    BACKUP);
      restore(&Input_Default,                      sizeof(Input_Default),                      BACKUP);
      restore(&jm_FG_LINE,                         sizeof(jm_FG_LINE),                         BACKUP);
      restore(&jm_BASE_SCREEN,                     sizeof(jm_BASE_SCREEN),                     BACKUP);
      restore(&jm_G_DOUBLETAP,                     sizeof(jm_G_DOUBLETAP),                     BACKUP);
      restore(&jm_HOME_SUM,                        sizeof(jm_HOME_SUM),                        BACKUP);
      restore(&graph_xmin,                         sizeof(graph_xmin),                         BACKUP);
      restore(&graph_xmax,                         sizeof(graph_xmax),                         BACKUP);
      restore(&graph_ymin,                         sizeof(graph_ymin),                         BACKUP);
      restore(&graph_ymax,                         sizeof(graph_ymax),                         BACKUP);
      restore(&jm_LARGELI,                         sizeof(jm_LARGELI),                         BACKUP);
      restore(&constantFractions,                  sizeof(constantFractions),                  BACKUP);
      restore(&constantFractionsMode,              sizeof(constantFractionsMode),              BACKUP);
      restore(&constantFractionsOn,                sizeof(constantFractionsOn),                BACKUP);
      restore(&running_program_jm,                 sizeof(running_program_jm),                 BACKUP);
      restore(&indic_x,                            sizeof(indic_x),                            BACKUP);
      restore(&indic_y,                            sizeof(indic_y),                            BACKUP);
      restore(&fnXEQMENUpos,                       sizeof(fnXEQMENUpos),                       BACKUP);
      restore(&indexOfItemsXEQM,                   sizeof(indexOfItemsXEQM),                   BACKUP);
      restore(&T_cursorPos,                        sizeof(T_cursorPos),                        BACKUP);   //JM ^^
      restore(&SHOWregis,                          sizeof(SHOWregis),                          BACKUP);   //JM ^^
      restore(&mm_MNU_HOME,                        sizeof(mm_MNU_HOME),                        BACKUP);   //JM ^^
      restore(&mm_MNU_ALPHA,                       sizeof(mm_MNU_ALPHA),                       BACKUP);   //JM ^^
      restore(&displayStackSHOIDISP,               sizeof(displayStackSHOIDISP),               BACKUP);   //JM ^^
      restore(&ListXYposition,                     sizeof(ListXYposition),                     BACKUP);   //JM ^^
      restore(&numLock,                            sizeof(numLock),                            BACKUP);   //JM ^^
      restore(&DRG_Cycling,                        sizeof(DRG_Cycling),                        BACKUP);   //JM
      restore(&lastFlgScr,                         sizeof(lastFlgScr),                         BACKUP);
      restore(&displayAIMbufferoffset,             sizeof(displayAIMbufferoffset),             BACKUP);   //C43 JM
      restore(&bcdDisplay,                         sizeof(bcdDisplay),                         BACKUP);   //C43 JM
      restore(&topHex,                             sizeof(topHex),                             BACKUP);   //C43 JM
      restore(&bcdDisplaySign,                     sizeof(bcdDisplaySign),                     BACKUP);   //C43 JM
      restore(&DM_Cycling,                         sizeof(DM_Cycling),                         BACKUP);   //JM
      restore(&SI_All,                             sizeof(SI_All),                             BACKUP);   //JM

      fclose(BACKUP);
      printf("End of calc's restoration\n");

      MY_ALPHA_MENU = mm_MNU_ALPHA;

      if(temporaryInformation == TI_SHOW_REGISTER_BIG || temporaryInformation == TI_SHOW_REGISTER_SMALL)
        temporaryInformation = TI_NO_INFO;                                                                  //JM

      if(currentProgramNumber >= (numberOfPrograms - numberOfProgramsInFlash)) {
        currentStep.flash = 1;
      }
      scanFlashPgmLibrary();
      scanLabelsAndPrograms();
      defineCurrentProgramFromGlobalStepNumber((programList[currentProgramNumber - 1].step < 0 ? -1 : 1) * (currentLocalStepNumber + abs(programList[currentProgramNumber - 1].step) - 1));
      if(programList[currentProgramNumber - 1].step < 0) {
        dynamicMenuItem = -1;
        fnGotoDot(-(currentLocalStepNumber + abs(programList[currentProgramNumber - 1].step) - 1));
      }
      defineCurrentStep();
      defineFirstDisplayedStep();
      defineCurrentProgramFromCurrentStep();

      //defineCurrentLocalRegisters();

      if(temporaryInformation==TI_SHOW_REGISTER) {
        temporaryInformation = TI_NO_INFO;
      }

      #if (DEBUG_REGISTER_L == 1)
        refreshRegisterLine(REGISTER_X); // to show L register
      #endif // (DEBUG_REGISTER_L == 1)

      for(int y = 0; y < SCREEN_HEIGHT; ++y) {
        for(int x = 0; x < SCREEN_WIDTH; x += 8) {
          uint8_t bmpdata = *(loadedScreen + (y * SCREEN_WIDTH + x) / 8);
          for(int bit = 7; bit >= 0; --bit) {
            *(screenData + y * screenStride + x + (7 - bit)) = (bmpdata & (1 << bit)) ? ON_PIXEL : OFF_PIXEL;
          }
        }
      }
      free(loadedScreen);

        if(calcMode == CM_NORMAL) {
        calcModeNormalGui();
        }
        else if(calcMode == CM_AIM) {
        calcModeAimGui();
          cursorEnabled = true;
        }
        else if(calcMode == CM_NIM) {
        calcModeNormalGui();
          cursorEnabled = true;
        }
        else if(calcMode == CM_REGISTER_BROWSER) {
        calcModeNormalGui();
        }
        else if(calcMode == CM_FLAG_BROWSER) {
        calcModeNormalGui();
        }
        else if(calcMode == CM_FONT_BROWSER) {
        calcModeNormalGui();
        }
        else if(calcMode == CM_PEM) {
        calcModeNormalGui();
        }
        else if(calcMode == CM_PLOT_STAT) {
        calcModeNormalGui();
        }
        else if(calcMode == CM_GRAPH) {   //JM
        calcModeNormalGui();
        }
        else if(calcMode == CM_LISTXY) {   //JM
        calcModeNormalGui();
        }
        else if(calcMode == CM_MIM) {
        calcModeNormalGui();
          mimRestore();
        }
        else if(calcMode == CM_EIM) {
        }
        else if(calcMode == CM_ASSIGN) {
        }
        else if(calcMode == CM_TIMER) {
        }
        else {
          sprintf(errorMessage, commonBugScreenMessages[bugMsgValueFor], "restoreCalc", calcMode, "calcMode");
          displayBugScreen(errorMessage);
        }
        if(catalog) {
          clearSystemFlag(FLAG_ALPHA);
        }

      updateMatrixHeightCache();
      refreshScreen();
    }
  }
#endif // PC_BUILD


static void registerToSaveString(calcRegister_t regist) {
  longInteger_t lgInt;
  int16_t sign;
  uint64_t value;
  char *str;
  uint8_t *cfg;

  tmpRegisterString = tmpString + START_REGISTER_VALUE;

  switch(getRegisterDataType(regist)) {
    case dtLongInteger: {
      convertLongIntegerRegisterToLongInteger(regist, lgInt);
      longIntegerToAllocatedString(lgInt, tmpRegisterString, TMP_STR_LENGTH - START_REGISTER_VALUE - 1);
      longIntegerFree(lgInt);
      strcpy(aimBuffer, "LonI");
      break;
    }

    case dtString: {
      stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)(tmpRegisterString));
      strcpy(aimBuffer, "Stri");
      break;
    }

    case dtShortInteger: {
      convertShortIntegerRegisterToUInt64(regist, &sign, &value);
      sprintf(tmpRegisterString, "%c%" PRIu64 " %" PRIu32, sign ? '-' : '+', value, getRegisterShortIntegerBase(regist));
      strcpy(aimBuffer, "ShoI");
      break;
    }

    case dtReal34: {
      real34ToString(REGISTER_REAL34_DATA(regist), tmpRegisterString);
      switch(getRegisterAngularMode(regist)) {
        case amDegree: {
          strcpy(aimBuffer, "Real:DEG");
          break;
        }

        case amDMS: {
          strcpy(aimBuffer, "Real:DMS");
          break;
        }

        case amRadian: {
          strcpy(aimBuffer, "Real:RAD");
          break;
        }

        case amMultPi: {
          strcpy(aimBuffer, "Real:MULTPI");
          break;
        }

        case amGrad: {
          strcpy(aimBuffer, "Real:GRAD");
          break;
        }

        case amNone: {
          strcpy(aimBuffer, "Real");
          break;
        }

        default: {
          strcpy(aimBuffer, "Real:???");
          break;
      }
      }
      break;
    }

    case dtComplex34: {
      real34ToString(REGISTER_REAL34_DATA(regist), tmpRegisterString);
      strcat(tmpRegisterString, " ");
      real34ToString(REGISTER_IMAG34_DATA(regist), tmpRegisterString + strlen(tmpRegisterString));
      strcpy(aimBuffer, "Cplx");
      break;
    }

    case dtTime: {
      real34ToString(REGISTER_REAL34_DATA(regist), tmpRegisterString);
      strcpy(aimBuffer, "Time");
      break;
    }

    case dtDate: {
      real34ToString(REGISTER_REAL34_DATA(regist), tmpRegisterString);
      strcpy(aimBuffer, "Date");
      break;
    }

    case dtReal34Matrix: {
      sprintf(tmpRegisterString, "%" PRIu16 " %" PRIu16, REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows, REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns);
      strcpy(aimBuffer, "Rema");
      break;
    }

    case dtComplex34Matrix: {
      sprintf(tmpRegisterString, "%" PRIu16 " %" PRIu16, REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixRows, REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixColumns);
      strcpy(aimBuffer, "Cxma");
      break;
    }

    case dtConfig: {
      for(str=tmpRegisterString, cfg=(uint8_t *)REGISTER_CONFIG_DATA(regist), value=0; value<sizeof(dtConfigDescriptor_t); value++, cfg++, str+=2) {
        sprintf(str, "%02X", *cfg);
      }
      strcpy(aimBuffer, "Conf");
      break;
    }

    default: {
      strcpy(tmpRegisterString, "???");
      strcpy(aimBuffer, "????");
  }
}
}


static void saveMatrixElements(calcRegister_t regist, void *stream) {
  #if !defined(TESTSUITE_BUILD)
  if(getRegisterDataType(regist) == dtReal34Matrix) {
    for(uint32_t element = 0; element < REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows * REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns; ++element) {
      real34ToString(REGISTER_REAL34_MATRIX_M_ELEMENTS(regist) + element, tmpString);
      strcat(tmpString, "\n");
      save(tmpString, strlen(tmpString), stream);
    }
  }
  else if(getRegisterDataType(regist) == dtComplex34Matrix) {
    for(uint32_t element = 0; element < REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixRows * REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixColumns; ++element) {
      real34ToString(VARIABLE_REAL34_DATA(REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist) + element), tmpString);
      strcat(tmpString, " ");
      real34ToString(VARIABLE_IMAG34_DATA(REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist) + element), tmpString + strlen(tmpString));
      strcat(tmpString, "\n");
      save(tmpString, strlen(tmpString), stream);
    }
  }
  #endif // !TESTSUITE_BUILD
}


static void doSave(uint16_t saveType);

void fnSaveAuto(void) {
  doSave(autoSave);
}

void fnSave(uint16_t unusedButMandatoryParameter) {
  doSave(manualSave);
}


void doSave(uint16_t saveType) {
  calcRegister_t regist;
  uint32_t i;

  #if defined(DMCP_BUILD)
    FRESULT result;
    char fileName[30];
    fileName[0] = 0;
    if(saveType == manualSave) {
      strcpy(fileName, "SAVFILES\\C43.sav");
    } else if(saveType == autoSave) {
      strcpy(fileName, "SAVFILES\\C43auto.sav");
    } 
    sys_disk_write_enable(1);
    check_create_dir("SAVFILES");
    result = f_open(BACKUP, fileName, FA_CREATE_ALWAYS | FA_WRITE);
    if(result != FR_OK) {
      sys_disk_write_enable(0);
      return;
    }
  #else // !DMCP_BUILD
    FILE *ppgm_fp;

    BACKUP = fopen("C43.sav", "wb");
    if(BACKUP == NULL) {
      printf("Cannot SAVE in file C43.sav!\n");
      return;
    }
  #endif // DMCP_BUILD
  // SAV file version number

  sprintf(tmpString, "SAVE_FILE_REVISION\n%" PRIu8 "\n", (uint8_t)0);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "C43_save_file_00\n%" PRIu32 "\n", (uint32_t)configFileVersion);
  save(tmpString, strlen(tmpString), BACKUP);


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

  // Keyboard arguments
  sprintf(tmpString, "KEYBOARD_ARGUMENTS\n");
  save(tmpString, strlen(tmpString), BACKUP);

  uint32_t num = 0;
  for(i = 0; i < 37 * 6; ++i) {
    if(*(getNthString((uint8_t *)userKeyLabel, i)) != 0) {
      ++num;
    }
  }
  sprintf(tmpString, "%" PRIu32 "\n", num);
  save(tmpString, strlen(tmpString), BACKUP);

  for(i = 0; i < 37 * 6; ++i) {
    if(*(getNthString((uint8_t *)userKeyLabel, i)) != 0) {
      sprintf(tmpString, "%" PRIu32 " ", i);
      stringToUtf8((char *)getNthString((uint8_t *)userKeyLabel, i), (uint8_t *)tmpString + strlen(tmpString));
      strcat(tmpString, "\n");
      save(tmpString, strlen(tmpString), BACKUP);
    }
  }

  // MyMenu
  sprintf(tmpString, "MYMENU\n18\n");
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<18; i++) {
    sprintf(tmpString, "%" PRId16, userMenuItems[i].item);
    if(userMenuItems[i].argumentName[0] != 0) {
      strcat(tmpString, " ");
      stringToUtf8(userMenuItems[i].argumentName, (uint8_t *)tmpString + strlen(tmpString));
    }
    strcat(tmpString, "\n");
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // MyAlpha
  sprintf(tmpString, "MYALPHA\n18\n");
  save(tmpString, strlen(tmpString), BACKUP);
  for(i=0; i<18; i++) {
    sprintf(tmpString, "%" PRId16, userAlphaItems[i].item);
    if(userAlphaItems[i].argumentName[0] != 0) {
      strcat(tmpString, " ");
      stringToUtf8(userAlphaItems[i].argumentName, (uint8_t *)tmpString + strlen(tmpString));
    }
    strcat(tmpString, "\n");
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // User menus
  sprintf(tmpString, "USER_MENUS\n");
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "%" PRIu16 "\n", numberOfUserMenus);
  save(tmpString, strlen(tmpString), BACKUP);
  for(uint32_t j = 0; j < numberOfUserMenus; ++j) {
    stringToUtf8(userMenus[j].menuName, (uint8_t *)tmpString);
    strcat(tmpString, "\n18\n");
    save(tmpString, strlen(tmpString), BACKUP);
    for(i=0; i<18; i++) {
      sprintf(tmpString, "%" PRId16, userMenus[j].menuItem[i].item);
      if(userMenus[j].menuItem[i].argumentName[0] != 0) {
        strcat(tmpString, " ");
        stringToUtf8(userMenus[j].menuItem[i].argumentName, (uint8_t *)tmpString + strlen(tmpString));
      }
      strcat(tmpString, "\n");
      save(tmpString, strlen(tmpString), BACKUP);
    }
  }

  // Programs
  uint16_t currentSizeInBlocks = RAM_SIZE - freeMemoryRegions[numberOfFreeMemoryRegions - 1].address - freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks;
  sprintf(tmpString, "PROGRAMS\n%" PRIu16 "\n", currentSizeInBlocks);
  save(tmpString, strlen(tmpString), BACKUP);

  sprintf(tmpString, "%" PRIu32 "\n%" PRIu32 "\n", (uint32_t)TO_WP43MEMPTR(currentStep.ram), (uint32_t)((void *)currentStep.ram - TO_PCMEMPTR(TO_WP43MEMPTR(currentStep.ram)))); // currentStep block pointer + offset within block
  save(tmpString, strlen(tmpString), BACKUP);

  sprintf(tmpString, "%" PRIu32 "\n%" PRIu32 "\n", (uint32_t)TO_WP43MEMPTR(firstFreeProgramByte), (uint32_t)((void *)firstFreeProgramByte - TO_PCMEMPTR(TO_WP43MEMPTR(firstFreeProgramByte)))); // firstFreeProgramByte block pointer + offset within block
  save(tmpString, strlen(tmpString), BACKUP);

  sprintf(tmpString, "%" PRIu16 "\n", freeProgramBytes);
  save(tmpString, strlen(tmpString), BACKUP);

  for(i=0; i<currentSizeInBlocks; i++) {
    sprintf(tmpString, "%" PRIu32 "\n", *(((uint32_t *)(beginOfProgramMemory)) + i));
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // Equations
  sprintf(tmpString, "EQUATIONS\n%" PRIu16 "\n", numberOfFormulae);
  save(tmpString, strlen(tmpString), BACKUP);

  for(i=0; i<numberOfFormulae; i++) {
    stringToUtf8(TO_PCMEMPTR(allFormulae[i].pointerToFormulaData), (uint8_t *)tmpString);
    strcat(tmpString, "\n");
    save(tmpString, strlen(tmpString), BACKUP);
  }

  // Other configuration stuff
  sprintf(tmpString, "OTHER_CONFIGURATION_STUFF\n39\n"); //JM 16+23
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
  sprintf(tmpString, "exponentHideLimit\n%" PRId16 "\n", exponentHideLimit);
  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "notBestF\n%" PRIu16 "\n", lrSelection);
  save(tmpString, strlen(tmpString), BACKUP);
//Number 16: digit

//11
  sprintf(tmpString, "SigFigMode\n%"          PRIu8 "\n",       SigFigMode);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "eRPN\n%"                PRIu8 "\n",       (uint8_t)eRPN);                save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "HOME3\n%"               PRIu8 "\n",       (uint8_t)HOME3);               save(tmpString, strlen(tmpString), BACKUP); 
  sprintf(tmpString, "ShiftTimoutMode\n%"     PRIu8 "\n",       (uint8_t)ShiftTimoutMode);     save(tmpString, strlen(tmpString), BACKUP);           
  sprintf(tmpString, "UNITDisplay\n%"         PRIu8 "\n",       (uint8_t)UNITDisplay);         save(tmpString, strlen(tmpString), BACKUP);       
  sprintf(tmpString, "SH_BASE_HOME\n%"        PRIu8 "\n",       (uint8_t)SH_BASE_HOME);        save(tmpString, strlen(tmpString), BACKUP);        
  sprintf(tmpString, "Norm_Key_00_VAR\n%"     PRId16 "\n",      Norm_Key_00_VAR);              save(tmpString, strlen(tmpString), BACKUP);           
  sprintf(tmpString, "Input_Default\n%"       PRIu8 "\n",       Input_Default);                save(tmpString, strlen(tmpString), BACKUP);         
  sprintf(tmpString, "jm_FG_LINE\n%"          PRIu8 "\n",       (uint8_t)jm_FG_LINE);          save(tmpString, strlen(tmpString), BACKUP);      
  sprintf(tmpString, "jm_BASE_SCREEN\n%"      PRIu8 "\n",       (uint8_t)jm_BASE_SCREEN);      save(tmpString, strlen(tmpString), BACKUP);             
  sprintf(tmpString, "jm_G_DOUBLETAP\n%"      PRIu8 "\n",       (uint8_t)jm_G_DOUBLETAP);      save(tmpString, strlen(tmpString), BACKUP);          


/*
  float  graph_xmin;
  float  graph_xmax;
  float  graph_ymin;
  float  graph_ymax;
  float  graph_dx;
  float  graph_dy;
  bool_t roundedTicks;
  bool_t extentx;
  bool_t extenty;
  bool_t PLOT_VECT;
  bool_t PLOT_NVECT;
  bool_t PLOT_SCALE;
  bool_t Aspect_Square;
  bool_t PLOT_LINE;
  bool_t PLOT_CROSS;
  bool_t PLOT_BOX;
  bool_t PLOT_INTG;
  bool_t PLOT_DIFF;
  bool_t PLOT_RMS;
  bool_t PLOT_SHADE;
  bool_t PLOT_AXIS;
  int8_t PLOT_ZMX;
  int8_t PLOT_ZMY;

  sprintf(tmpString, "graph_xmin\n%"                            graph_xmin);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "graph_xmax\n%"                            graph_xmax);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "graph_ymin\n%"                            graph_ymin);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "graph_ymax\n%"                            graph_ymax);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "graph_dx\n%"                              graph_dx);                     save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "graph_dy\n%"                              graph_dy);                     save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "roundedTicks\n%"                          roundedTicks);                 save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "extentx\n%"                               extentx);                      save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "extenty\n%"                               extenty);                      save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_VECT\n%"                             PLOT_VECT);                    save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_NVECT\n%"                            PLOT_NVECT);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_SCALE\n%"                            PLOT_SCALE);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "Aspect_Square\n%"                         Aspect_Square);                save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_LINE\n%"                             PLOT_LINE);                    save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_CROSS\n%"                            PLOT_CROSS);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_BOX\n%"                              PLOT_BOX);                     save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_INTG\n%"                             PLOT_INTG);                    save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_DIFF\n%"                             PLOT_DIFF);                    save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_RMS\n%"                              PLOT_RMS);                     save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_SHADE\n%"                            PLOT_SHADE);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_AXIS\n%"                             PLOT_AXIS);                    save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_ZMX\n%"                              PLOT_ZMX);                     save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "PLOT_ZMY\n%"                              PLOT_ZMY);                     save(tmpString, strlen(tmpString), BACKUP);
*/

//12
  sprintf(tmpString, "jm_HOME_SUM\n%"           PRIu8 "\n",     (uint8_t)jm_HOME_SUM);         save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "jm_LARGELI\n%"            PRIu8 "\n",     (uint8_t)jm_LARGELI);          save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "constantFractions\n%"     PRIu8 "\n",     (uint8_t)constantFractions);   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "constantFractionsMode\n%" PRIu8 "\n",     constantFractionsMode);        save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "constantFractionsOn\n%"   PRIu8 "\n",     (uint8_t)constantFractionsOn); save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "displayStackSHOIDISP\n%"  PRIu8 "\n",     displayStackSHOIDISP);         save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "bcdDisplay\n%"            PRIu8 "\n",     (uint8_t)bcdDisplay);          save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "topHex\n%"                PRIu8 "\n",     (uint8_t)topHex);              save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "bcdDisplaySign\n%"        PRIu8 "\n",     bcdDisplaySign);               save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "DRG_Cycling\n%"           PRIu8 "\n",     DRG_Cycling);                  save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "DM_Cycling\n%"            PRIu8 "\n",     DM_Cycling);                   save(tmpString, strlen(tmpString), BACKUP);
  sprintf(tmpString, "SI_All\n%"                PRIu8 "\n",     (uint8_t)SI_All);              save(tmpString, strlen(tmpString), BACKUP);




  #if defined(DMCP_BUILD)
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
    if(type[5] == 'R') {
      tag = amRadian;
    }
    else if(type[5] == 'M') {
      tag = amMultPi;
    }
    else if(type[5] == 'G') {
      tag = amGrad;
    }
    else if(type[5] == 'D' && type[6] == 'E') {
      tag = amDegree;
    }
    else if(type[5] == 'D' && type[6] == 'M') {
      tag = amDMS;
    }
    else {
      tag = amNone;
    }

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

    while(*value != ' ') {
      value++;
    }
    while(*value == ' ') {
      value++;
    }
    uint32_t base = stringToUint32(value);

    convertUInt64ToShortIntegerRegister(sign, val, base, regist);
  }

  else if(strcmp(type, "Cplx") == 0) {
    char *imaginaryPart;

    reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, amNone);
    imaginaryPart = value;
    while(*imaginaryPart != ' ') {
      imaginaryPart++;
    }
    *(imaginaryPart++) = 0;
    stringToReal34(value, REGISTER_REAL34_DATA(regist));
    stringToReal34(imaginaryPart, REGISTER_IMAG34_DATA(regist));
  }

#if !defined(TESTSUITE_BUILD)
  else if(strcmp(type, "Rema") == 0) {
    char *numOfCols;
    uint16_t rows, cols;

    numOfCols = value;
    while(*numOfCols != ' ') {
      numOfCols++;
    }
    *(numOfCols++) = 0;
    rows = stringToUint16(value);
    cols = stringToUint16(numOfCols);
    reallocateRegister(regist, dtReal34Matrix, REAL34_SIZE * rows * cols, amNone);
    REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows = rows;
    REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns = cols;
  }

  else if(strcmp(type, "Cxma") == 0) {
    char *numOfCols;
    uint16_t rows, cols;

    numOfCols = value;
    while(*numOfCols != ' ') {
      numOfCols++;
    }
    *(numOfCols++) = 0;
    rows = stringToUint16(value);
    cols = stringToUint16(numOfCols);
    reallocateRegister(regist, dtComplex34Matrix, COMPLEX34_SIZE * rows * cols, amNone);
    REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixRows = rows;
    REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixColumns = cols;
  }
#endif // TESTSUITE_BUILD

  else if(strcmp(type, "Conf") == 0) {
    char *cfg;

    reallocateRegister(regist, dtConfig, CONFIG_SIZE, amNone);
    for(cfg=(char *)REGISTER_CONFIG_DATA(regist), tag=0; tag<sizeof(dtConfigDescriptor_t); tag++, value+=2, cfg++) {
      *cfg = ((*value >= 'A' ? *value - 'A' + 10 : *value - '0') << 4) | (*(value + 1) >= 'A' ? *(value + 1) - 'A' + 10 : *(value + 1) - '0');
    }
  }

  else {
    sprintf(errorMessage, "In function restoreRegister: Date type %s is to be coded!", type);
    displayBugScreen(errorMessage);
  }
}


static void restoreMatrixData(calcRegister_t regist, void *stream) {
  #if !defined(TESTSUITE_BUILD)
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

  if(getRegisterDataType(regist) == dtComplex34Matrix) {
    rows = REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixRows;
    cols = REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixColumns;

    for(i = 0; i < rows * cols; ++i) {
      char *imaginaryPart;

      readLine(stream, tmpString);
      imaginaryPart = tmpString;
        while(*imaginaryPart != ' ') {
          imaginaryPart++;
        }
      *(imaginaryPart++) = 0;
      stringToReal34(tmpString,     VARIABLE_REAL34_DATA(REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist) + i));
      stringToReal34(imaginaryPart, VARIABLE_IMAG34_DATA(REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist) + i));
    }
  }
  #endif // !TESTSUITE_BUILD
}


static void skipMatrixData(char *type, char *value, void *stream) {
  #if !defined(TESTSUITE_BUILD)
  uint16_t rows, cols;
  uint32_t i;
  char *numOfCols;

  if(strcmp(type, "Rema") == 0 || strcmp(type, "Cxma") == 0) {
    numOfCols = value;
      while(*numOfCols != ' ') {
        numOfCols++;
      }
    *(numOfCols++) = 0;
    rows = stringToUint16(value);
    cols = stringToUint16(numOfCols);

    for(i = 0; i < rows * cols; ++i) {
      readLine(stream, tmpString);
    }
  }
  #endif // !TESTSUITE_BUILD
}



static bool_t restoreOneSection(void *stream, uint16_t loadMode, uint16_t s, uint16_t n, uint16_t d) {
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

      if(loadMode == LM_ALL || (loadMode == LM_REGISTERS && regist < REGISTER_X) || (loadMode == LM_REGISTERS_PARTIAL && regist >= s && regist < (s + n))) {
        restoreRegister(loadMode == LM_REGISTERS_PARTIAL ? (regist - s + d) : regist, aimBuffer, tmpString);
        restoreMatrixData(loadMode == LM_REGISTERS_PARTIAL ? (regist - s + d) : regist, stream);
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

      while(*str != ' ') {
        str++;
      }
      while(*str == ' ') {
        str++;
      }
      globalFlags[1] = stringToInt16(str);

      while(*str != ' ') {
        str++;
      }
      while(*str == ' ') {
        str++;
      }
      globalFlags[2] = stringToInt16(str);

      while(*str != ' ') {
        str++;
      }
      while(*str == ' ') {
        str++;
      }
      globalFlags[3] = stringToInt16(str);

      while(*str != ' ') {
        str++;
      }
      while(*str == ' ') {
        str++;
      }
      globalFlags[4] = stringToInt16(str);

      while(*str != ' ') {
        str++;
      }
      while(*str == ' ') {
        str++;
      }
      globalFlags[5] = stringToInt16(str);

      while(*str != ' ') {
        str++;
      }
      while(*str == ' ') {
        str++;
      }
      globalFlags[6] = stringToInt16(str);
    }
  }

  else if(strcmp(tmpString, "LOCAL_REGISTERS") == 0) {
    readLine(stream, tmpString); // Number of local registers
    numberOfRegs = stringToInt16(tmpString);
    if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
      allocateLocalRegisters(numberOfRegs);
    }

    if((loadMode != LM_ALL && loadMode != LM_REGISTERS) || lastErrorCode == ERROR_NONE) {
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
    }
  }

  else if(strcmp(tmpString, "LOCAL_FLAGS") == 0) {
    readLine(stream, tmpString); // LOCAL_FLAGS
    if(loadMode == LM_ALL || loadMode == LM_REGISTERS) {
      currentLocalFlags->localFlags = stringToUint32(tmpString);
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
      if(statisticalSumsPointer) { // likely
        if(loadMode == LM_ALL || loadMode == LM_SUMS) {
          stringToReal(tmpString, (real_t *)(statisticalSumsPointer + REAL_SIZE * i), &ctxtReal75);
        }
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
        kbd_usr[i].keyId = stringToInt16(str);

        while(*str != ' ') {
          str++;
        }
        while(*str == ' ') {
          str++;
        }
        kbd_usr[i].primary = stringToInt16(str);

        while(*str != ' ') {
          str++;
        }
        while(*str == ' ') {
          str++;
        }
        kbd_usr[i].fShifted = stringToInt16(str);

        while(*str != ' ') {
          str++;
        }
        while(*str == ' ') {
          str++;
        }
        kbd_usr[i].gShifted = stringToInt16(str);

        while(*str != ' ') {
          str++;
        }
        while(*str == ' ') {
          str++;
        }
        kbd_usr[i].keyLblAim = stringToInt16(str);

        while(*str != ' ') {
          str++;
        }
        while(*str == ' ') {
          str++;
        }
        kbd_usr[i].primaryAim = stringToInt16(str);

        while(*str != ' ') {
          str++;
        }
        while(*str == ' ') {
          str++;
        }
        kbd_usr[i].fShiftedAim = stringToInt16(str);

        while(*str != ' ') {
          str++;
        }
        while(*str == ' ') {
          str++;
        }
        kbd_usr[i].gShiftedAim = stringToInt16(str);

        while(*str != ' ') {
          str++;
        }
        while(*str == ' ') {
          str++;
        }
        kbd_usr[i].primaryTam = stringToInt16(str);
      }
    }
  }

  else if(strcmp(tmpString, "KEYBOARD_ARGUMENTS") == 0) {
    readLine(stream, tmpString); // Number of keys
    numberOfRegs = stringToInt16(tmpString);
    if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
      freeWp43(userKeyLabel, TO_BLOCKS(userKeyLabelSize));
      userKeyLabelSize = 37/*keys*/ * 6/*states*/ * 1/*byte terminator*/ + 1/*byte sentinel*/;
      userKeyLabel = allocWp43(TO_BLOCKS(userKeyLabelSize));
      memset(userKeyLabel,   0, TO_BYTES(TO_BLOCKS(userKeyLabelSize)));
    }
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpString); // key
      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        str = tmpString;
        uint16_t key = stringToUint16(str);
        userMenuItems[i].argumentName[0] = 0;

        while((*str != ' ') && (*str != '\n') && (*str != 0)) {
          str++;
        }
        if(*str == ' ') {
          while(*str == ' ') {
            str++;
          }
          if((*str != '\n') && (*str != 0)) {
            utf8ToString((uint8_t *)str, tmpString + TMP_STR_LENGTH / 2);
            setUserKeyArgument(key, tmpString + TMP_STR_LENGTH / 2);
          }
        }
      }
    }
  }

  else if(strcmp(tmpString, "MYMENU") == 0) {
    readLine(stream, tmpString); // Number of keys
    numberOfRegs = stringToInt16(tmpString);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpString); // key
      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        str = tmpString;
        userMenuItems[i].item            = stringToInt16(str);
        userMenuItems[i].argumentName[0] = 0;

        while((*str != ' ') && (*str != '\n') && (*str != 0)) {
          str++;
        }
        if(*str == ' ') {
          while(*str == ' ') {
            str++;
          }
          if((*str != '\n') && (*str != 0)) {
            utf8ToString((uint8_t *)str, userMenuItems[i].argumentName);
          }
        }
      }
    }
  }

  else if(strcmp(tmpString, "MYALPHA") == 0) {
    readLine(stream, tmpString); // Number of keys
    numberOfRegs = stringToInt16(tmpString);
    for(i=0; i<numberOfRegs; i++) {
      readLine(stream, tmpString); // key
      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        str = tmpString;
        userAlphaItems[i].item            = stringToInt16(str);
        userAlphaItems[i].argumentName[0] = 0;

        while((*str != ' ') && (*str != '\n') && (*str != 0)) {
          str++;
        }
        if(*str == ' ') {
          while(*str == ' ') {
            str++;
          }
          if((*str != '\n') && (*str != 0)) {
            utf8ToString((uint8_t *)str, userAlphaItems[i].argumentName);
          }
        }
      }
    }
  }

  else if(strcmp(tmpString, "USER_MENUS") == 0) {
    readLine(stream, tmpString); // Number of keys
    int16_t numberOfMenus = stringToInt16(tmpString);
    for(int32_t j=0; j<numberOfMenus; j++) {
      readLine(stream, tmpString);
      int16_t target = -1;
      if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
        utf8ToString((uint8_t *)tmpString, tmpString + TMP_STR_LENGTH / 2);
        for(i = 0; i < numberOfUserMenus; ++i) {
          if(compareString(tmpString + TMP_STR_LENGTH / 2, userMenus[i].menuName, CMP_NAME) == 0) {
            target = i;
          }
        }
        if(target == -1) {
          createMenu(tmpString + TMP_STR_LENGTH / 2);
          target = numberOfUserMenus - 1;
        }
      }

      readLine(stream, tmpString);
      numberOfRegs = stringToInt16(tmpString);
      for(i=0; i<numberOfRegs; i++) {
        readLine(stream, tmpString); // key
        if(loadMode == LM_ALL || loadMode == LM_SYSTEM_STATE) {
          str = tmpString;
          userMenus[target].menuItem[i].item            = stringToInt16(str);
          userMenus[target].menuItem[i].argumentName[0] = 0;

          while((*str != ' ') && (*str != '\n') && (*str != 0)) {
            str++;
          }
          if(*str == ' ') {
            while(*str == ' ') {
              str++;
            }
            if((*str != '\n') && (*str != 0)) {
              utf8ToString((uint8_t *)str, userMenus[target].menuItem[i].argumentName);
            }
          }
        }
      }
    }
  }

  else if(strcmp(tmpString, "PROGRAMS") == 0) {
    uint16_t numberOfBlocks;
    uint16_t oldSizeInBlocks = RAM_SIZE - freeMemoryRegions[numberOfFreeMemoryRegions - 1].address - freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks;
    uint8_t *oldFirstFreeProgramByte = firstFreeProgramByte;
    uint16_t oldFreeProgramBytes = freeProgramBytes;

    readLine(stream, tmpString); // Number of blocks
    numberOfBlocks = stringToUint16(tmpString);
    if(loadMode == LM_ALL) {
      resizeProgramMemory(numberOfBlocks);
    }
    else if(loadMode == LM_PROGRAMS) {
      resizeProgramMemory(oldSizeInBlocks + numberOfBlocks);
      oldFirstFreeProgramByte = beginOfProgramMemory + TO_BYTES(oldSizeInBlocks) - oldFreeProgramBytes - 2;
    }

    readLine(stream, tmpString); // currentStep (pointer to block)
    if(loadMode == LM_ALL) {
      currentStep.ram = TO_PCMEMPTR(stringToUint32(tmpString));
    }
    readLine(stream, tmpString); // currentStep (offset in bytes within block)
    if(loadMode == LM_ALL) {
      currentStep.ram += stringToUint32(tmpString);
    }
    else if(loadMode == LM_PROGRAMS) {
      if(programList[currentProgramNumber - 1].step > 0) {
        currentStep.ram           -= TO_BYTES(numberOfBlocks);
        firstDisplayedStep.ram    -= TO_BYTES(numberOfBlocks);
        beginOfCurrentProgram.ram -= TO_BYTES(numberOfBlocks);
        endOfCurrentProgram.ram   -= TO_BYTES(numberOfBlocks);
      }
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

    if(loadMode == LM_PROGRAMS) { // .END. to END
      freeProgramBytes += oldFreeProgramBytes;
      if((oldFirstFreeProgramByte >= (beginOfProgramMemory + 2)) && (*(oldFirstFreeProgramByte - 2) == ((ITM_END >> 8) | 0x80)) && (*(oldFirstFreeProgramByte - 1) == (ITM_END & 0xff))) {
      }
      else {
        if(oldFreeProgramBytes + freeProgramBytes < 2) {
          uint16_t tmpFreeProgramBytes = freeProgramBytes;
          resizeProgramMemory(oldSizeInBlocks + numberOfBlocks + 1);
          oldFirstFreeProgramByte -= 4;
          freeProgramBytes = tmpFreeProgramBytes + 4;
          if(programList[currentProgramNumber - 1].step > 0) {
            currentStep.ram           -= 4;
            firstDisplayedStep.ram    -= 4;
            beginOfCurrentProgram.ram -= 4;
            endOfCurrentProgram.ram   -= 4;
          }
        }
        *(oldFirstFreeProgramByte    ) = (ITM_END >> 8) | 0x80;
        *(oldFirstFreeProgramByte + 1) =  ITM_END       & 0xff;
        freeProgramBytes -= 2;
        oldFirstFreeProgramByte += 2;
      }
    }

    for(i=0; i<numberOfBlocks; i++) {
      readLine(stream, tmpString); // One block
      if(loadMode == LM_ALL) {
        *(((uint32_t *)(beginOfProgramMemory)) + i) = stringToUint32(tmpString);
      }
      else if(loadMode == LM_PROGRAMS) {
        uint32_t tmpBlock = stringToUint32(tmpString);
        xcopy(oldFirstFreeProgramByte + TO_BYTES(i), (uint8_t *)(&tmpBlock), 4);
      }
    }

    scanLabelsAndPrograms();
  }

  else if(strcmp(tmpString, "EQUATIONS") == 0) {
    uint16_t formulae;

    if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
      for(i = numberOfFormulae; i > 0; --i) {
        deleteEquation(i - 1);
      }
    }

    readLine(stream, tmpString); // Number of formulae
    formulae = stringToUint16(tmpString);
    if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
      allFormulae = allocWp43(TO_BLOCKS(sizeof(formulaHeader_t)) * formulae);
      numberOfFormulae = formulae;
      currentFormula = 0;
      for(i = 0; i < formulae; i++) {
        allFormulae[i].pointerToFormulaData = WP43_NULL;
        allFormulae[i].sizeInBlocks = 0;
      }
    }

    for(i = 0; i < formulae; i++) {
      readLine(stream, tmpString); // One formula
      if(loadMode == LM_ALL || loadMode == LM_PROGRAMS) {
        utf8ToString((uint8_t *)tmpString, tmpString + TMP_STR_LENGTH / 2);
        setEquation(i, tmpString + TMP_STR_LENGTH / 2);
      }
    }
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
          while(*str != ' ') {
            str++;
          }
          while(*str == ' ') {
            str++;
          }
          pcg32_global.inc = stringToUint64(str);
        }
        else if(strcmp(aimBuffer, "exponentLimit") == 0) {
          exponentLimit = stringToInt16(tmpString);
        }
        else if(strcmp(aimBuffer, "exponentHideLimit") == 0) {
          exponentHideLimit = stringToInt16(tmpString);
        }
        else if(strcmp(aimBuffer, "notBestF") == 0) {
          lrSelection = stringToUint16(tmpString);
        }

        else if(strcmp(aimBuffer, "SigFigMode"                  ) == 0) { SigFigMode            = stringToUint8(tmpString); }
        else if(strcmp(aimBuffer, "eRPN"                        ) == 0) { eRPN                  = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "HOME3"                       ) == 0) { HOME3                 = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "ShiftTimoutMode"             ) == 0) { ShiftTimoutMode       = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "UNITDisplay"                 ) == 0) { UNITDisplay           = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "SH_BASE_HOME"                ) == 0) { SH_BASE_HOME          = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "Norm_Key_00_VAR"             ) == 0) { Norm_Key_00_VAR       = stringToUint16(tmpString); }
        else if(strcmp(aimBuffer, "Input_Default"               ) == 0) { Input_Default         = stringToUint8(tmpString); }
        else if(strcmp(aimBuffer, "jm_FG_LINE"                  ) == 0) { jm_FG_LINE            = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "jm_BASE_SCREEN"              ) == 0) { jm_BASE_SCREEN        = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "jm_G_DOUBLETAP"              ) == 0) { jm_G_DOUBLETAP        = (bool_t)stringToUint8(tmpString) != 0; }

/*
        else if(strcmp(aimBuffer, "graph_xmin\n"                            graph_xmin);                   save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "graph_xmax\n"                            graph_xmax);                   save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "graph_ymin\n"                            graph_ymin);                   save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "graph_ymax\n"                            graph_ymax);                   save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "graph_dx\n"                              graph_dx);                     save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "graph_dy\n"                              graph_dy);                     save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "roundedTicks\n"                          roundedTicks);                 save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "extentx\n"                               extentx);                      save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "extenty\n"                               extenty);                      save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_VECT\n"                             PLOT_VECT);                    save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_NVECT\n"                            PLOT_NVECT);                   save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_SCALE\n"                            PLOT_SCALE);                   save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "Aspect_Square\n"                         Aspect_Square);                save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_LINE\n"                             PLOT_LINE);                    save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_CROSS\n"                            PLOT_CROSS);                   save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_BOX\n"                              PLOT_BOX);                     save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_INTG\n"                             PLOT_INTG);                    save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_DIFF\n"                             PLOT_DIFF);                    save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_RMS\n"                              PLOT_RMS);                     save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_SHADE\n"                            PLOT_SHADE);                   save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_AXIS\n"                             PLOT_AXIS);                    save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_ZMX\n"                              PLOT_ZMX);                     save(tmpString, strlen(tmpString), BACKUP);
        else if(strcmp(aimBuffer, "PLOT_ZMY\n"                              PLOT_ZMY);                     save(tmpString, strlen(tmpString), BACKUP);
*/
        else if(strcmp(aimBuffer, "jm_HOME_SUM"                 ) == 0) { jm_HOME_SUM           = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "jm_LARGELI"                  ) == 0) { jm_LARGELI            = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "constantFractions"           ) == 0) { constantFractions     = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "constantFractionsMode"       ) == 0) { constantFractionsMode = stringToUint8(tmpString); }
        else if(strcmp(aimBuffer, "constantFractionsOn"         ) == 0) { constantFractionsOn   = (bool_t)stringToUint8(tmpString) != 0; }


        else if(strcmp(aimBuffer, "displayStackSHOIDISP"        ) == 0) { displayStackSHOIDISP = stringToUint8(tmpString); }
        else if(strcmp(aimBuffer, "bcdDisplay"                  ) == 0) { bcdDisplay           = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "topHex"                      ) == 0) { topHex               = (bool_t)stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "bcdDisplaySign"              ) == 0) { bcdDisplaySign       = stringToUint8(tmpString) != 0; }
        else if(strcmp(aimBuffer, "DRG_Cycling"                 ) == 0) { DRG_Cycling          = stringToUint8(tmpString); }
        else if(strcmp(aimBuffer, "DM_Cycling"                  ) == 0) { DM_Cycling           = stringToUint8(tmpString); }
        else if(strcmp(aimBuffer, "SI_All"                      ) == 0) { SI_All               = (bool_t)stringToUint8(tmpString) != 0; }

      }
    }
    return false; //Signal that this was the last section loaded and no more sections to follow
  }

  return true; //Signal to continue loading the next section
}



void doLoad(uint16_t loadMode, uint16_t s, uint16_t n, uint16_t d, uint16_t loadType) {
  #if defined(DMCP_BUILD)
    char fileName[30];
    fileName[0] = 0;
    if(loadType == manualLoad) {
      strcpy(fileName, "SAVFILES\\C43.sav");
    } else if(loadType == autoLoad) {
      strcpy(fileName, "SAVFILES\\C43auto.sav");
    }
    if(f_open(BACKUP, fileName, FA_READ) != FR_OK) {
      displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        char errMsg[200];
        strcpy(errMsg,"In function fnLoad: cannot find or read backup data file ");
        strcat(errMsg,fileName);
        moreInfoOnError(errMsg, NULL, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  #else // !DMCP_BUILD
    FILE *ppgm_fp;

    if((BACKUP = fopen("C43.sav", "rb")) == NULL) {
      displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnLoad: cannot find or read backup data file C43.sav", NULL, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  #endif // DMCP_BUILD

  if(loadMode == LM_ALL) {
    while(currentSubroutineLevel > 0) {
      fnReturn(0);
    }
  }


  //Tentatively insist on perfect version match for autoloaded sav file
  //  while doing no check on manual loading. This may allow manual loading of older files at risk
  loadedVersion = 0;
  if(loadType == autoLoad && loadMode == LM_ALL) {
    readLine(BACKUP, tmpString);
    if(strcmp(tmpString, "SAVE_FILE_REVISION") == 0) {
      readLine(BACKUP, aimBuffer); // internal rev number (ignore now)
      readLine(BACKUP, aimBuffer); // param
      readLine(BACKUP, tmpString); // value
      if(strcmp(aimBuffer, "C43_save_file_00") == 0) {
        loadedVersion = stringToUint32(tmpString);
      }    
    }
  }

  if(loadType == manualLoad || (loadType == autoLoad && loadedVersion == configFileVersion)) {
    while(restoreOneSection(BACKUP, loadMode, s, n, d)) {
    }
  }


  lastErrorCode = ERROR_NONE;

  #if defined(DMCP_BUILD)
    f_close(BACKUP);
  #else // !DMCP_BUILD
    fclose(BACKUP);
  #endif //DMCP_BUILD

  #if !defined(TESTSUITE_BUILD)
    if(loadType == manualLoad && loadMode == LM_ALL) {
      temporaryInformation = TI_BACKUP_RESTORED;
    } else
    if(loadType == autoLoad && loadedVersion == configFileVersion && loadMode == LM_ALL) {
      temporaryInformation = TI_BACKUP_RESTORED;
    }
  #endif // !TESTSUITE_BUILD
}



void fnLoad(uint16_t loadMode) {
  doLoad(loadMode, 0, 0, 0, manualLoad);
}

void fnLoadAuto(void) {
  doLoad(LM_ALL, 0, 0, 0, autoLoad);
}


#undef BACKUP



void fnDeleteBackup(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnDeleteBackup);
  }
  else {
    #if defined(DMCP_BUILD)
      FRESULT result;
      sys_disk_write_enable(1);
      result = f_unlink("SAVFILES\\C43.sav");
      if(result != FR_OK && result != FR_NO_FILE && result != FR_NO_PATH) {
        displayCalcErrorMessage(ERROR_IO, ERR_REGISTER_LINE, REGISTER_X);
      }
      result = f_unlink("SAVFILES\\C43auto.sav");
      if(result != FR_OK && result != FR_NO_FILE && result != FR_NO_PATH) {
        displayCalcErrorMessage(ERROR_IO, ERR_REGISTER_LINE, REGISTER_X);
      }
      sys_disk_write_enable(0);
    #else // !DMCP_BUILD
      int result = remove("C43.sav");
      if(result == -1) {
        #if !defined(TESTSUITE_BUILD)
          int e = errno;
          if(e != ENOENT) {
            displayCalcErrorMessage(ERROR_IO, ERR_REGISTER_LINE, REGISTER_X);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              sprintf(errorMessage, "removing the backup failed with error code %d", e);
              moreInfoOnError("In function fnDeleteBackup:", errorMessage, NULL, NULL);
            #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
          }
        #endif // !TESTSUITE_BUILD
      }
    #endif // DMCP_BUILD
  }
}
