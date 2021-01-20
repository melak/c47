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

void press_key(void) {
    #ifdef DMCP_BUILD
      print_inlinestr("Press key",true);
      wait_for_key_press();
    #endif
}


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
    stringToUtf8(line1, (uint8_t *)tmpString);
    export_string_to_file(tmpString);
    #endif
}


//############################ SEND KEY TO 43S ENGINE ####################
void runkey(int16_t item){
  #ifndef TESTSUITE_BUILD

/*
char tmp[2];
tmp[0]=0;
executeFunction(tmp, item);
*/  


  if(item < 0) {
    showSoftmenu(item);
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
      case '.':  runkey(ITM_PERIOD); break; //.
      case 'E':  runkey(ITM_EXPONENT); break; //E
      case 'e': runkey(ITM_EXPONENT); break; //e
      case '-':  runkey(ITM_SUB); break; //-
      case '+':  runkey(ITM_ADD); break; //+
      case ' ':  runkey(ITM_SPACE); break; //space
      case '#':  runkey(ITM_toINT); break; //#
      case ':':  runkey(ITM_COLON        ); break; //#
      case ';':  runkey(ITM_SEMICOLON    ); break; //#
      case '\'': runkey(ITM_QUOTE        ); break; //#
      case '?':  runkey(ITM_QUESTION_MARK); break; //#
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
uint32_t indic_x = 0;
uint32_t indic_y = SCREEN_HEIGHT-1;




//                     ****************************************************************************************************
bool_t checkindexes(int16_t *com,  char *str, bool_t exec) {
                      *com = 0;
//FROM SPREADSHEET vvv ****************************************************************************************************

//XEQM TYPE1 REDUCED SELECTED LIST
                      if (strcompare(str,"PRIME?" )) { *com = ITM_PRIME;} else
                      if (strcompare(str,"ENTER" )) { *com = ITM_ENTER;} else
                      if (strcompare(str,"X<>Y" )) { *com = ITM_XexY;} else
                      if (strcompare(str,"DROP" )) { *com = ITM_DROP;} else
                      if (strcompare(str,"CLX" )) { *com = ITM_CLX;} else
                      if (strcompare(str,"FILL" )) { *com = ITM_FILL; } else
                      if (strcompare(str,"STO" )) { *com = ITM_STO;} else
                      if (strcompare(str,"COMB" )) { *com = ITM_COMB;} else
                      if (strcompare(str,"PERM" )) { *com = ITM_PERM;} else
                      if (strcompare(str,"RCL" )) { *com = ITM_RCL;} else
                      if (strcompare(str,"X^2" )) { *com = ITM_SQUARE;} else
                      if (strcompare(str,"X^3" )) { *com = ITM_CUBE;} else
                      if (strcompare(str,"Y^X" )) { *com = ITM_YX;} else
                      if (strcompare(str,"SQRT" )) { *com = ITM_SQUAREROOTX;} else
                      if (strcompare(str,"CUBRT" )) { *com = ITM_CUBEROOT;} else
                      if (strcompare(str,"XRTY" )) { *com = ITM_XTHROOT;} else
                      if (strcompare(str,"2^X" )) { *com = ITM_2X;} else
                      if (strcompare(str,"E^X" )) { *com = ITM_EXP;} else
                      if (strcompare(str,"10^X" )) { *com = ITM_10x;} else
                      if (strcompare(str,"LOG2" )) { *com = ITM_LOG2;} else
                      if (strcompare(str,"LN" )) { *com = ITM_LN;} else
                      if (strcompare(str,"LOG10" )) { *com = ITM_LOG10;} else
                      if (strcompare(str,"LOGXY" )) { *com = ITM_LOGXY;} else
                      if (strcompare(str,"1/X" )) { *com = ITM_1ONX;} else
                      if (strcompare(str,"COS" )) { *com = ITM_cos;} else
                      if (strcompare(str,"COSH" )) { *com = ITM_cosh;} else
                      if (strcompare(str,"SIN" )) { *com = ITM_sin;} else
                      if (strcompare(str,"SINH" )) { *com = ITM_sinh;} else
                      if (strcompare(str,"TAN" )) { *com = ITM_tan;} else
                      if (strcompare(str,"TANH" )) { *com = ITM_tanh;} else
                      if (strcompare(str,"ARCCOS" )) { *com = ITM_arccos;} else
                      if (strcompare(str,"ARCCOSH" )) { *com = ITM_arcosh;} else
                      if (strcompare(str,"ARCSIN" )) { *com = ITM_arcsin;} else
                      if (strcompare(str,"ARCSINH" )) { *com = ITM_arsinh;} else
                      if (strcompare(str,"ARCTAN" )) { *com = ITM_arctan;} else
                      if (strcompare(str,"ARCTANH" )) { *com = ITM_artanh;} else
                      if (strcompare(str,"GCD" )) { *com = ITM_GCD;} else
                      if (strcompare(str,"LCM" )) { *com = ITM_LCM;} else
                      if (strcompare(str,"DEC" )) { *com = ITM_DEC;} else
                      if (strcompare(str,"INC" )) { *com = ITM_INC;} else
                      if (strcompare(str,"IP" )) { *com = ITM_IP;} else
                      if (strcompare(str,"FP" )) { *com = ITM_FP;} else
                      if (strcompare(str,"+" )) { *com = ITM_ADD;} else
                      if (strcompare(str,"-" )) { *com = ITM_SUB;} else
                      if (strcompare(str,"CHS" )) { *com = ITM_CHS;} else
                      if (strcompare(str,"*" )) { *com = ITM_MULT;} else
                      if (strcompare(str,"/" )) { *com = ITM_DIV;} else
                      if (strcompare(str,"IDIV" )) { *com = ITM_IDIV;} else
//                      if (strcompare(str,"MOD" )) { *com = ITM_MOD;} else
                      if (strcompare(str,"ABS" )) { *com = ITM_MAGNITUDE;} else
                      if (strcompare(str,"NEXTP" )) { *com = ITM_NEXTP;} else
//                      if (strcompare(str,"X!" )) { *com = ITM_XFACT;} else
                      if (strcompare(str,"PI" )) { *com = ITM_CONSTpi;} else
//                      if (strcompare(str,">DEG" )) { *com = ITM_toDEG;} else
//                      if (strcompare(str,">RAD" )) { *com = ITM_toRAD;} else
                      if (strcompare(str,"D>R" )) { *com = ITM_DtoR;} else
                      if (strcompare(str,"R>D" )) { *com = ITM_RtoD;} else
//                      if (strcompare(str,"RMD" )) { *com = ITM_RMD;} else
                      if (strcompare(str,"c" )) { *com = CST_05;} else
                      if (strcompare(str,"PHI" )) { *com = CST_74;} else
 //                     if (strcompare(str,"-INF" )) { *com = CST_77;} else
 //                     if (strcompare(str,"INF" )) { *com = CST_78;} else
                      if (strcompare(str,"SUM+" )) { *com = ITM_SIGMAPLUS;} else
                      if (strcompare(str,"NSUM" )) { *com = ITM_NSIGMA;} else
                      if (strcompare(str,"SUMX" )) { *com = ITM_SIGMAx;} else
                      if (strcompare(str,"SUMY" )) { *com = ITM_SIGMAy;} else
                      if (strcompare(str,"X" )) { *com = ITM_REG_X;} else
                      if (strcompare(str,"Y" )) { *com = ITM_REG_Y;} else
                      if (strcompare(str,"IND>" )) { *com = ITM_INDIRECTION;} else
                      if (strcompare(str,"EEX" )) { *com = ITM_EXPONENT;} else
                      if (strcompare(str,"SNAP" )) { *com = ITM_SNAP;} else
                      if (strcompare(str,"ABS" )) { *com = ITM_ABS;} else
                      if (strcompare(str,"ALL" )) { *com = ITM_ALL;} else
                      if (strcompare(str,"BATT?" )) { *com = ITM_BATT;} else
                      if (strcompare(str,"CASE" )) { *com = CHR_case;} else
//                      if (strcompare(str,"CLREGS" )) { *com = ITM_CLREGS;} else
                      if (strcompare(str,"CLSTK" )) { *com = ITM_CLSTK;} else
                      if (strcompare(str,"CLSUM" )) { *com = ITM_CLSIGMA;} else
                      if (strcompare(str,"DEG" )) { *com = ITM_DEG;} else
                      if (strcompare(str,"ENG" )) { *com = ITM_ENG;} else
                      if (strcompare(str,"EXPT" )) { *com = ITM_EXPT;} else
                      if (strcompare(str,"FIB" )) { *com = ITM_FIB;} else
                      if (strcompare(str,"FIX" )) { *com = ITM_FIX;} else
                      if (strcompare(str,"GD" )) { *com = ITM_GD;} else
                      if (strcompare(str,"GD^-1" )) { *com = ITM_GDM1;} else
                      if (strcompare(str,"IM" )) { *com = ITM_IM; } else
                      if (strcompare(str,"RE" )) { *com = ITM_RE; } else
                      if (strcompare(str,"SINC" )) { *com = ITM_sinc;} else
//                      if (strcompare(str,"LASTX" )) { *com = ITM_LASTX;} else
                      if (strcompare(str,"MULPI" )) { *com = ITM_MULPI;} else
                      if (strcompare(str,"SINCPI" )) { *com = ITM_sincpi;} else
                      if (strcompare(str,"PLOT" )) { *com = ITM_PLOT;} else
                      if (strcompare(str,"RAD" )) { *com = ITM_RAD;} else
                      if (strcompare(str,"RAN#" )) { *com = ITM_RAN;} else
                      if (strcompare(str,"E^X-1" )) { *com = ITM_EX1;} else
                      if (strcompare(str,"SCI" )) { *com = ITM_SCI;} else
                      if (strcompare(str,"LN(1+X)" )) { *com = ITM_LN1X;} else
//                      if (strcompare(str,"SUM" )) { *com = ITM_SUM;} else
                      if (strcompare(str,"TICKS" )) { *com = ITM_TICKS;} else
                      if (strcompare(str,"(-1)^X" )) { *com = ITM_M1X;} else
                      if (strcompare(str,"ARG" )) { *com = ITM_ANGLE;} else
                      if (strcompare(str,"EXIT" )) { *com = ITM_EXIT1;} else
                      if (strcompare(str,"ALPHA" )) { *com = ITM_AIM;} else
                      if (strcompare(str,"DOTD" )) { *com = ITM_dotD;} else
                      if (strcompare(str,"COMPLEX" )) { *com = KEY_COMPLEX;} else
                      if (strcompare(str,">POLAR" )) { *com = ITM_toPOL2;} else
                      if (strcompare(str,">RECT" )) { *com = ITM_toREC2;} else
                      if (strcompare(str,"ERPN" )) { *com = ITM_eRPN_ON;} else
                      if (strcompare(str,"RPN" )) { *com = ITM_eRPN_OFF;} else
                      if (strcompare(str,"TEST_45" )) { *com = ITM_PGMTST;} else
                      if (strcompare(str,"SIG" )) { *com = ITM_SIGFIG;} else
                      if (strcompare(str,"ROUND" )) { *com = ITM_ROUND2;} else
                      if (strcompare(str,"ROUNDI" )) { *com = ITM_ROUNDI2;} else
//                      if (strcompare(str,"OP_A" )) { *com = ITM_op_a;} else
//                      if (strcompare(str,"OP_J" )) { *com = ITM_op_j;} else
//                      if (strcompare(str,"LNGINT" )) { *com = ITM_LI;} else
                      if (strcompare(str,"CASEUP" )) { *com = CHR_caseUP;} else
                      if (strcompare(str,"CASEDN" )) { *com = CHR_caseDN;} else
                      if (strcompare(str,"LISTXY" )) { *com = ITM_LISTXY;} else
                      if (strcompare(str,"PLOTLS" )) { *com = ITM_PLOTLS;} else
                      if (strcompare(str,"P_INT" )) { *com = ITM_INTG;} else
                      if (strcompare(str,"P_DIFF" )) { *com = ITM_DIFF;} else
                      if (strcompare(str,"P_RMS" )) { *com = ITM_RMS;} else
                      if (strcompare(str,"P_SHADE" )) { *com = ITM_SHADE;} else
                      if (strcompare(str,"CLGRF" )) { *com = ITM_CLGRF;} else


//FROM SPREADSHEET ^^^ ****************************************************************************************************
                        {*com = 0;}
                      if(*com != 0) return true;
                      else          return false;
                    }




void execute_string(const char *inputstring, bool_t exec1) {
#ifndef TESTSUITE_BUILD
      int16_t commno;
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
    
      #ifdef PC_BUILD_VERBOSE1
        #ifdef PC_BUILD
          printf("------Starting parse ------- Indexes: M1:%d M2:%d M3:%d M4:%d   EXEC:%d\n",ix_m1, ix_m2, ix_m3, ix_m4, exec);
          printf("|%s|\n",inputstring);
        #endif
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
                      #ifdef PC_BUILD_VERBOSE1
                        #ifdef PC_BUILD
                          printf("Command/number detected:(tempjm=%d)(gotoinprogress=%d) %45s \n",temporaryInformation,gotoinprogress,commandnumber);
                        #endif
                      #endif
                      
                      //DSZ:
                      if(!(gotoinprogress != 11 || (gotoinprogress == 11 && (temporaryInformation == TI_FALSE)))) {     //If DEC results in 0, then 'true'.    It is now the command that may or may not be skipped
                          //......IS NOT DSZ.... OR               DSZ    with REG NOT ZERO
                          go = (temporaryInformation == TI_FALSE); //As per GTO_SZ ---- REGISTER<>0, then go
                          //printf("   DSZ/ISZ temporaryInformation = %5d\n",temporaryInformation);
                          gotoinprogress = 1;                      //As per GTO_SZ
                          commandnumber[0]=0;                      //As per GTO_SZ
                      } else


// Unlimited GTO                                      GTO M1
// 4 Labels M1, M2, M3 & M4                           XEQLBL M1
// Unlimited DSZ and ISZ                              DSZ 00
// Non-nested subroutine GSB M1..M4; RTN              GSB M01     RTN
// END reacts to labelling parse and execution parse  END
// RETURN reacts to execution parse only              RETURN

                      if(checkindexes(&commno, commandnumber, exec)) {sprintf(commandnumber,"%d", commno);} else
                   
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
                          invert_Pixel(indic_x, indic_y-1);
                        break;
//HERE DEFAULT !!
//NOT IN PROGRESS                        
                        default:                 //NOT IN PROGRESS
                          no = atoi(commandnumber);       //Will force all unknown commands to have no number, and invalid command and RETURN MARK etc. to 0
                          //printf("   Command send %s EXEC=%d no=%d ",commandnumber,exec,no);
                          if(no > LAST_ITEM-1) {no = 0;}
                          if(no!=0 && exec) {
                            invert_Pixel(indic_x++, indic_y);
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

  #if (VERBOSE_LEVEL >= 1) 
    char tmp[400];          //Messages
  #endif


  #define pgmpath "PROGRAMS"
  strcpy(fn_short,"XEQMINDEX.TXT");
  strcpy(fn_long, "");
  strcpy(fallback,"XEQM01:HELP;");

  #if (VERBOSE_LEVEL >= 1) 
    strcpy(tmp,fn_short); strcat(tmp," A: Loading XEQMENU mapping"); print_linestr(tmp,false);
  #endif

  import_string_from_filename(line1,pgmpath,fn_short,fn_long,fallback);

  #if (VERBOSE_LEVEL >= 1) 
   print_inlinestr(" B: Loaded. ",false);
  #endif

  #if (VERBOSE_LEVEL >= 2)
    #ifdef DMCP_BUILD
      press_key();
    #endif
  #endif

  #if (VERBOSE_LEVEL >= 1) 
    clearScreen_old(false, true, true);
  #endif

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

  #if (VERBOSE_LEVEL >= 1) 
    sprintf(tmp,"C: Trying %s then %s.",fn_short,fn_long);
    print_linestr(tmp,true);
  #endif

  line1[0]=0;                                     //Clear incoming/outgoing string data

  //printf(">>> original name:|%s|, replacement file name:|%s|\n",fn_short,fn_long);           
  if(selection==1) {
    sprintf(fallback,"XEQLBL 01 HELP ALPHA \"I\" CASE \"n directory \" CASE \"PROGRAMS\" CASEDN \" create \" CASEUP \"XEQM\" CASEDN \"NN\" CASEUP \".TXT\" EXIT ");
  } 
  else {
    sprintf(fallback,"XEQLBL %s X%s ",nn,nn);
  }

  #if (VERBOSE_LEVEL >= 2) 
    sprintf(tmp,"  Fallback:%s",fallback); print_inlinestr(tmp,false);
  #endif

  import_string_from_filename(line1,pgmpath,fn_short,fn_long,fallback); 
  replaceFF(nn,line1); 
  #if (VERBOSE_LEVEL >= 1) 
    print_inlinestr(line1,true);
  #endif

  #if (VERBOSE_LEVEL >= 2) 
    #ifdef DMCP_BUILD
      press_key();
    #endif
  #endif


  #if (VERBOSE_LEVEL >= 1) 
    clearScreen_old(false, true, true);
  #endif

  displaywords(line1);

  #if (VERBOSE_LEVEL >= 2) 
    #ifdef DMCP_BUILD
      press_key();
    #endif
  #endif

  #if (VERBOSE_LEVEL >= 1) 
    clearScreen_old(false, true, true);
  #endif

  execute_string(line1,exec);

  #if (VERBOSE_LEVEL >= 2) 
    #ifdef DMCP_BUILD
      press_key();
      clearScreen_old(false, true, true);
    #endif
  #endif

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
      print_inlinestr("Loading XEQM:",true);

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
void testprogram_12(uint16_t unusedButMandatoryParameter){

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
void testprogram_1(uint16_t unusedButMandatoryParameter){
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
    xcopy(tmpString + TMP_STR_LENGTH/2, REGISTER_STRING_DATA(REGISTER_X), stringByteLength(REGISTER_STRING_DATA(REGISTER_X))+1);
    tt[0]=0;

    sprintf(tt,"XEQM%02u.TXT",XEQM_no);
    //printf("^^^^^^^^^^^^^%s\n",tt);

/*
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
*/

    #ifdef PC_BUILD
    printf(">>> string ready  ## %s:%s\n",tt,tmpString + TMP_STR_LENGTH/2);
    //uint16_t ix = 0;while (ix!=20) {printf("%d:%d=\n",ix,tmpString[ix]);ix++;}
    stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
    printf(">>> string in utf ## %s:%s\n",tt, tmpString);
    //ix = 0;while (ix!=20) {printf("%d:%d=\n",ix,ll[ix]);ix++;}
    #endif

    #ifndef TESTSUITE_BUILD
      stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
      if(tt[0]!=0) export_string_to_filename(tmpString, overwrite, "PROGRAMS", tt);
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




void fnXEQMEDIT (uint16_t unusedButMandatoryParameter) {

          if(aimBuffer[0] != 0) {          //JM if something already in the AIMB|UFFER when X-EDIT is called, store this in the stack first
            setSystemFlag(FLAG_ASLIFT);
            liftStack();
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
            copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_Y);

            int16_t len = stringByteLength(aimBuffer) + 1;
            reallocateRegister(REGISTER_Z, dtString, TO_BLOCKS(len), AM_NONE);
            xcopy(REGISTER_STRING_DATA(REGISTER_Z), aimBuffer, len);
            aimBuffer[0] = 0;

            setSystemFlag(FLAG_ASLIFT);
          }


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
        showSoftmenu(-MNU_T_EDIT);
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


void fnXEQMXXEQ (uint16_t unusedButMandatoryParameter) {
  char line1[3000];
  if(getRegisterDataType(REGISTER_X) == dtString) {
    xcopy(line1, REGISTER_STRING_DATA(REGISTER_X), stringByteLength(REGISTER_STRING_DATA(REGISTER_X)) + 1);
    fnDrop(0);
    displaywords(line1); 
    execute_string(line1,false); 
    execute_string(line1,true);
  }

}



void fnXEQNEW (uint16_t unusedButMandatoryParameter) {
  fnStrtoX("XEQC43 XEQLBL 01 XXXXXX ");
  fnXEQMEDIT(0);
}


void fnCla(uint16_t unusedButMandatoryParameter){
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


void fnCln(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
    calcModeNim(0);
    last_CM=252;
    refreshRegisterLine(REGISTER_X);        //JM Execute here, to make sure that the 5/2 line check is done
    last_CM=253;
    addItemToNimBuffer(ITM_0);
  #endif
}



