  /* This file is part of WP 43S.
 *
 * WP 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */

/********************************************//**
 * \file xeqm.c
 ***********************************************/

#define commandnumberl NIM_BUFFER_LENGTH

#include "wp43s.h"

void capture_sequence(char *origin, uint16_t item) {
   char line1[TMP_STR_LENGTH];
   char ll[commandnumberl];
#ifdef PC_BUILD
   //printf("Captured: %4d   //%10s//  (%s)\n",item,indexOfItems[item].itemSoftmenuName, origin);
#endif

    line1[0]=0;
    ll[0]=0; ll[1]=0;
    switch (item) {
      case  ITM_XexY: strcpy(ll,"X<>Y"); break;
      case  ITM_YX:   strcpy(ll,"Y^X" ); break;
      case  ITM_DIV:  strcpy(ll,"/"   ); break;
      case  ITM_0:        ll[0]=48; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_1:        ll[0]=49; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_2:        ll[0]=50; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_3:        ll[0]=51; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_4:        ll[0]=52; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_5:        ll[0]=53; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_6:        ll[0]=54; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_7:        ll[0]=55; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_8:        ll[0]=56; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_9:        ll[0]=57; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_PERIOD:   ll[0]=46; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break; //.
      case  ITM_EXPONENT: ll[0]=69; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break; //E
      default: { strcpy(ll,indexOfItems[item].itemSoftmenuName);
               }  
    }

//    uint16_t ix;
//    ix = 0;                  //CONVERT OUTPUT ## for >128 characters
//    while (ll[ix] != 0) {
//    if (( (ll[ix] & 128) == 128) || ll[ix] < 32) {ll[ix] = 35;}
//    ix++;
//    }

    if(line1[0]==0) sprintf(line1, " %4d //%10s",item,ll);

    #ifndef TESTSUITE_BUILD
    stringToUtf8(line1, (uint8_t *)tmpStr3000);
    export_string_to_file(tmpStr3000);
    #endif
}


//############################ SEND KEY TO 43S ENGINE ####################
void runkey(int16_t item){
  #ifndef TESTSUITE_BUILD

  if(item < 0) {
    showSoftmenu(NULL, item, true);
  } else {

    //printf("§%d§ ",item);
    hideFunctionName();           //Clear in case active
    processKeyAction(item);
    if (!keyActionProcessed){
      hideFunctionName();         //Clear in case activated during process
      runFunction(item);
//      #ifdef DMCP_BUILD
//        lcd_forced_refresh(); // Just redraw from LCD buffer    
//      #endif
    } 
  }

  #endif
}

//############################ DECODE NUMBERS AND THEN SEND KEY TO 43S ENGINE ####################
void sendkeys(const char aa[]) {
  int16_t ix = 0;
  while (aa[ix]!=0) {
    if(aa[ix]>=65 && aa[ix]<= 90){runkey(ITM_A+aa[ix]-65);} else //A..Z
    if(aa[ix]>=97 && aa[ix]<=122){runkey(ITM_A+aa[ix]-65-32);} else //a..z converted to A..Z
    if(aa[ix]>=48 && aa[ix]<= 57){runkey(ITM_0+aa[ix]-48);} else //0..9
    switch (aa[ix]) {
      case 46:  runkey(ITM_PERIOD); break; //.
      case 69:  runkey(ITM_EXPONENT); break; //E
      case 101: runkey(ITM_EXPONENT); break; //e
      case 45:  runkey(ITM_SUB); break; //-
      case 43:  runkey(ITM_ADD); break; //+
      case 32:  runkey(ITM_SPACE); break; //space
      case 35:  runkey(ITM_toINT); break; //#
      default:;
    }
  ix++;
  }
}





bool_t strcompare( char *in1, char *in2) {
  if (stringByteLength(in1) == stringByteLength(in2)) {
    int16_t i = 0;
    bool_t areEqual = true;
    while (areEqual && in1[i] != 0) {
      if(in1[i] != in2[i]) {areEqual = false; return false;}
      i++;
    }
    return areEqual;
  } else return false;
}

// COMMAND name or number: command to be located in between any of CR, LF, comma, space, tab, i.e. X<>Y, PRIME?, ...
// COMMENT: comment to be located in between / and /, i.e. /Comment/
// NUMBER to be located in between quotes: "123.123" or STO "00"
// Example: "200" EXIT PRIME?
// Ignores all other ASCII control and white space.


bool_t running_program_jm = false;
uint16_t indic_x = 0;
uint16_t indic_y = SCREEN_HEIGHT-1;


void execute_string(const char *inputstring, bool_t exec1) {
#ifndef TESTSUITE_BUILD
      uint16_t ix, ix_m;
      uint16_t ix_m1 = 0;
      uint16_t ix_m2 = 0;
      uint16_t ix_m3 = 0;
      uint16_t ix_m4 = 0;
      int16_t no, ii;
      char     commandnumber[commandnumberl];
      char     aa[2], bb[2];
      bool_t   state_comments, state_commands, state_quotes;
      uint16_t xeqlblinprogress;
      uint16_t gotoinprogress;
      bool_t gotlabels = false;
      bool_t exec = false;
      bool_t go;
      running_program_jm = true;
      indic_x = 0;
      indic_y = SCREEN_HEIGHT-1;
      uint8_t starttoken = 0;
      #ifdef PC_BUILD
        uint16_t loopnumber = 0;
      #endif
                                                 //If !gotlabels, means it is a scouting pass/parse to find and mark the goto labels M1-M4
    while(!gotlabels || (gotlabels && exec) ){   //scheme to use for label scouting and name processing in "false", and to do a two parse exec 
      #ifdef PC_BUILD
        printf("------Starting parse ------- Indexes: M1:%d M2:%d M3:%d M4:%d   EXEC:%d\n",ix_m1, ix_m2, ix_m3, ix_m4, exec);
        printf("|%s|\n",inputstring);
      #endif
      xeqlblinprogress = 0;
      gotoinprogress   = 0;
      go = false;
      ix = 0;
      ix_m = 0;
      no = 0;
      state_comments = false;
      state_commands = false;
      state_quotes = false;
      commandnumber[0]=0;
      aa[0]=0;
#ifdef DMCP_BUILD
      while (inputstring[ix]!=0 && key_empty()) {
#else
      while (inputstring[ix]!=0) {
#endif
        strcpy(bb,aa);
        aa[0] = inputstring[ix];
        aa[1] = 0;

        switch(bb[0]) {//COMMAND can start after any of these: space, tab, cr, lf, comma, beginning of file
          case 32:
          case 8 :
          case 13:
          case 10:
          case 44:
          case 0 :  if( //COMMAND WORD START DETECTION +-*/ 0-9; A-Z; %; >; (; a-z; . 
                    (       aa[0]=='*' //42 // *
                        ||  aa[0]=='+' //43 // +
                        ||  aa[0]=='-' //45 // -
                        ||  aa[0]=='/' //47 // /
                        ||  aa[0]=='~' //126) //~
                        || (aa[0]>=48 && aa[0]<=57) //0-9
                        || (aa[0]>=65 && aa[0]<=90) //A-Z
                        || (aa[0]>=65+32 && aa[0]<=90+32) //a-z
                        || aa[0]=='%'
                        || aa[0]=='>'
                        || aa[0]=='('
                        || aa[0]=='.'
                        )
                    && !state_comments                 //If not inside comments
                    && !state_quotes                   //if not inside quotes
                    && !state_commands                 //Don't re-check until done
                    ) {
                        state_commands = true;         // Waiting to open command number or name: nnn
                      }
          default:;
        }
        
        if(state_comments && (aa[0] == 13 || aa[0] == 10)) {
          state_comments=false;
        } else
        switch(aa[0]) {
          case 47: if(bb[0] == 47 && state_comments == false) {//ADDED  STATE, SO //always switches on the comment, but not off. CR/LF cancels it
                      state_comments = true;           // Switch comment state on
                      state_commands = false;
                      state_quotes   = false;
                      commandnumber[0]=0;
                    }
                     break;
          case 34: if(!state_comments && !state_commands) {   // " Toggle quote state
                       state_quotes   = !state_quotes;
                     }
                     break;

          case 13: //cr
          case 10: //lf
          case 8 : //tab
          case 44: //,
          case 32: 
                   //printf("@@@ %s\n",commandnumber);
                   if(state_commands){
                      state_commands = false;                // Waiting for delimiter to close off and send command number: nnn<                 
                      //printf("Command/number detected:(tempjm=%d)(gotoinprogress=%d) %45s \n",temporaryInformation,gotoinprogress,commandnumber);
                      
                      //DSZ:
                      if(!(gotoinprogress != 11 || (gotoinprogress == 11 && (temporaryInformation == TI_FALSE)))) {     //If DEC results in 0, then 'true'.    It is now the command that may or may not be skipped
                          //......IS NOT DSZ.... OR               DSZ    with REG NOT ZERO
                          go = (temporaryInformation == TI_FALSE); //As per GTO_SZ ---- REGISTER<>0, then go
                          //printf("   DSZ/ISZ temporaryInformation = %5d\n",temporaryInformation);
                          gotoinprogress = 1;                      //As per GTO_SZ
                          commandnumber[0]=0;                      //As per GTO_SZ
                      } else

// FAST TRACKED FOR QUICKER RESPONSE DUPLICATED ************************************************************************
                      if (strcompare(commandnumber,"TICKS" )) {sprintf(commandnumber,"%d",ITM_TICKS);} else
                      if (strcompare(commandnumber,"PRIME?" )) {sprintf(commandnumber,"%d", ITM_PRIME);} else
                      if (strcompare(commandnumber,"ENTER" )) {sprintf(commandnumber,"%d", ITM_ENTER);} else
                      if (strcompare(commandnumber,"X<>Y" )) {sprintf(commandnumber,"%d", ITM_XexY);} else
                      if (strcompare(commandnumber,"DROP" )) {sprintf(commandnumber,"%d", ITM_DROP);} else
                      if (strcompare(commandnumber,"STO" )) {sprintf(commandnumber,"%d", ITM_STO);} else
                      if (strcompare(commandnumber,"RCL" )) {sprintf(commandnumber,"%d", ITM_RCL);} else
                      if (strcompare(commandnumber,"X^2" )) {sprintf(commandnumber,"%d", ITM_SQUARE);} else
                      if (strcompare(commandnumber,"Y^X" )) {sprintf(commandnumber,"%d", ITM_YX);} else
                      if (strcompare(commandnumber,"SQRT" )) {sprintf(commandnumber,"%d", ITM_SQUAREROOTX);} else
                      if (strcompare(commandnumber,"2^X" )) {sprintf(commandnumber,"%d", ITM_2X);} else
                      if (strcompare(commandnumber,"10^X" )) {sprintf(commandnumber,"%d", ITM_10x);} else
                      if (strcompare(commandnumber,"+" )) {sprintf(commandnumber,"%d", ITM_ADD);} else
                      if (strcompare(commandnumber,"-" )) {sprintf(commandnumber,"%d", ITM_SUB);} else
                      if (strcompare(commandnumber,"CHS" )) {sprintf(commandnumber,"%d", ITM_CHS);} else
                      if (strcompare(commandnumber,"*" )) {sprintf(commandnumber,"%d", ITM_MULT);} else
                      if (strcompare(commandnumber,"/" )) {sprintf(commandnumber,"%d", ITM_DIV);} else
                      if (strcompare(commandnumber,"EXIT" )) {sprintf(commandnumber,"%d", ITM_EXIT1);} else
                      if (strcompare(commandnumber,"SUM+" )) {sprintf(commandnumber,"%d", ITM_SIGMAPLUS);} else
                      if (strcompare(commandnumber,"NEXTP" )) {sprintf(commandnumber,"%d", ITM_NEXTP);} else
                      if (strcompare(commandnumber,"PI" )) {sprintf(commandnumber,"%d", ITM_CONSTpi);} else
                      if (strcompare(commandnumber,"RAN#" )) {sprintf(commandnumber,"%d", ITM_RAN);} else
                      if (strcompare(commandnumber,"e" )) {sprintf(commandnumber,"%d", CST_08);} else
                      if (strcompare(commandnumber,"PHI" )) {sprintf(commandnumber,"%d", CST_74);} else
                      if (strcompare(commandnumber,"ABS" )) {sprintf(commandnumber,"%d", ITM_MAGNITUDE);} else
                      if (strcompare(commandnumber,"ARG" )) {sprintf(commandnumber,"%d", ITM_ANGLE);} else
                      if (strcompare(commandnumber,"RE" )) {sprintf(commandnumber,"%d", ITM_RE);} else
                      if (strcompare(commandnumber,"IM" )) {sprintf(commandnumber,"%d", ITM_IM);} else

// FROM SPREADSHEET vvv ****************************************************************************************************


                      if (strcompare(commandnumber,"CPX?" )) {sprintf(commandnumber,"%d", ITM_CPX);} else
                      if (strcompare(commandnumber,"REAL?" )) {sprintf(commandnumber,"%d", ITM_REAL);} else
                      if (strcompare(commandnumber,"PRIME?" )) {sprintf(commandnumber,"%d", ITM_PRIME);} else
                      if (strcompare(commandnumber,"ENTER" )) {sprintf(commandnumber,"%d", ITM_ENTER);} else
                      if (strcompare(commandnumber,"X<>Y" )) {sprintf(commandnumber,"%d", ITM_XexY);} else
                      if (strcompare(commandnumber,"DROP" )) {sprintf(commandnumber,"%d", ITM_DROP);} else
                      if (strcompare(commandnumber,"DROPY" )) {sprintf(commandnumber,"%d", ITM_DROPY);} else
                      if (strcompare(commandnumber,"CLX" )) {sprintf(commandnumber,"%d", ITM_CLX);} else
                      if (strcompare(commandnumber,"FILL" )) {sprintf(commandnumber,"%d", ITM_FILL);} else
                      if (strcompare(commandnumber,"STO" )) {sprintf(commandnumber,"%d", ITM_STO);} else
                      if (strcompare(commandnumber,"RCL" )) {sprintf(commandnumber,"%d", ITM_RCL);} else
                      if (strcompare(commandnumber,"X^2" )) {sprintf(commandnumber,"%d", ITM_SQUARE);} else
                      if (strcompare(commandnumber,"X^3" )) {sprintf(commandnumber,"%d", ITM_CUBE);} else
                      if (strcompare(commandnumber,"Y^X" )) {sprintf(commandnumber,"%d", ITM_YX);} else
                      if (strcompare(commandnumber,"SQRT" )) {sprintf(commandnumber,"%d", ITM_SQUAREROOTX);} else
                      if (strcompare(commandnumber,"CUBRT" )) {sprintf(commandnumber,"%d", ITM_CUBEROOT);} else
                      if (strcompare(commandnumber,"XRTY" )) {sprintf(commandnumber,"%d", ITM_XTHROOT);} else
                      if (strcompare(commandnumber,"2^X" )) {sprintf(commandnumber,"%d", ITM_2X);} else
                      if (strcompare(commandnumber,"E^X" )) {sprintf(commandnumber,"%d", ITM_EXP);} else
                      if (strcompare(commandnumber,"E^X-1" )) {sprintf(commandnumber,"%d", ITM_EX1);} else
                      if (strcompare(commandnumber,"10^X" )) {sprintf(commandnumber,"%d", ITM_10x);} else
                      if (strcompare(commandnumber,"LOG2" )) {sprintf(commandnumber,"%d", ITM_LOG2);} else
                      if (strcompare(commandnumber,"LN" )) {sprintf(commandnumber,"%d", ITM_LN);} else
                      if (strcompare(commandnumber,"LN(1+X)" )) {sprintf(commandnumber,"%d", ITM_LN1X);} else
                      if (strcompare(commandnumber,"LOG10" )) {sprintf(commandnumber,"%d", ITM_LOG10);} else
                      if (strcompare(commandnumber,"LOGXY" )) {sprintf(commandnumber,"%d", ITM_LOGXY);} else
                      if (strcompare(commandnumber,"1/X" )) {sprintf(commandnumber,"%d", ITM_1ONX);} else
                      if (strcompare(commandnumber,"COS" )) {sprintf(commandnumber,"%d", ITM_cos);} else
                      if (strcompare(commandnumber,"COSH" )) {sprintf(commandnumber,"%d", ITM_cosh);} else
                      if (strcompare(commandnumber,"SIN" )) {sprintf(commandnumber,"%d", ITM_sin);} else
                      if (strcompare(commandnumber,"SINC" )) {sprintf(commandnumber,"%d", ITM_sinc);} else
                      if (strcompare(commandnumber,"SINH" )) {sprintf(commandnumber,"%d", ITM_sinh);} else
                      if (strcompare(commandnumber,"TAN" )) {sprintf(commandnumber,"%d", ITM_tan);} else
                      if (strcompare(commandnumber,"TANH" )) {sprintf(commandnumber,"%d", ITM_tanh);} else
                      if (strcompare(commandnumber,"ARCCOS" )) {sprintf(commandnumber,"%d", ITM_arccos);} else
                      if (strcompare(commandnumber,"ARCCOSH" )) {sprintf(commandnumber,"%d", ITM_arcosh);} else
                      if (strcompare(commandnumber,"ARCSIN" )) {sprintf(commandnumber,"%d", ITM_arcsin);} else
                      if (strcompare(commandnumber,"ARCSINH" )) {sprintf(commandnumber,"%d", ITM_arsinh);} else
                      if (strcompare(commandnumber,"ARCTAN" )) {sprintf(commandnumber,"%d", ITM_arctan);} else
                      if (strcompare(commandnumber,"ARCTANH" )) {sprintf(commandnumber,"%d", ITM_artanh);} else
                      if (strcompare(commandnumber,"CEIL" )) {sprintf(commandnumber,"%d", ITM_CEIL);} else
                      if (strcompare(commandnumber,"FLOOR" )) {sprintf(commandnumber,"%d", ITM_FLOOR);} else
                      if (strcompare(commandnumber,"GCD" )) {sprintf(commandnumber,"%d", ITM_GCD);} else
                      if (strcompare(commandnumber,"LCM" )) {sprintf(commandnumber,"%d", ITM_LCM);} else
                      if (strcompare(commandnumber,"DECR" )) {sprintf(commandnumber,"%d", ITM_DEC);} else
                      if (strcompare(commandnumber,"INCR" )) {sprintf(commandnumber,"%d", ITM_INC);} else
                      if (strcompare(commandnumber,"IP" )) {sprintf(commandnumber,"%d", ITM_IP);} else
                      if (strcompare(commandnumber,"FP" )) {sprintf(commandnumber,"%d", ITM_FP);} else
                      if (strcompare(commandnumber,"+" )) {sprintf(commandnumber,"%d", ITM_ADD);} else
                      if (strcompare(commandnumber,"-" )) {sprintf(commandnumber,"%d", ITM_SUB);} else
                      if (strcompare(commandnumber,"CHS" )) {sprintf(commandnumber,"%d", ITM_CHS);} else
                      if (strcompare(commandnumber,"*" )) {sprintf(commandnumber,"%d", ITM_MULT);} else
                      if (strcompare(commandnumber,"/" )) {sprintf(commandnumber,"%d", ITM_DIV);} else
                      if (strcompare(commandnumber,"IDIV" )) {sprintf(commandnumber,"%d", ITM_IDIV);} else
                      if (strcompare(commandnumber,"IDIVR" )) {sprintf(commandnumber,"%d", ITM_IDIVR);} else
                      if (strcompare(commandnumber,"MOD" )) {sprintf(commandnumber,"%d", ITM_MOD);} else
                      if (strcompare(commandnumber,"MAX" )) {sprintf(commandnumber,"%d", ITM_MAX);} else
                      if (strcompare(commandnumber,"MIN" )) {sprintf(commandnumber,"%d", ITM_MIN);} else
                      if (strcompare(commandnumber,"ABS" )) {sprintf(commandnumber,"%d", ITM_MAGNITUDE);} else
                      if (strcompare(commandnumber,"NEIGHB" )) {sprintf(commandnumber,"%d", ITM_NEIGHB);} else
                      if (strcompare(commandnumber,"NEXTP" )) {sprintf(commandnumber,"%d", ITM_NEXTP);} else
                      if (strcompare(commandnumber,"X!" )) {sprintf(commandnumber,"%d", ITM_XFACT);} else
                      if (strcompare(commandnumber,"PI" )) {sprintf(commandnumber,"%d", ITM_CONSTpi);} else
                      if (strcompare(commandnumber,"FF" )) {sprintf(commandnumber,"%d", ITM_FF);} else
                      if (strcompare(commandnumber,"SINCPI" )) {sprintf(commandnumber,"%d", ITM_sincpi);} else
                      if (strcompare(commandnumber,"c" )) {sprintf(commandnumber,"%d", CST_05);} else
                      if (strcompare(commandnumber,"e" )) {sprintf(commandnumber,"%d", CST_08);} else
                      if (strcompare(commandnumber,"ge" )) {sprintf(commandnumber,"%d", CST_16);} else
                      if (strcompare(commandnumber,"gEARTH" )) {sprintf(commandnumber,"%d", CST_18);} else
                      if (strcompare(commandnumber,"mu0" )) {sprintf(commandnumber,"%d", CST_65);} else
                      if (strcompare(commandnumber,"PHI" )) {sprintf(commandnumber,"%d", CST_74);} else
                      if (strcompare(commandnumber,"-INFINITY" )) {sprintf(commandnumber,"%d", CST_77);} else
                      if (strcompare(commandnumber,"INFINITY" )) {sprintf(commandnumber,"%d", CST_78);} else
                      if (strcompare(commandnumber,"NOT" )) {sprintf(commandnumber,"%d", ITM_LOGICALNOT);} else
                      if (strcompare(commandnumber,"AND" )) {sprintf(commandnumber,"%d", ITM_LOGICALAND);} else
                      if (strcompare(commandnumber,"OR" )) {sprintf(commandnumber,"%d", ITM_LOGICALOR);} else
                      if (strcompare(commandnumber,"XOR" )) {sprintf(commandnumber,"%d", ITM_LOGICALXOR);} else
                      if (strcompare(commandnumber,"NAND" )) {sprintf(commandnumber,"%d", ITM_LOGICALNAND);} else
                      if (strcompare(commandnumber,"NOR" )) {sprintf(commandnumber,"%d", ITM_LOGICALNOR);} else
                      if (strcompare(commandnumber,"XNOR" )) {sprintf(commandnumber,"%d", ITM_LOGICALXNOR);} else
                      if (strcompare(commandnumber,"BS?" )) {sprintf(commandnumber,"%d", ITM_BS);} else
                      if (strcompare(commandnumber,"BC?" )) {sprintf(commandnumber,"%d", ITM_BC);} else
                      if (strcompare(commandnumber,"CB" )) {sprintf(commandnumber,"%d", ITM_CB);} else
                      if (strcompare(commandnumber,"SB" )) {sprintf(commandnumber,"%d", ITM_SB);} else
                      if (strcompare(commandnumber,"FB" )) {sprintf(commandnumber,"%d", ITM_FB);} else
                      if (strcompare(commandnumber,"RL" )) {sprintf(commandnumber,"%d", ITM_RL);} else
                      if (strcompare(commandnumber,"RLC" )) {sprintf(commandnumber,"%d", ITM_RLC);} else
                      if (strcompare(commandnumber,"RR" )) {sprintf(commandnumber,"%d", ITM_RR);} else
                      if (strcompare(commandnumber,"RRC" )) {sprintf(commandnumber,"%d", ITM_RRC);} else
                      if (strcompare(commandnumber,"SL" )) {sprintf(commandnumber,"%d", ITM_SL);} else
                      if (strcompare(commandnumber,"SR" )) {sprintf(commandnumber,"%d", ITM_SR);} else
                      if (strcompare(commandnumber,"ASR" )) {sprintf(commandnumber,"%d", ITM_ASR);} else
                      if (strcompare(commandnumber,"LJ" )) {sprintf(commandnumber,"%d", ITM_LJ);} else
                      if (strcompare(commandnumber,"RJ" )) {sprintf(commandnumber,"%d", ITM_RJ);} else
                      if (strcompare(commandnumber,"MASKL" )) {sprintf(commandnumber,"%d", ITM_MASKL);} else
                      if (strcompare(commandnumber,"MASKR" )) {sprintf(commandnumber,"%d", ITM_MASKR);} else
                      if (strcompare(commandnumber,"MIRROR" )) {sprintf(commandnumber,"%d", ITM_MIRROR);} else
                      if (strcompare(commandnumber,"#B" )) {sprintf(commandnumber,"%d", ITM_NUMB);} else
                      if (strcompare(commandnumber,"SDL" )) {sprintf(commandnumber,"%d", ITM_SDL);} else
                      if (strcompare(commandnumber,"SDR" )) {sprintf(commandnumber,"%d", ITM_SDR);} else
                      if (strcompare(commandnumber,"SUM+" )) {sprintf(commandnumber,"%d", ITM_SIGMAPLUS);} else
                      if (strcompare(commandnumber,"NSUM" )) {sprintf(commandnumber,"%d", ITM_NSIGMA);} else
                      if (strcompare(commandnumber,"SUMX" )) {sprintf(commandnumber,"%d", ITM_SIGMAx);} else
                      if (strcompare(commandnumber,"SUMY" )) {sprintf(commandnumber,"%d", ITM_SIGMAy);} else
                      if (strcompare(commandnumber,"SUMX^2" )) {sprintf(commandnumber,"%d", ITM_SIGMAx2);} else
                      if (strcompare(commandnumber,"SUMX^2Y" )) {sprintf(commandnumber,"%d", ITM_SIGMAx2y);} else
                      if (strcompare(commandnumber,"SUMY^2" )) {sprintf(commandnumber,"%d", ITM_SIGMAy2);} else
                      if (strcompare(commandnumber,"SUMXY" )) {sprintf(commandnumber,"%d", ITM_SIGMAxy);} else
                      if (strcompare(commandnumber,"SUMLNXY" )) {sprintf(commandnumber,"%d", ITM_SIGMAlnxy);} else
                      if (strcompare(commandnumber,"SUMLNX" )) {sprintf(commandnumber,"%d", ITM_SIGMAlnx);} else
                      if (strcompare(commandnumber,"SUMLN^2X" )) {sprintf(commandnumber,"%d", ITM_SIGMAln2x);} else
                      if (strcompare(commandnumber,"SUMYLNX" )) {sprintf(commandnumber,"%d", ITM_SIGMAylnx);} else
                      if (strcompare(commandnumber,"SUMLNY" )) {sprintf(commandnumber,"%d", ITM_SIGMAlny);} else
                      if (strcompare(commandnumber,"SUMLN^2Y" )) {sprintf(commandnumber,"%d", ITM_SIGMAln2y);} else
                      if (strcompare(commandnumber,"SUMXLNY" )) {sprintf(commandnumber,"%d", ITM_SIGMAxlny);} else
                      if (strcompare(commandnumber,"SUMLNY/X" )) {sprintf(commandnumber,"%d", ITM_SIGMAlnyonx);} else
                      if (strcompare(commandnumber,"SUMX^2/Y" )) {sprintf(commandnumber,"%d", ITM_SIGMAx2ony);} else
                      if (strcompare(commandnumber,"SUM^1/X" )) {sprintf(commandnumber,"%d", ITM_SIGMA1onx);} else
                      if (strcompare(commandnumber,"SUM^1/X^2" )) {sprintf(commandnumber,"%d", ITM_SIGMA1onx2);} else
                      if (strcompare(commandnumber,"SUMX/Y" )) {sprintf(commandnumber,"%d", ITM_SIGMAxony);} else
                      if (strcompare(commandnumber,"SUM^1/Y" )) {sprintf(commandnumber,"%d", ITM_SIGMA1ony);} else
                      if (strcompare(commandnumber,"SUM^1/Y^2" )) {sprintf(commandnumber,"%d", ITM_SIGMA1ony2);} else
                      if (strcompare(commandnumber,"SUMX^3" )) {sprintf(commandnumber,"%d", ITM_SIGMAx3);} else
                      if (strcompare(commandnumber,"SUMX^4" )) {sprintf(commandnumber,"%d", ITM_SIGMAx4);} else
                      if (strcompare(commandnumber,"FRACT" )) {sprintf(commandnumber,"%d", SFL_FRACT);} else
                      if (strcompare(commandnumber,"PROPFR" )) {sprintf(commandnumber,"%d", SFL_PROPFR);} else
                      if (strcompare(commandnumber,"DENANY" )) {sprintf(commandnumber,"%d", SFL_DENANY);} else
                      if (strcompare(commandnumber,"DENFIX" )) {sprintf(commandnumber,"%d", SFL_DENFIX);} else
                      if (strcompare(commandnumber,"ST.A" )) {sprintf(commandnumber,"%d", ITM_ST_A);} else
                      if (strcompare(commandnumber,"ST.B" )) {sprintf(commandnumber,"%d", ITM_ST_B);} else
                      if (strcompare(commandnumber,"ST.C" )) {sprintf(commandnumber,"%d", ITM_ST_C);} else
                      if (strcompare(commandnumber,"ST.D" )) {sprintf(commandnumber,"%d", ITM_ST_D);} else
                      if (strcompare(commandnumber,"ST.T" )) {sprintf(commandnumber,"%d", ITM_ST_T);} else
                      if (strcompare(commandnumber,"ST.X" )) {sprintf(commandnumber,"%d", ITM_ST_X);} else
                      if (strcompare(commandnumber,"ST.Y" )) {sprintf(commandnumber,"%d", ITM_ST_Y);} else
                      if (strcompare(commandnumber,"ST.Z" )) {sprintf(commandnumber,"%d", ITM_ST_Z);} else
                      if (strcompare(commandnumber,"IND>" )) {sprintf(commandnumber,"%d", ITM_INDIRECTION);} else
                      if (strcompare(commandnumber,"REG_A" )) {sprintf(commandnumber,"%d", ITM_REG_A);} else
                      if (strcompare(commandnumber,"REG_B" )) {sprintf(commandnumber,"%d", ITM_REG_B);} else
                      if (strcompare(commandnumber,"REG_C" )) {sprintf(commandnumber,"%d", ITM_REG_C);} else
                      if (strcompare(commandnumber,"REG_D" )) {sprintf(commandnumber,"%d", ITM_REG_D);} else
                      if (strcompare(commandnumber,"REG_L" )) {sprintf(commandnumber,"%d", ITM_REG_L);} else
                      if (strcompare(commandnumber,"REG_I" )) {sprintf(commandnumber,"%d", ITM_REG_I);} else
                      if (strcompare(commandnumber,"REG_J" )) {sprintf(commandnumber,"%d", ITM_REG_J);} else
                      if (strcompare(commandnumber,"REG_K" )) {sprintf(commandnumber,"%d", ITM_REG_K);} else
                      if (strcompare(commandnumber,"E" )) {sprintf(commandnumber,"%d", ITM_EXPONENT);} else
                      if (strcompare(commandnumber,"1COMPL" )) {sprintf(commandnumber,"%d", ITM_1COMPL);} else
                      if (strcompare(commandnumber,"SNAP" )) {sprintf(commandnumber,"%d", ITM_SCRDMP);} else
                      if (strcompare(commandnumber,"2COMPL" )) {sprintf(commandnumber,"%d", ITM_2COMPL);} else
                      if (strcompare(commandnumber,"AGM" )) {sprintf(commandnumber,"%d", ITM_AGM);} else
                      if (strcompare(commandnumber,"BATT?" )) {sprintf(commandnumber,"%d", ITM_BATT);} else
                      if (strcompare(commandnumber,"CLFALL" )) {sprintf(commandnumber,"%d", ITM_CLFALL);} else
                      if (strcompare(commandnumber,"CLLCD" )) {sprintf(commandnumber,"%d", ITM_CLLCD);} else
                      if (strcompare(commandnumber,"CLREGS" )) {sprintf(commandnumber,"%d", ITM_CLREGS);} else
                      if (strcompare(commandnumber,"CLSTK" )) {sprintf(commandnumber,"%d", ITM_CLSTK);} else
                      if (strcompare(commandnumber,"CLSUM" )) {sprintf(commandnumber,"%d", ITM_CLSIGMA);} else
                      if (strcompare(commandnumber,"COMB" )) {sprintf(commandnumber,"%d", ITM_COMB);} else
                      if (strcompare(commandnumber,"CONJ" )) {sprintf(commandnumber,"%d", ITM_CONJ);} else
                      if (strcompare(commandnumber,"CROSS" )) {sprintf(commandnumber,"%d", ITM_CROSS_PROD);} else
                      if (strcompare(commandnumber,"CX>RE" )) {sprintf(commandnumber,"%d", ITM_CXtoRE);} else
                      if (strcompare(commandnumber,"DECOMP" )) {sprintf(commandnumber,"%d", ITM_DECOMP);} else
                      if (strcompare(commandnumber,"DEG" )) {sprintf(commandnumber,"%d", ITM_DEG);} else
                      if (strcompare(commandnumber,"DEG>" )) {sprintf(commandnumber,"%d", ITM_DEGto);} else
                      if (strcompare(commandnumber,"DENMAX" )) {sprintf(commandnumber,"%d", ITM_DENMAX);} else
                      if (strcompare(commandnumber,"DOT" )) {sprintf(commandnumber,"%d", ITM_DOT_PROD);} else
                      if (strcompare(commandnumber,"D.MS" )) {sprintf(commandnumber,"%d", ITM_DMS);} else
                      if (strcompare(commandnumber,"D.MS>" )) {sprintf(commandnumber,"%d", ITM_DMSto);} else
                      if (strcompare(commandnumber,"D>R" )) {sprintf(commandnumber,"%d", ITM_DtoR);} else
                      if (strcompare(commandnumber,"ENG" )) {sprintf(commandnumber,"%d", ITM_ENG);} else
                      if (strcompare(commandnumber,"ENTRY?" )) {sprintf(commandnumber,"%d", ITM_ENTRY);} else
                      if (strcompare(commandnumber,"EXPT" )) {sprintf(commandnumber,"%d", ITM_EXPT);} else
                      if (strcompare(commandnumber,"FIB" )) {sprintf(commandnumber,"%d", ITM_FIB);} else
                      if (strcompare(commandnumber,"FIX" )) {sprintf(commandnumber,"%d", ITM_FIX);} else
                      if (strcompare(commandnumber,"FLASH?" )) {sprintf(commandnumber,"%d", ITM_FLASH);} else
                      if (strcompare(commandnumber,"GD" )) {sprintf(commandnumber,"%d", ITM_GD);} else
                      if (strcompare(commandnumber,"GD^MINUS_1" )) {sprintf(commandnumber,"%d", ITM_GDM1);} else
                      if (strcompare(commandnumber,"GRAD" )) {sprintf(commandnumber,"%d", ITM_GRAD);} else
                      if (strcompare(commandnumber,"GRAD>" )) {sprintf(commandnumber,"%d", ITM_GRADto);} else
                      if (strcompare(commandnumber,"IM" )) {sprintf(commandnumber,"%d", ITM_IM);} else
                      if (strcompare(commandnumber,"KEY?" )) {sprintf(commandnumber,"%d", ITM_KEYQ);} else
                      if (strcompare(commandnumber,"LASTX" )) {sprintf(commandnumber,"%d", ITM_LASTX);} else
                      if (strcompare(commandnumber,"LNBETA" )) {sprintf(commandnumber,"%d", ITM_LNBETA);} else
                      if (strcompare(commandnumber,"LNGAMMA" )) {sprintf(commandnumber,"%d", ITM_LNGAMMA);} else
                      if (strcompare(commandnumber,"LOCR?" )) {sprintf(commandnumber,"%d", ITM_LocRQ);} else
                      if (strcompare(commandnumber,"MANT" )) {sprintf(commandnumber,"%d", ITM_MANT);} else
                      if (strcompare(commandnumber,"MEM?" )) {sprintf(commandnumber,"%d", ITM_MEM);} else
                      if (strcompare(commandnumber,"MULPI" )) {sprintf(commandnumber,"%d", ITM_MULPI);} else
                      if (strcompare(commandnumber,"PERM" )) {sprintf(commandnumber,"%d", ITM_PERM);} else
                      if (strcompare(commandnumber,"PLOT" )) {sprintf(commandnumber,"%d", ITM_PLOT);} else
                      if (strcompare(commandnumber,"RAD" )) {sprintf(commandnumber,"%d", ITM_RAD);} else
                      if (strcompare(commandnumber,"RAD>" )) {sprintf(commandnumber,"%d", ITM_RADto);} else
                      if (strcompare(commandnumber,"RAN#" )) {sprintf(commandnumber,"%d", ITM_RAN);} else
                      if (strcompare(commandnumber,"RCLEL" )) {sprintf(commandnumber,"%d", ITM_RCLEL);} else
                      if (strcompare(commandnumber,"RCLIJ" )) {sprintf(commandnumber,"%d", ITM_RCLIJ);} else
                      if (strcompare(commandnumber,"RCLS" )) {sprintf(commandnumber,"%d", ITM_RCLS);} else
                      if (strcompare(commandnumber,"RE" )) {sprintf(commandnumber,"%d", ITM_RE);} else
                      if (strcompare(commandnumber,"RE>CX" )) {sprintf(commandnumber,"%d", ITM_REtoCX);} else
                      if (strcompare(commandnumber,"RE<>IM" )) {sprintf(commandnumber,"%d", ITM_REexIM);} else
                      if (strcompare(commandnumber,"RMODE?" )) {sprintf(commandnumber,"%d", ITM_RMQ);} else
                      if (strcompare(commandnumber,"RMD" )) {sprintf(commandnumber,"%d", ITM_RMD);} else
                      if (strcompare(commandnumber,"R>D" )) {sprintf(commandnumber,"%d", ITM_RtoD);} else
                      if (strcompare(commandnumber,"SCI" )) {sprintf(commandnumber,"%d", ITM_SCI);} else
                      if (strcompare(commandnumber,"SDIGS?" )) {sprintf(commandnumber,"%d", ITM_SDIGS);} else
                      if (strcompare(commandnumber,"SEED" )) {sprintf(commandnumber,"%d", ITM_SEED);} else
                      if (strcompare(commandnumber,"SIGN" )) {sprintf(commandnumber,"%d", ITM_SIGN);} else
                      if (strcompare(commandnumber,"SIGNMT" )) {sprintf(commandnumber,"%d", ITM_SIGNMT);} else
                      if (strcompare(commandnumber,"SLVQ" )) {sprintf(commandnumber,"%d", ITM_SLVQ);} else
                      if (strcompare(commandnumber,"ISM?" )) {sprintf(commandnumber,"%d", ITM_ISM);} else
                      if (strcompare(commandnumber,"SSIZE?" )) {sprintf(commandnumber,"%d", ITM_SSIZE);} else
                      if (strcompare(commandnumber,"STOEL" )) {sprintf(commandnumber,"%d", ITM_STOEL);} else
                      if (strcompare(commandnumber,"STOIJ" )) {sprintf(commandnumber,"%d", ITM_STOIJ);} else
                      if (strcompare(commandnumber,"STOS" )) {sprintf(commandnumber,"%d", ITM_STOS);} else
                      if (strcompare(commandnumber,"SUM" )) {sprintf(commandnumber,"%d", ITM_SUM);} else
                      if (strcompare(commandnumber,"TICKS" )) {sprintf(commandnumber,"%d", ITM_TICKS);} else
                      if (strcompare(commandnumber,"T<>" )) {sprintf(commandnumber,"%d", ITM_Tex);} else
                      if (strcompare(commandnumber,"ULP?" )) {sprintf(commandnumber,"%d", ITM_ULP);} else
                      if (strcompare(commandnumber,"UNITV" )) {sprintf(commandnumber,"%d", ITM_UNITV);} else
                      if (strcompare(commandnumber,"UNSIGN" )) {sprintf(commandnumber,"%d", ITM_UNSIGN);} else
                      if (strcompare(commandnumber,"WSIZE" )) {sprintf(commandnumber,"%d", ITM_WSIZE);} else
                      if (strcompare(commandnumber,"WSIZE?" )) {sprintf(commandnumber,"%d", ITM_WSIZEQ);} else
                      if (strcompare(commandnumber,"X_MEAN" )) {sprintf(commandnumber,"%d", ITM_XBAR);} else
                      if (strcompare(commandnumber,"X_GEO" )) {sprintf(commandnumber,"%d", ITM_XG);} else
                      if (strcompare(commandnumber,"X_WEIGHTD" )) {sprintf(commandnumber,"%d", ITM_XW);} else
                      if (strcompare(commandnumber,"X>ALPHA" )) {sprintf(commandnumber,"%d", ITM_XtoALPHA);} else
                      if (strcompare(commandnumber,"X<>" )) {sprintf(commandnumber,"%d", ITM_Xex);} else
                      if (strcompare(commandnumber,"Y<>" )) {sprintf(commandnumber,"%d", ITM_Yex);} else
                      if (strcompare(commandnumber,"Z<>" )) {sprintf(commandnumber,"%d", ITM_Zex);} else
                      if (strcompare(commandnumber,"XMAX" )) {sprintf(commandnumber,"%d", ITM_XMAX);} else
                      if (strcompare(commandnumber,"XMIN" )) {sprintf(commandnumber,"%d", ITM_XMIN);} else
                      if (strcompare(commandnumber,"BETA(X,Y)" )) {sprintf(commandnumber,"%d", ITM_BETAXY);} else
                      if (strcompare(commandnumber,"GAMMA(X)" )) {sprintf(commandnumber,"%d", ITM_GAMMAX);} else
                      if (strcompare(commandnumber,"DELTA%" )) {sprintf(commandnumber,"%d", ITM_DELTAPC);} else
                      if (strcompare(commandnumber,"RANI#" )) {sprintf(commandnumber,"%d", ITM_RANI);} else
                      if (strcompare(commandnumber,"RANGE" )) {sprintf(commandnumber,"%d", ITM_RANGE);} else
                      if (strcompare(commandnumber,"RANGE?" )) {sprintf(commandnumber,"%d", ITM_GETRANGE);} else
                      if (strcompare(commandnumber,"(-1)^X" )) {sprintf(commandnumber,"%d", ITM_M1X);} else
                      if (strcompare(commandnumber,">DEG" )) {sprintf(commandnumber,"%d", ITM_toDEG);} else
                      if (strcompare(commandnumber,">D.MS" )) {sprintf(commandnumber,"%d", ITM_toDMS);} else
                      if (strcompare(commandnumber,">GRAD" )) {sprintf(commandnumber,"%d", ITM_toGRAD);} else
                      if (strcompare(commandnumber,">HR" )) {sprintf(commandnumber,"%d", ITM_toHR);} else
                      if (strcompare(commandnumber,">H.MS" )) {sprintf(commandnumber,"%d", ITM_toHMS);} else
                      if (strcompare(commandnumber,">INT" )) {sprintf(commandnumber,"%d", ITM_toINT);} else
                      if (strcompare(commandnumber,">MULPI" )) {sprintf(commandnumber,"%d", ITM_toMULpi);} else
                      if (strcompare(commandnumber,">RAD" )) {sprintf(commandnumber,"%d", ITM_toRAD);} else
                      if (strcompare(commandnumber,">REAL" )) {sprintf(commandnumber,"%d", ITM_toREAL);} else
                      if (strcompare(commandnumber,"D>D.MS" )) {sprintf(commandnumber,"%d", ITM_DtoDMS);} else
                      if (strcompare(commandnumber,"<>" )) {sprintf(commandnumber,"%d", ITM_SHUFFLE);} else
                      if (strcompare(commandnumber,"%" )) {sprintf(commandnumber,"%d", ITM_PC);} else
                      if (strcompare(commandnumber,"%MRR" )) {sprintf(commandnumber,"%d", ITM_PCMRR);} else
                      if (strcompare(commandnumber,"%T" )) {sprintf(commandnumber,"%d", ITM_PCT);} else
                      if (strcompare(commandnumber,"%SUM" )) {sprintf(commandnumber,"%d", ITM_PCSIGMA);} else
                      if (strcompare(commandnumber,"%+MG" )) {sprintf(commandnumber,"%d", ITM_PCPMG);} else
                      if (strcompare(commandnumber,"PARL" )) {sprintf(commandnumber,"%d", ITM_PARALLEL);} else
                      if (strcompare(commandnumber,"ARG" )) {sprintf(commandnumber,"%d", ITM_ANGLE);} else
                      if (strcompare(commandnumber,"MULPI>" )) {sprintf(commandnumber,"%d", ITM_MULPIto);} else
                      if (strcompare(commandnumber,"CC" )) {sprintf(commandnumber,"%d", ITM_CC);} else
                      if (strcompare(commandnumber,"EXIT" )) {sprintf(commandnumber,"%d", ITM_EXIT1);} else
                      if (strcompare(commandnumber,"ALPHA" )) {sprintf(commandnumber,"%d", ITM_AIM);} else
                      if (strcompare(commandnumber,"DOTD" )) {sprintf(commandnumber,"%d", ITM_dotD);} else
                      if (strcompare(commandnumber,"D.MS>D" )) {sprintf(commandnumber,"%d", ITM_DMStoD);} else
                      if (strcompare(commandnumber,"X_HARM" )) {sprintf(commandnumber,"%d", ITM_XH);} else
                      if (strcompare(commandnumber,"X_RMS" )) {sprintf(commandnumber,"%d", ITM_XRMS);} else
                      if (strcompare(commandnumber,"DET" )) {sprintf(commandnumber,"%d", ITM_DET);} else
                      if (strcompare(commandnumber,"INVRT" )) {sprintf(commandnumber,"%d", ITM_INVRT);} else
                      if (strcompare(commandnumber,"TRANS" )) {sprintf(commandnumber,"%d", ITM_TRANS);} else
                      if (strcompare(commandnumber,"COMPLEX" )) {sprintf(commandnumber,"%d", KEY_COMPLEX);} else
                      if (strcompare(commandnumber,">POLAR" )) {sprintf(commandnumber,"%d", ITM_toPOL2);} else
                      if (strcompare(commandnumber,">RECT" )) {sprintf(commandnumber,"%d", ITM_toREC2);} else
                      if (strcompare(commandnumber,"ERPN" )) {sprintf(commandnumber,"%d", ITM_eRPN_ON);} else
                      if (strcompare(commandnumber,"RPN" )) {sprintf(commandnumber,"%d", ITM_eRPN_OFF);} else
                      if (strcompare(commandnumber,"TEST_45" )) {sprintf(commandnumber,"%d", ITM_PGMTST);} else
                      if (strcompare(commandnumber,"SIG" )) {sprintf(commandnumber,"%d", ITM_SIGFIG);} else
                      if (strcompare(commandnumber,"UNIT" )) {sprintf(commandnumber,"%d", ITM_UNIT);} else
                      if (strcompare(commandnumber,"ROUND" )) {sprintf(commandnumber,"%d", ITM_ROUND2);} else
                      if (strcompare(commandnumber,"ROUNDI" )) {sprintf(commandnumber,"%d", ITM_ROUNDI2);} else
                      if (strcompare(commandnumber,"OP_A" )) {sprintf(commandnumber,"%d", ITM_op_a);} else
                      if (strcompare(commandnumber,"OP_A^2" )) {sprintf(commandnumber,"%d", ITM_op_a2);} else
                      if (strcompare(commandnumber,"OP_J" )) {sprintf(commandnumber,"%d", ITM_op_j);} else
                      if (strcompare(commandnumber,"D>Y" )) {sprintf(commandnumber,"%d", ITM_EE_D2Y);} else
                      if (strcompare(commandnumber,"Y>D" )) {sprintf(commandnumber,"%d", ITM_EE_Y2D);} else
                      if (strcompare(commandnumber,"ATOSYM" )) {sprintf(commandnumber,"%d", ITM_EE_A2S);} else
                      if (strcompare(commandnumber,"SYMTOA" )) {sprintf(commandnumber,"%d", ITM_EE_S2A);} else
                      if (strcompare(commandnumber,"E^THETAJ" )) {sprintf(commandnumber,"%d", ITM_EE_EXP_TH);} else
                      if (strcompare(commandnumber,"STO3Z" )) {sprintf(commandnumber,"%d", ITM_EE_STO_Z);} else
                      if (strcompare(commandnumber,"RCL3Z" )) {sprintf(commandnumber,"%d", ITM_EE_RCL_Z);} else
                      if (strcompare(commandnumber,"STO3V" )) {sprintf(commandnumber,"%d", ITM_EE_STO_V);} else
                      if (strcompare(commandnumber,"RCL3V" )) {sprintf(commandnumber,"%d", ITM_EE_RCL_V);} else
                      if (strcompare(commandnumber,"STO3I" )) {sprintf(commandnumber,"%d", ITM_EE_STO_I);} else
                      if (strcompare(commandnumber,"RCL3I" )) {sprintf(commandnumber,"%d", ITM_EE_RCL_I);} else
                      if (strcompare(commandnumber,"3V/3I" )) {sprintf(commandnumber,"%d", ITM_EE_STO_V_I);} else
                      if (strcompare(commandnumber,"3Ix3Z" )) {sprintf(commandnumber,"%d", ITM_EE_STO_IR);} else
                      if (strcompare(commandnumber,"3V/3Z" )) {sprintf(commandnumber,"%d", ITM_EE_STO_V_Z);} else
                      if (strcompare(commandnumber,"X>BAL" )) {sprintf(commandnumber,"%d", ITM_EE_X2BAL);} else
                      if (strcompare(commandnumber,"LNGINT" )) {sprintf(commandnumber,"%d", ITM_LI);} else
                      if (strcompare(commandnumber,">BIN" )) {sprintf(commandnumber,"%d", ITM_2BIN);} else
                      if (strcompare(commandnumber,">OCT" )) {sprintf(commandnumber,"%d", ITM_2OCT);} else
                      if (strcompare(commandnumber,">DEC" )) {sprintf(commandnumber,"%d", ITM_2DEC);} else
                      if (strcompare(commandnumber,">HEX" )) {sprintf(commandnumber,"%d", ITM_2HEX);} else
                      if (strcompare(commandnumber,">I" )) {sprintf(commandnumber,"%d", ITM_RI);} else
                      if (strcompare(commandnumber,"CASEUP" )) {sprintf(commandnumber,"%d", CHR_caseUP);} else
                      if (strcompare(commandnumber,"CASEDN" )) {sprintf(commandnumber,"%d", CHR_caseDN);} else
                      if (strcompare(commandnumber,"LISTXY" )) {sprintf(commandnumber,"%d", ITM_LISTXY);} else
                      if (strcompare(commandnumber,"ERPN?" )) {sprintf(commandnumber,"%d", ITM_SH_ERPN);} else
                      if (strcompare(commandnumber,"X.XEQ" )) {sprintf(commandnumber,"%d", ITM_XXEQ);} else
                      if (strcompare(commandnumber,"CPXI" )) {sprintf(commandnumber,"%d", ITM_CPXI);} else
                      if (strcompare(commandnumber,"CPXJ" )) {sprintf(commandnumber,"%d", ITM_CPXJ);} else
                      if (strcompare(commandnumber,"SSIZE4" )) {sprintf(commandnumber,"%d", ITM_SSIZE4);} else
                      if (strcompare(commandnumber,"SSIZE8" )) {sprintf(commandnumber,"%d", ITM_SSIZE8);} else
                      if (strcompare(commandnumber,".MS" )) {sprintf(commandnumber,"%d", ITM_ms);} else
                      if (strcompare(commandnumber,">>DEG" )) {sprintf(commandnumber,"%d", ITM_DEG2);} else
                      if (strcompare(commandnumber,">>D.MS" )) {sprintf(commandnumber,"%d", ITM_DMS2);} else
                      if (strcompare(commandnumber,">>GRAD" )) {sprintf(commandnumber,"%d", ITM_GRAD2);} else
                      if (strcompare(commandnumber,">>MULPI" )) {sprintf(commandnumber,"%d", ITM_MULPI2);} else
                      if (strcompare(commandnumber,">>RAD" )) {sprintf(commandnumber,"%d", ITM_RAD2);} else
                      if (strcompare(commandnumber,">>H.MS" )) {sprintf(commandnumber,"%d", ITM_HMS2);} else
                      if (strcompare(commandnumber,"XEQM01" ) && exec) {sprintf(commandnumber,"%d", ITM_X_P1);} else
                      if (strcompare(commandnumber,"XEQM02" ) && exec) {sprintf(commandnumber,"%d", ITM_X_P2);} else
                      if (strcompare(commandnumber,"XEQM03" ) && exec) {sprintf(commandnumber,"%d", ITM_X_P3);} else
                      if (strcompare(commandnumber,"XEQM04" ) && exec) {sprintf(commandnumber,"%d", ITM_X_P4);} else
                      if (strcompare(commandnumber,"XEQM05" ) && exec) {sprintf(commandnumber,"%d", ITM_X_P5);} else
                      if (strcompare(commandnumber,"XEQM06" ) && exec) {sprintf(commandnumber,"%d", ITM_X_P6);} else
                      if (strcompare(commandnumber,"XEQM07" ) && exec) {sprintf(commandnumber,"%d", ITM_X_f1);} else
                      if (strcompare(commandnumber,"XEQM08" ) && exec) {sprintf(commandnumber,"%d", ITM_X_f2);} else
                      if (strcompare(commandnumber,"XEQM09" ) && exec) {sprintf(commandnumber,"%d", ITM_X_f3);} else
                      if (strcompare(commandnumber,"XEQM10" ) && exec) {sprintf(commandnumber,"%d", ITM_X_f4);} else
                      if (strcompare(commandnumber,"XEQM11" ) && exec) {sprintf(commandnumber,"%d", ITM_X_f5);} else
                      if (strcompare(commandnumber,"XEQM12" ) && exec) {sprintf(commandnumber,"%d", ITM_X_f6);} else
                      if (strcompare(commandnumber,"XEQM13" ) && exec) {sprintf(commandnumber,"%d", ITM_X_g1);} else
                      if (strcompare(commandnumber,"XEQM14" ) && exec) {sprintf(commandnumber,"%d", ITM_X_g2);} else
                      if (strcompare(commandnumber,"XEQM15" ) && exec) {sprintf(commandnumber,"%d", ITM_X_g3);} else
                      if (strcompare(commandnumber,"XEQM16" ) && exec) {sprintf(commandnumber,"%d", ITM_X_g4);} else
                      if (strcompare(commandnumber,"XEQM17" ) && exec) {sprintf(commandnumber,"%d", ITM_X_g5);} else
                      if (strcompare(commandnumber,"XEQM18" ) && exec) {sprintf(commandnumber,"%d", ITM_X_g6);} else
                      if (strcompare(commandnumber,"X.SAVE" )) {sprintf(commandnumber,"%d", ITM_XSAVE);} else
                      if (strcompare(commandnumber,"X.LOAD" )) {sprintf(commandnumber,"%d", ITM_XLOAD);} else
                      if (strcompare(commandnumber,"PLOTLS" )) {sprintf(commandnumber,"%d", ITM_PLOTLS);} else
                      if (strcompare(commandnumber,"GRF" )) {sprintf(commandnumber,"%d", ITM_PLOTJM);} else


// FROM SPREADSHEET ^^^ ****************************************************************************************************

// Unlimited GTO                                      GTO M1
// 4 Labels M1, M2, M3 & M4                           XEQLBL M1
// Unlimited DSZ and ISZ                              DSZ 00
// Non-nested subroutine GSB M1..M4; RTN              GSB M01     RTN
// END reacts to labelling parse and execution parse  END
// RETURN reacts to execution parse only              RETURN


                      if (strcompare(commandnumber,"DSZ"    )) {sprintf(commandnumber,"%d", ITM_DEC); gotoinprogress = 9;}      else //EXPECTING FOLLOWING OPERAND "nn"
                       if (strcompare(commandnumber,"ISZ"   )) {sprintf(commandnumber,"%d", ITM_INC); gotoinprogress = 9;}       else //EXPECTING FOLLOWING OPERAND "nn"
                        if (strcompare(commandnumber,"LBL"))       {xeqlblinprogress = 10; }                                      else //EXPECTING FOLLOWING OPERAND Mn
                          if (strcompare(commandnumber,"XEQC43"))   {starttoken = 1; }                                             else //EXPECTING FOLLOWING OPERAND Mn
                           if (strcompare(commandnumber,"XEQLBL"))    {xeqlblinprogress =  1; starttoken = 1;}                      else //EXPECTING 2 OPERANDS nn XXXXXX
                            if (strcompare(commandnumber,"GTO"   ))    {
                                  if(exec) {go = true; gotoinprogress = 1;
                                  force_refresh();
                                  #ifdef PC_BUILD
                                    printf("   >>> Loop GTO Jump %d:go\n",loopnumber++);
                                  #endif
                              }
                            } else
                             if (strcompare(commandnumber,"GSB"   ))    {
                                  if(exec) {go = true; gotoinprogress = 1; 
                                  ix_m = ix;
                                  force_refresh();
                                  #ifdef PC_BUILD
                                    printf("   >>> Sub  GSB Jump %d:go storing return address %d\n",loopnumber++, ix_m);
                                  #endif
                               }
                             } else
                              if (strcompare(commandnumber,"RTN"))       {
                                  if(exec) {ix = ix_m+2; ix_m = 0;
                                  force_refresh();
                                  #ifdef PC_BUILD
                                    printf("   >>> Sub  RTN to return address %d\n", ix);
                                  #endif
                               }
                             } else
                               if (strcompare(commandnumber,"GTO_SZ"))    {if(exec) {go = (temporaryInformation == TI_FALSE); gotoinprogress = 1; }} else
                                if (strcompare(commandnumber,"END"))       {ix = stringByteLength(inputstring)-2;}                      else
                                 if (strcompare(commandnumber,"RETURN"))    {if(exec) {ix = stringByteLength(inputstring)-2;}}           else
//         END ELSE
                                   { 
                                     ii = 0;
                                     while(commandnumber[ii]!=0 && ((commandnumber[ii]<='9' && commandnumber[ii]>='0') || commandnumber[ii]>='.' || commandnumber[ii]>='E' ) ) {
                                       ii++;
                                     }
                                     if(commandnumber[ii]==0 && (gotoinprogress == 0 || gotoinprogress == 10) && xeqlblinprogress == 0 ) {
                                       //printf("   Fell thru, i.e. number %s, gotoinprogress %d\n",commandnumber,gotoinprogress);   // at this stage there SHOULD be a number (not checked) coming out at this point, from the ELSE, it is a number from the text file, therefore literal mnumbers
                                                                 // prepare to break out of this state
                                                                 // and set flags as if in direct character (quotes) state
                                       if (exec) {
                                         sendkeys(commandnumber);
                                         //printf("   sent-->a|%s|\n",commandnumber);
                                       }
                                       commandnumber[0]=0;
                                       if(gotoinprogress == 10) {gotoinprogress = 11;} //if the digits of STO or DSZ
                                       break;
                                     }
                                   }
        //v

                      if(starttoken == 0) {                  //if not started with XEQLBL or XEQC43, immediately abandon program mode
                        running_program_jm = false;
                        return;
                      }
                      
                      //printf("   gotoinprogress = %5d; xeqlblinprogress = %5d; commandnumber = %10s\n",gotoinprogress,xeqlblinprogress,commandnumber);


                      temporaryInformation   = TI_NO_INFO;   //Cancel after go was determined.
                      switch(gotoinprogress) {
                        case 1:                  //GOTO IN PROGRESS: got command GOTO. If arriving from DSZ with no label, just pass around and skip to 2
                          gotoinprogress = 2;
                          commandnumber[0]=0;
                        break;

                        case 2:                  //GOTO IN PROGRESS: if arriving from DSZ / 1, witho9ut M1-M4, waste the commandd, zero, and pass around
                          if(strcompare(commandnumber,"M1") && exec && go && (ix_m1 !=0)) ix = ix_m1; else
                          if(strcompare(commandnumber,"M2") && exec && go && (ix_m2 !=0)) ix = ix_m2; else 
                          if(strcompare(commandnumber,"M3") && exec && go && (ix_m3 !=0)) ix = ix_m3; else
                          if(strcompare(commandnumber,"M4") && exec && go && (ix_m4 !=0)) ix = ix_m4;
                          gotoinprogress = 0;
                          commandnumber[0]=0;   //Processed
                          go = false;
                        break;

                        case 13:                  //GOTO IN PROGRESS: eat one word
                          gotoinprogress = 0;
                          commandnumber[0]=0;
                        break;


                        default:;
                      }



                      switch(xeqlblinprogress) {
                        case 1:                  //XEQMLABEL IN PROGRESS: got command XEQLBL
                          xeqlblinprogress = 2;
                          commandnumber[0]=0;
                        break;

                        case 2:                  //XEQMLABEL IN PROGRESS: get softkeynumber 01 - 18
                          no = atoi(commandnumber);
                          if(no>=1 && no <=18) {
                            xeqlblinprogress = 3;
                            commandnumber[0]=0;   //Processed
                          } 
                          else {
                            xeqlblinprogress = 0;
                            commandnumber[0]=0;   //Processed
                          }
                        break;

                        case 3:                  //XEQMLABEL IN PROGRESS: get label
                          if (no>=1 && no<=18) {
                            char tmpp[commandnumberl];
                            strcpy(tmpp,commandnumber);
                            tmpp[8-1]=0;         //Limit to length of indexOfItemsXEQM
                            //printf(">>> Exec:%d no:%d ComndNo:%s tmpp:%s>>XEQM:%s\n",exec, no,commandnumber, tmpp,indexOfItemsXEQM + (no-1)*8);
                            if(!exec) strcpy(indexOfItemsXEQM + (no-1)*8, tmpp);        // At Exec time, the XEQM label is changed to the command number. So the re-allocation of the name can only happen in the !exec state
                            xeqlblinprogress = 0;
                            commandnumber[0]=0;  //Processed
                            #ifndef TESTSUITE_BUILD
//JMXX                            showSoftmenuCurrentPart(); //Redisplay because softkey content changed
                            #endif
                          }
                        break;

                        case 10:                  //LABEL IN PROGRESS: got command XEQLBL
                          xeqlblinprogress = 11;
                          commandnumber[0]=0;
                        break;

                        case 11:                  //LABEL IN PROGRESS: get label M1-M4
                          //printf("LABEL %s\n",commandnumber);
                          if(strcompare(commandnumber,"M1")) ix_m1 = ix; else
                          if(strcompare(commandnumber,"M2")) ix_m2 = ix; else
                          if(strcompare(commandnumber,"M3")) ix_m3 = ix; else
                          if(strcompare(commandnumber,"M4")) ix_m4 = ix;
                          xeqlblinprogress = 0;
                          commandnumber[0]=0;   //Processed
                          invertPixel(indic_x, indic_y-1);
                        break;
//HERE DEFAULT !!
//NOT IN PROGRESS                        
                        default:                 //NOT IN PROGRESS
                          no = atoi(commandnumber);       //Will force all unknown commands to have no number, and invalid command and RETURN MARK etc. to 0
                          //printf("   Command send %s EXEC=%d no=%d ",commandnumber,exec,no);
                          if(no > LAST_ITEM-1) {no = 0;}
                          if(no!=0 && exec) {
                            invertPixel(indic_x++, indic_y);
                            if(indic_x==SCREEN_WIDTH) {indic_x=0;indic_y--;indic_y--;}

                            if(exec) {
                              runkey(no); 
                              //printf("   -->%d sent ",no);
                            } else {
                              //printf("   -->%d not sent ",no);
                            }
                            //printf(">>> %d\n",temporaryInformation);
                            if(gotoinprogress == 9 ) {gotoinprogress = 10;}
                          }
                          else {
                            //printf("Skip execution |%s|",commandnumber);
                          }
                          //printf("#\n");
                          commandnumber[0]=0;   //Processed
                        break;
                      }
                    }
                    break;
          default:;           //ignore all other characters
        }
        if(state_quotes) {
          if (exec) {
            //printf("   sent-->b|%s|\n",aa);
            sendkeys(aa);} //else printf("Skip sending |%s|",aa);
        }
        else {
          if(state_commands && stringByteLength(commandnumber) < commandnumberl-1) {
            strcat(commandnumber,aa);
          }   // accumulate string
        }
        ix++;
      }

      gotlabels = true;                              //allow to run only once, unless
      if(!exec) exec = exec1; else exec = false;     //exec must run, and ensure it runs only once.
    }
running_program_jm = false;
#endif
}


 
void replaceFF(char* FF, char* line2) {
  int16_t ix =0;
  if(FF[0]>=48 && FF[0]<=57 && FF[1]>=48 && FF[1]<=57 && FF[2]==0) {
    while(line2[ix] != 0 && ix+10<stringByteLength(line2)) {
      if(line2[ix]==88 /*X*/ && line2[ix+1]==69 /*E*/ && line2[ix+2]==81 /*Q*/ && line2[ix+3]==76 /*L*/ && line2[ix+4]==66 /*B*/ && line2[ix+5]==76 /*L*/ && line2[ix+6]==32 && line2[ix+7]==70 /*F*/ && line2[ix+8]==70 /*F*/) {
        line2[ix+7]=FF[0];
        line2[ix+8]=FF[1];
      }
      ix++;
    }
  }
}

void XEQMENU_Selection(uint16_t selection, char *line1, bool_t exec) {
#ifndef TESTSUITE_BUILD
                                            //Read in XEQMINDEX.TXT file, with default XEQMnn file name replacements
  line1[0]=0;                               //Clear incoming/outgoing string data
  char nn[6];
  nn[0]=0;
  char fallback[130];     //Fallback text
  char fn_long[200];      //Long file name
  char fn_short[16];      //standard file name
  char tmp[400];          //Messages


  #define pgmpath "PROGRAMS"
  strcpy(fn_short,"XEQMINDEX.TXT");
  strcpy(fn_long, "");
  strcpy(fallback,"XEQM01:HELP;");

  if(verbose_jm>=1) {
    strcpy(tmp,fn_short); strcat(tmp," A: Loading XEQMENU mapping"); print_linestr(tmp,false);
  }

  import_string_from_filename(line1,pgmpath,fn_short,fn_long,fallback);
  if(verbose_jm>=1) {print_inlinestr(" B: Loaded. ",false);}
  if(verbose_jm>=2) {
    #ifdef DMCP_BUILD
      print_inlinestr("Press a key to continue.",true);
      wait_for_key_press();
    #endif
  }
  if(verbose_jm>=1) {
    clearScreen_old(false, true, true);
  }

  int16_t ix = 0;
  int16_t iy = 0;
  sprintf(nn,"%2d",selection);                   //Create string value for 00
  if(nn[0]==32) {nn[0]=48;}
  if(nn[1]==32) {nn[1]=48;}
  strcpy(fn_short,"XEQM");                        //Build default short file name XEQMnn
  strcat(fn_short,nn);
  strcpy(fn_long,fn_short);

                                            //Find XEQMnn in the replacement token file         
  while(line1[ix] != 0 && ix+6<stringByteLength(line1)) {
     if(line1[ix]==88 /*X*/ && line1[ix+1]==69 /*E*/ && line1[ix+2]==81 /*Q*/ && line1[ix+3]==77 /*M*/ && line1[ix+4]==nn[0] && line1[ix+5]==nn[1] && line1[ix+6]==58 /*:*/) {
       ix = ix + 7;
       iy = ix;                             //If found, find the replacement text after the colon until before the semi-colon
       while(line1[ix] != 0 && ix<stringByteLength(line1)) {
          if(line1[ix] == 59) {line1[ix]=0; strcpy(fn_long,line1 + iy); break;}     //Replace file name with content from replacement string
          ix++;
       }
     } 
     ix++;
  }
  strcat(fn_short,".TXT");                        //Add .TXT
  strcat(fn_long,".TXT");                         //Add .TXT

  if(verbose_jm>=1) {
    sprintf(tmp,"C: Trying %s then %s.",fn_short,fn_long);
    print_linestr(tmp,true);
  }

  line1[0]=0;                                     //Clear incoming/outgoing string data

  //printf(">>> original name:|%s|, replacement file name:|%s|\n",fn_short,fn_long);           
  if(selection==1) {
    sprintf(fallback,"XEQLBL 01 HELP ALPHA \"I\" CASE \"n directory \" CASE \"PROGRAMS\" CASE \" create \" CASE \"XEQM\" CASE \"NN\" CASE \".TXT\" EXIT ");
  } 
  else {
    sprintf(fallback,"XEQLBL %s X%s ",nn,nn);
  }

  if(verbose_jm>=2) {
    sprintf(tmp,"  Fallback:%s",fallback); print_inlinestr(tmp,false);
  }

  import_string_from_filename(line1,pgmpath,fn_short,fn_long,fallback); 
  replaceFF(nn,line1); 
  if(verbose_jm>=1) {
    print_inlinestr(line1,true);
  }

  if(verbose_jm>=2) {
    #ifdef DMCP_BUILD
      print_inlinestr("Press a key to continue.",true);
      wait_for_key_press();
    #endif
  }
  if(verbose_jm>=1) {
    clearScreen_old(false, true, true);
  }

  displaywords(line1);

  if(verbose_jm>=2) {
    #ifdef DMCP_BUILD
      print_inlinestr("Press a key to continue.",true);
      wait_for_key_press();
    #endif
  }
  if(verbose_jm>=1) {
    clearScreen_old(false, true, true);
  }

  execute_string(line1,exec);

  if(verbose_jm>=2) {
    #ifdef DMCP_BUILD
      print_inlinestr("Press a key to continue",true);
      wait_for_key_press();
      clearScreen_old(false, true, true);
    #endif
  }

#endif
}



void fnXEQMENU(uint16_t XEQM_no) {
  #ifndef TESTSUITE_BUILD
  clearScreen_old(false, true, true);
  print_linestr("Loading XEQM program file:",true);

  char line[TMP_STR_LENGTH];
  XEQMENU_Selection( XEQM_no, line, true);

  //calcMode = CM_BUG_ON_SCREEN;
  //temporaryInformation = TI_NO_INFO;
  #endif
}



void XEQMENU_loadAllfromdisk(void) {
#ifndef TESTSUITE_BUILD
      clearScreen_old(false, true, true);
      print_inlinestr("Loading all XEQM program files:",true);

      char line[TMP_STR_LENGTH];
      
      char tmp[10];
      uint8_t ix = 1;
      while(ix<=18) {
        sprintf(tmp,"%2d",ix);
        print_inlinestr(tmp,false);
        XEQMENU_Selection( ix, line, false);
        ix++;
      }
    #endif
}



/*
//Fixed test program, dispatching commands
void testprogram_12(uint16_t unusedParamButMandatory){

    runkey(ITM_TICKS); //622
    runkey(684);       //X<>Y
    sendkeys("2"); runkey(ITM_EXIT1); //EXIT
    runkey(684);   //X<>Y
    runkey(698);   //Y^X
    sendkeys("1"); runkey(ITM_EXIT1); //EXIT
    runkey(780);   //-
    runkey(589);   sendkeys("00"); //STO 00
    runkey(469);   //PRIME?
    runkey(684);   //X<>Y
    runkey(ITM_TICKS);
    runkey(684);   //X<>Y
    runkey(ITM_SUB);
}


//Fixed test program, dispatching commands from text string
void testprogram_1(uint16_t unusedParamButMandatory){
char line1[TMP_STR_LENGTH];
   strcpy(line1,
    "TICKS //RPN Program to demostrate PRIME// "
    "\"2\" EXIT "
    "\"2203\" "
    "Y^X "
    "\"1\" - "
    "PRIME?  "
    "X<>Y "
    "TICKS "
    "X<>Y - "
    "\"10.0\" / "    
    "RETURN "
    "ABCDEFGHIJKLMNOPQ!@#$%^&*()\n"
    );
    displaywords(line1);
    execute_string(line1,true);
}
*/




void fnXEQMSAVE (uint16_t XEQM_no) {                                  //X-REGISTER TO DISK
  char tt[40]; 
  if(getRegisterDataType(REGISTER_X) == dtString) {
    xcopy(tmpStr3000 + TMP_STR_LENGTH/2, REGISTER_STRING_DATA(REGISTER_X), stringByteLength(REGISTER_STRING_DATA(REGISTER_X))+1);
    tt[0]=0;
    switch(XEQM_no) {
      case  1:strcpy(tt, "XEQM01.TXT"); break;
      case  2:strcpy(tt, "XEQM02.TXT"); break;
      case  3:strcpy(tt, "XEQM03.TXT"); break;
      case  4:strcpy(tt, "XEQM04.TXT"); break;
      case  5:strcpy(tt, "XEQM05.TXT"); break;
      case  6:strcpy(tt, "XEQM06.TXT"); break;
      case  7:strcpy(tt, "XEQM07.TXT"); break;
      case  8:strcpy(tt, "XEQM08.TXT"); break;
      case  9:strcpy(tt, "XEQM09.TXT"); break;
      case 10:strcpy(tt, "XEQM10.TXT"); break;
      case 11:strcpy(tt, "XEQM11.TXT"); break;
      case 12:strcpy(tt, "XEQM12.TXT"); break;
      case 13:strcpy(tt, "XEQM13.TXT"); break;
      case 14:strcpy(tt, "XEQM14.TXT"); break;
      case 15:strcpy(tt, "XEQM15.TXT"); break;
      case 16:strcpy(tt, "XEQM16.TXT"); break;
      case 17:strcpy(tt, "XEQM17.TXT"); break;
      case 18:strcpy(tt, "XEQM18.TXT"); break;
      default:;
    }

    #ifdef PC_BUILD
    printf(">>> string ready  ## %s:%s\n",tt,tmpStr3000 + TMP_STR_LENGTH/2);
    //uint16_t ix = 0;while (ix!=20) {printf("%d:%d=\n",ix,tmpStr3000[ix]);ix++;}
    stringToUtf8(tmpStr3000 + TMP_STR_LENGTH/2, (uint8_t *)tmpStr3000);
    printf(">>> string in utf ## %s:%s\n",tt, tmpStr3000);
    //ix = 0;while (ix!=20) {printf("%d:%d=\n",ix,ll[ix]);ix++;}
    #endif

    #ifndef TESTSUITE_BUILD
      stringToUtf8(tmpStr3000 + TMP_STR_LENGTH/2, (uint8_t *)tmpStr3000);
      if(tt[0]!=0) export_string_to_filename(tmpStr3000, overwrite, "PROGRAMS", tt);
    #endif
  }
}


void fnXEQMLOAD (uint16_t XEQM_no) {                                  //DISK to X-REGISTER
  #ifdef PC_BUILD
    printf("LOAD %d\n",XEQM_no);
  #endif
  char line1[TMP_STR_LENGTH];
  line1[0]=0;
  XEQMENU_Selection(XEQM_no, line1, false);
  uint16_t ix = 0;while (ix!=20) {
    #ifdef PC_BUILD
      printf("%d ",line1[ix]);
    #endif
    ix++;
  }
  //printf(">>> loaded: utf:%s\n",line1);  
  utf8ToString((uint8_t *)line1,line1 + TMP_STR_LENGTH/2);
  //ix = 0;while (ix!=20) {printf("%d ",line1[ix]);ix++;}  printf(">>> loaded: str:%s\n",line1 + TMP_STR_LENGTH/2);  
  int16_t len = stringByteLength(line1 + TMP_STR_LENGTH/2);
  liftStack();
  reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);
  strcpy(REGISTER_STRING_DATA(REGISTER_X),line1 + TMP_STR_LENGTH/2);

}




void fnXEQMEDIT (uint16_t unusedParamButMandatory) {
  if(calcMode == CM_AIM && getRegisterDataType(REGISTER_Y) == dtString) {
    //printf(">>> !@# stringByteLength(REGISTER_STRING_DATA(REGISTER_Y))=%d; AIM_BUFFER_LENGTH=%d\n",stringByteLength(REGISTER_STRING_DATA(REGISTER_Y)),AIM_BUFFER_LENGTH);
    if(stringByteLength(REGISTER_STRING_DATA(REGISTER_Y)) < AIM_BUFFER_LENGTH) {
      if(eRPN) {      //JM NEWERPN 
        setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
      }                                        //JM NEWERPN 
      strcpy(aimBuffer, REGISTER_STRING_DATA(REGISTER_Y)); 
      T_cursorPos = stringByteLength(aimBuffer);
      fnDrop(0);
/*      refreshRegisterLine(REGISTER_T);
      refreshRegisterLine(REGISTER_Z);
      refreshRegisterLine(REGISTER_Y);
*/      refreshRegisterLine(REGISTER_X);        //JM Execute here, to make sure that the 5/2 line check is done
      last_CM=253;

    }
  }
  else
  if(calcMode == CM_AIM && (getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtComplex34  || getRegisterDataType(REGISTER_Y) == dtLongInteger   || getRegisterDataType(REGISTER_Y) == dtShortInteger)) {
    if(stringByteLength(REGISTER_STRING_DATA(REGISTER_Y)) < AIM_BUFFER_LENGTH) {
      if(eRPN) {      //JM NEWERPN 
        setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
      }                                        //JM NEWERPN 

      char tmp[2];
      tmp[0]=0;
      int16_t len = stringByteLength(tmp) + 1;

      reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);           //Make blank string in X
      xcopy(REGISTER_STRING_DATA(REGISTER_X), tmp, len);
      addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();//Convert Y number to string in X REGISTER
      adjustResult(REGISTER_X, false, false, -1, -1, -1);                          //Copy X string to Aimbuffer
      strcpy(aimBuffer, REGISTER_STRING_DATA(REGISTER_X)); 

      T_cursorPos = stringByteLength(aimBuffer);
      fnDrop(0);
      refreshRegisterLine(REGISTER_X);        //JM Execute here, to make sure that the 5/2 line check is done
      last_CM=253;

    }
  }


  else if (calcMode == CM_NORMAL && getRegisterDataType(REGISTER_X) == dtString) {
    if(stringByteLength(REGISTER_STRING_DATA(REGISTER_X)) < AIM_BUFFER_LENGTH) {
      if(eRPN) {      //JM NEWERPN 
        setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
      }                                        //JM NEWERPN 
      strcpy(aimBuffer, REGISTER_STRING_DATA(REGISTER_X));
      T_cursorPos = stringByteLength(aimBuffer);
      fnDrop(0);
      #ifndef TESTSUITE_BUILD
        resetShiftState();
        calcModeAim(NOPARAM); // Alpha Input Mode
        showSoftmenu(NULL, -MNU_T_EDIT, true);
      #endif
    } 
//    else {
//    	printf(">>> !@# stringByteLength(REGISTER_STRING_DATA(REGISTER_Y))=%d; AIM_BUFFER_LENGTH=%d\n",stringByteLength(REGISTER_STRING_DATA(REGISTER_Y)),AIM_BUFFER_LENGTH);
//    }
  }
  else if (calcMode == CM_NORMAL && getRegisterDataType(REGISTER_X) != dtString) {
    char line1[TMP_STR_LENGTH];
    line1[0]=0;
    strcpy(line1," ");
    int16_t len = stringByteLength(line1);
    if(eRPN) {      //JM NEWERPN 
      setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
    }                                        //JM NEWERPN 
    liftStack();
    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);
    strcpy(REGISTER_STRING_DATA(REGISTER_X),line1);
    fnXEQMEDIT(0);
  }
  last_CM=252;
  refreshScreen();
  last_CM=251;
  refreshScreen();
}


void fnXEQMXXEQ (uint16_t unusedParamButMandatory) {
  char line1[3000];
  if(getRegisterDataType(REGISTER_X) == dtString) {
    xcopy(line1, REGISTER_STRING_DATA(REGISTER_X), stringByteLength(REGISTER_STRING_DATA(REGISTER_X)) + 1);
    fnDrop(0);
    displaywords(line1); 
    execute_string(line1,false); 
    execute_string(line1,true);
  }

}



void fnXEQNEW (uint16_t unusedParamButMandatory) {
  fnStrtoX("XEQC43 XEQLBL 01 XXXXXX ");
  fnXEQMEDIT(0);
}


void fnCla(uint16_t unusedParamButMandatory){
  //Not using calcModeAim becose some modes are reset which should not be
  aimBuffer[0]=0;
  T_cursorPos = 0;
  nextChar = NC_NORMAL;
  xCursor = 1;
  yCursor = Y_POSITION_OF_AIM_LINE + 6;
  cursorFont = &standardFont;
  cursorEnabled = true;
  last_CM=252;
  #ifndef TESTSUITE_BUILD
    clearRegisterLine(AIM_REGISTER_LINE, true, true);
    refreshRegisterLine(AIM_REGISTER_LINE);        //JM Execute here, to make sure that the 5/2 line check is done
  #endif
  last_CM=253;
}


void fnCln(uint16_t unusedParamButMandatory){
  #ifndef TESTSUITE_BUILD
    calcModeNim(0);
    last_CM=252;
    refreshRegisterLine(REGISTER_X);        //JM Execute here, to make sure that the 5/2 line check is done
    last_CM=253;
    addItemToNimBuffer(ITM_0);
  #endif
}



