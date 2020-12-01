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
 * \file lblGtoXeq.c
 ***********************************************/

#include "wp43s.h"



void fnGoto(uint16_t label) {
  if(calcMode == CM_TAM || calcMode == CM_ASM_OVER_TAM) {
    if(dynamicMenuItem >= 0) {
      fnGotoDot(label);
      return;
    }

    // Local Label
    if(label < REGISTER_X || (label != REGISTER_L && REGISTER_A <= label && label <= REGISTER_J)) {
      switch(label) {
        case REGISTER_A: label = 100 - 'A' + 'A'; break;
        case REGISTER_B: label = 100 - 'A' + 'B'; break;
        case REGISTER_C: label = 100 - 'A' + 'C'; break;
        case REGISTER_D: label = 100 - 'A' + 'D'; break;
        case REGISTER_I: label = 100 - 'A' + 'I'; break;
        case REGISTER_J: label = 100 - 'A' + 'J'; break;
        default: {}
      }

      // Search for local label
      for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
        if(labelList[lbl].program - 1 == currentProgramNumber && labelList[lbl].followingStep < 0 && *(labelList[lbl].labelPointer) == label) { // Is in the current program and is a local label and is the searched label
          fnGotoDot(-labelList[lbl].followingStep - 1);
          return;
        }
      }

      #ifndef DMCP_BUILD
        if(label < REGISTER_X) {
          printf("Error in function fnGoto: there is no local label %02u in current program\n", label);
        }
        else {
          printf("Error in function fnGoto: there is no local label %c in current program\n", 'A' + (label - 100));
        }
      #endif // DMCP_BUILD
    }
    else { // Global label X, Y, Z, T, K, or L
      switch(label) {
        case REGISTER_X: label = 'X'; break;
        case REGISTER_Y: label = 'Y'; break;
        case REGISTER_Z: label = 'Z'; break;
        case REGISTER_T: label = 'T'; break;
        case REGISTER_K: label = 'K'; break;
        case REGISTER_L: label = 'L'; break;
        default: {}
      }

      for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
        if(labelList[lbl].followingStep > 0 && *(labelList[lbl].labelPointer) == 1 && *(labelList[lbl].labelPointer + 1) == label) { // Is a global label and is the searched label
          fnGotoDot(labelList[lbl].followingStep - 1);
          return;
        }
      }

      #ifndef DMCP_BUILD
        printf("Error in function fnGoto: there is no global label %c\n", label);
      #endif // DMCP_BUILD
    }
  }
  else {
    insertStepInProgram(ITM_GTO);
  }
}



void fnGotoDot(uint16_t stepNumber) {
  if(dynamicMenuItem >= 0) {
    if(dynamicMenuItem >= dynamicSoftmenu[softmenuStack[softmenuStackPointer].softmenuId].numItems) {
      return;
    }

    uint8_t *labelName = dynamicSoftmenu[softmenuStack[softmenuStackPointer].softmenuId].menuContent;
    while(dynamicMenuItem > 0) {
      labelName += stringByteLength((char *)labelName) + 1;
      dynamicMenuItem--;
    }

    if(*labelName == 0) {
      return;
    }

    int16_t c, len = stringByteLength((char *)labelName);
    for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
      if(labelList[lbl].followingStep > 0 && *labelList[lbl].labelPointer == len) { // It's a global label and the length is OK
        for(c=0; c<len; c++) {
          if(labelName[c] != labelList[lbl].labelPointer[c + 1]) {
            break;
          }
        }
        if(c == len) {
          stepNumber = labelList[lbl].followingStep - 1;
          break;
        }
      }
    }
  }

  currentStepNumber = stepNumber;

  defineCurrentProgramFromCurrentStepNumber();

  uint8_t *stepPointer = beginOfCurrentProgram;
  stepNumber = programList[currentProgramNumber].step - 1;
  while(true) {
    if(stepNumber == currentStepNumber) {
      currentStep = stepPointer;
      break;
    }

    stepPointer = findNextStep(stepPointer);
    stepNumber++;
  }

  if(currentStepNumber >= 3) {
    firstDisplayedStepNumber = currentStepNumber - 3;
    firstDisplayedStep = findPreviousStep(findPreviousStep(findPreviousStep(currentStep)));
  }
  else {
    firstDisplayedStepNumber = 0;
    firstDisplayedStep = beginOfProgramMemory;
  }
}
