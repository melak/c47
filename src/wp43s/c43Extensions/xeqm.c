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

#include "c43Extensions/xeqm.h"

#include "bufferize.h"
#include "charString.h"
#include "flags.h"
#include "gui.h"
#include "c43Extensions/graphText.h"
#include "items.h"
#include "c43Extensions/jm.h"
#include "keyboard.h"
#include "c43Extensions/keyboardTweak.h"
#include "registers.h"
#include "screen.h"
#include "softmenus.h"
#include "stack.h"
#include "statusBar.h"
#include "timer.h"
#include "c43Extensions/textfiles.h"
#include <string.h>

#include "wp43s.h"

#define commandnumberl NIM_BUFFER_LENGTH

#define SCAN true
#define EXEC true



void press_key(void) {
    #ifdef DMCP_BUILD
      print_inlinestr("Press key",true);
      wait_for_key_press();
    #endif
}

#define XEQ_STR_LENGTH_SHORT 300
#define XEQ_STR_LENGTH_LONG  3000 //note the limit is the tmpString limit

void capture_sequence(char *origin, uint16_t item) {
  #ifdef PC_BUILD
   //printf("Captured: %4d   //%10s//  (%s)\n",item,indexOfItems[item].itemSoftmenuName, origin);
   char line1[XEQ_STR_LENGTH_LONG];
   char ll[commandnumberl];

    line1[0]=0;
    ll[0]=0; ll[1]=0;
    switch (item) {
      case  ITM_XexY: strcpy(ll,"X<>Y"); break;
      case  ITM_YX:   strcpy(ll,"Y^X" ); break;
      case  ITM_DIV:  strcpy(ll,"/"   ); break;

      case  ITM_0:
      case  ITM_1:
      case  ITM_2:
      case  ITM_3:
      case  ITM_4:
      case  ITM_5:
      case  ITM_6:
      case  ITM_7:
      case  ITM_8:
      case  ITM_9:        ll[0]=item - ITM_0 + 48; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break;
      case  ITM_PERIOD:   ll[0]=46; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break; //.
      case  ITM_EXPONENT: ll[0]=69; strcpy(line1,"   \""); strcat(line1,ll); strcat(line1,"\" "); break; //E
      default: { strcpy(ll,indexOfItems[item].itemSoftmenuName);
               }  
    }

    if(line1[0]==0) sprintf(line1, " %4d //%10s",item,ll);

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
      case '.':  runkey(ITM_PERIOD);        break; //.
      case 'E':  runkey(ITM_EXPONENT);      break; //E
      case 'e': runkey(ITM_EXPONENT);       break; //e
      case '-':  runkey(ITM_SUB);           break; //-
      case '+':  runkey(ITM_ADD);           break; //+
      case ' ':  runkey(ITM_SPACE);         break; //space
      case '#':  runkey(ITM_toINT);         break; //#
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



typedef struct {
  uint16_t itemNr;            ///<
  char     *itemName;         ///<
} function_t;


TO_QSPI const function_t indexOfFunctions[] = {
#ifndef SAVE_SPACE_DM42_2
//            function                      functionName
//XEQM TYPE1 ORIGINAL FULL SELECTED LIST

//           {ITM_XEQ,   "XEQ"},
//           {ITM_XEQU,   "X=?"},
//           {ITM_XNE,   "XNOT_EQUAL?"},
//           {ITM_XEQUP0,   "X=+0?"},
//           {ITM_XEQUM0,   "X=-0?"},
//           {ITM_XAEQU,   "XALMOST_EQUAL?"},
//           {ITM_XLT,   "X<?"},
//           {ITM_XLE,   "XLESS_EQUAL?"},
//           {ITM_XGE,   "XGREATER_EQUAL?"},
//           {ITM_XGT,   "X>?"},
//           {ITM_FC,   "FC?"},
//           {ITM_FS,   "FS?"},
//           {ITM_EVEN,   "EVEN?"},
//           {ITM_ODD,   "ODD?"},
//           {ITM_FPQ,   "FP?"},
//           {ITM_INT,   "INT?"},
//           {ITM_CPX,   "CPX?"},
//           {ITM_MATR,   "MATR?"},
//           {ITM_NAN,   "NAN?"},
//           {ITM_REAL,   "REAL?"},
//           {ITM_SPEC,   "SPEC?"},
//           {ITM_STRI,   "STRI?"},
//           {ITM_PMINFINITY,   "PLUS_MINUSINFINITY?"},
           {ITM_PRIME,   "PRIME?"},
//           {ITM_TOP,   "TOP?"},
           {ITM_ENTER,   "ENTER"},
           {ITM_XexY,   "X<>Y"},
           {ITM_DROP,   "DROP"},
//           {ITM_PAUSE,   "PAUSE"},
//           {ITM_Rup,   "R"},
//           {ITM_Rdown,   "RDOWN_ARROW"},
           {ITM_CLX,   "CLX"},
           {ITM_FILL,   "FILL"},
//           {ITM_INPUT,   "INPUT"},
           {ITM_STO,   "STO"},
           {ITM_COMB,   "COMB"},
           {ITM_PERM,   "PERM"},
           {ITM_RCL,   "RCL"},
//           {ITM_CONVG,   "CONVG?"},
//           {ITM_ENTRY,   "ENTRY?"},
           {ITM_SQUARE,   "X^2"},
           {ITM_CUBE,   "X^3"},
           {ITM_YX,   "Y^X"},
           {ITM_SQUAREROOTX,   "SQRT"},
           {ITM_CUBEROOT,   "CUBRT"},
           {ITM_XTHROOT,   "XRTY"},
           {ITM_2X,   "2^X"},
           {ITM_EXP,   "E^X"},
           {ITM_10x,   "10^X"},
           {ITM_LOG2,   "LOG2"},
           {ITM_LN,   "LN"},
//           {ITM_STOP,   "STOP"},
           {ITM_LOG10,   "LOG10"},
           {ITM_LOGXY,   "LOGXY"},
           {ITM_1ONX,   "1/X"},
           {ITM_cos,   "COS"},
           {ITM_cosh,   "COSH"},
           {ITM_sin,   "SIN"},
//           {ITM_KEYQ,   "KEY?"},
           {ITM_sinh,   "SINH"},
           {ITM_tan,   "TAN"},
           {ITM_tanh,   "TANH"},
           {ITM_arccos,   "ARCCOS"},
           {ITM_arcosh,   "ARCCOSH"},
           {ITM_arcsin,   "ARCSIN"},
           {ITM_arsinh,   "ARCSINH"},
           {ITM_arctan,   "ARCTAN"},
           {ITM_artanh,   "ARCTANH"},
//           {ITM_CEIL,   "CEIL"},
//           {ITM_FLOOR,   "FLOOR"},
           {ITM_GCD,   "GCD"},
           {ITM_LCM,   "LCM"},
           {ITM_DEC,   "DEC"},
           {ITM_INC,   "INC"},
           {ITM_IP,   "IP"},
           {ITM_FP,   "FP"},
           {ITM_ADD,   "+"},
           {ITM_SUB,   "-"},
           {ITM_CHS,   "CHS"},
           {ITM_MULT,   "*"},
           {ITM_DIV,   "/"},
           {ITM_IDIV,   "IDIV"},
           {ITM_VIEW,   "VIEW"},
           {ITM_MOD,   "MOD"},
           {ITM_MAX,   "MAX"},
           {ITM_MIN,   "MIN"},
           {ITM_MAGNITUDE,   "ABS"},
//           {ITM_NEIGHB,   "NEIGHB"},
           {ITM_NEXTP,   "NEXTP"},
//           {ITM_XFACT,   "X!"},
           {ITM_CONSTpi,   "PI"},
//           {ITM_CF,   "CF"},
//           {ITM_SF,   "SF"},
//           {ITM_FF,   "FF"},
//           {ITM_M_SQR,   "M.SQR?"},
           {ITM_toDEG,   ">DEG"},
           {ITM_toDMS,   ">D.MS"},
           {ITM_toGRAD,   ">GRAD"},
           {ITM_toMULpi,   ">MULPI"},
           {ITM_toRAD,   ">RAD"},
           {ITM_DtoR,   "D>R"},
           {ITM_RtoD,   "R>D"},
//           {ITM_RMD,   "RMD"},
//           {ITM_LOGICALNOT,   "NOT"},
//           {ITM_LOGICALAND,   "AND"},
//           {ITM_LOGICALOR,   "OR"},
//           {ITM_LOGICALXOR,   "XOR"},
//           {ITM_Xex,   "X<>"},
           {CST_05,   "c"},
//           {CST_08,   "e"},
//           {CST_16,   "ge"},
//           {CST_18,   "gEARTH"},
//           {CST_65,   "mu0"},
           {CST_74,   "PHI"},
//           {CST_77,   "-INF"},
//           {CST_78,   "INF"},
//           {ITM_FCC,   "FC?C"},
//           {ITM_FCS,   "FC?S"},
//           {ITM_FCF,   "FC?F"},
//           {ITM_FSC,   "FS?C"},
//           {ITM_FSS,   "FS?S"},
//           {ITM_FSF,   "FS?F"},
//           {ITM_LOGICALNAND,   "NAND"},
//           {ITM_LOGICALNOR,   "NOR"},
//           {ITM_LOGICALXNOR,   "XNOR"},
//           {ITM_BS,   "BS?"},
//           {ITM_BC,   "BC?"},
//           {ITM_CB,   "CB"},
//           {ITM_SB,   "SB"},
//           {ITM_FB,   "FB"},
//           {ITM_RL,   "RL"},
//           {ITM_RLC,   "RLC"},
//           {ITM_RR,   "RR"},
//           {ITM_RRC,   "RRC"},
//           {ITM_SL,   "SL"},
//           {ITM_SR,   "SR"},
//           {ITM_ASR,   "ASR"},
//           {ITM_LJ,   "LJ"},
//           {ITM_RJ,   "RJ"},
//           {ITM_MASKL,   "MASKL"},
//           {ITM_MASKR,   "MASKR"},
//           {ITM_MIRROR,   "MIRROR"},
//           {ITM_NUMB,   "#B"},
//           {ITM_SDL,   "SDL"},
//           {ITM_SDR,   "SDR"},
           {ITM_SIGMAPLUS,   "SUM+"},
           {ITM_NSIGMA,   "NSUM"},
           {ITM_SIGMAx,   "SUMX"},
           {ITM_SIGMAy,   "SUMY"},
//           {ITM_SIGMAx2,   "SMX^2"},
//           {ITM_SIGMAx2y,   "SMX^2Y"},
//           {ITM_SIGMAy2,   "SMY^2"},
//           {ITM_SIGMAxy,   "SMXY"},
//           {ITM_SIGMAlnxy,   "SMLNXY"},
//           {ITM_SIGMAlnx,   "SMLNX"},
//           {ITM_SIGMAln2x,   "SMLN^2X"},
//           {ITM_SIGMAylnx,   "SMYLNX"},
//           {ITM_SIGMAlny,   "SMLNY"},
//           {ITM_SIGMAln2y,   "SMLN^2Y"},
//           {ITM_SIGMAxlny,   "SMXLNY"},
//           {ITM_SIGMAx2lny,   "SMX^2LNY"},
//           {ITM_SIGMAlnyonx,   "SMLNY/X"},
//           {ITM_SIGMAx2ony,   "SMX^2/Y"},
//           {ITM_SIGMA1onx,   "SM^1/X"},
//           {ITM_SIGMA1onx2,   "SM^1/X^2"},
//           {ITM_SIGMAxony,   "SMX/Y"},
//           {ITM_SIGMA1ony,   "SM^1/Y"},
//           {ITM_SIGMA1ony2,   "SM^1/Y^2"},
//           {ITM_SIGMAx3,   "SMX^3"},
//           {ITM_SIGMAx4,   "SMX^4"},
//           {SFL_FRACT,   "FRACT"},
//           {SFL_PROPFR,   "PROPFR"},
//           {SFL_DENANY,   "DENANY"},
//           {SFL_DENFIX,   "DENFIX"},
           {ITM_REG_X,   "X"},
           {ITM_REG_Y,   "Y"},
//           {ITM_REG_Z,   "Z"},
//           {ITM_REG_T,   "T"},
//           {ITM_REG_A,   "A"},
//           {ITM_REG_B,   "B"},
           {ITM_REG_C,   "C"},
//           {ITM_REG_D,   "D"},
//           {ITM_REG_L,   "L"},
//           {ITM_REG_I,   "I"},
//           {ITM_REG_J,   "J"},
//           {ITM_REG_K,   "K"},
           {ITM_INDIRECTION,   "IND>"},
           {ITM_Max,   "MAX"},
           {ITM_Min,   "MIN"},
           {ITM_EXPONENT,   "EEX"},
//           {ITM_BINOMP,   "BINOMP"},
//           {ITM_BINOM,   "BINOMGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_BINOMU,   "BINOMGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_BINOMM1,   "BINOM^MINUS_1"},
//           {ITM_CAUCHP,   "CAUCHP"},
//           {ITM_CAUCH,   "CAUCHGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_CAUCHU,   "CAUCHGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_CAUCHM1,   "CAUCH^MINUS_1"},
//           {ITM_EXPONP,   "EXPONP"},
//           {ITM_EXPON,   "EXPONGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_EXPONU,   "EXPONGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_EXPONM1,   "EXPON^MINUS_1"},
//           {ITM_FPX,   "FP(X)"},
//           {ITM_FX,   "FGAUSS_BLACK_LGAUSS_WHITE_R(X)"},
//           {ITM_FUX,   "FGAUSS_WHITE_LGAUSS_BLACK_R(X)"},
//           {ITM_FM1P,   "F^MINUS_1(P)"},
//           {ITM_GEOMP,   "GEOMP"},
//           {ITM_GEOM,   "GEOMGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_GEOMU,   "GEOMGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_GEOMM1,   "GEOM^MINUS_1"},
//           {ITM_HYPERP,   "HYPERP"},
//           {ITM_HYPER,   "HYPERGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_HYPERU,   "HYPERGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_HYPERM1,   "HYPER^MINUS_1"},
//           {ITM_LGNRMP,   "LGNRMP"},
//           {ITM_LGNRM,   "LGNRMGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_LGNRMU,   "LGNRMGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_LGNRMM1,   "LGNRM^MINUS_1"},
//           {ITM_LOGISP,   "LOGISP"},
//           {ITM_LOGIS,   "LOGISGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_LOGISU,   "LOGISGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_LOGISM1,   "LOGIS^MINUS_1"},
//           {ITM_NBINP,   "NBINP"},
//           {ITM_NBIN,   "NBINGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_NBINU,   "NBINGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_NBINM1,   "NBIN^MINUS_1"},
//           {ITM_NORMLP,   "NORMLP"},
//           {ITM_NORML,   "NORMLGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_NORMLU,   "NORMLGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_NORMLM1,   "NORML^MINUS_1"},
//           {ITM_POISSP,   "POISSP"},
//           {ITM_POISS,   "POISSGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_POISSU,   "POISSGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_POISSM1,   "POISS^MINUS_1"},
//           {ITM_TPX,   "TP(X)"},
//           {ITM_TX,   "TGAUSS_BLACK_LGAUSS_WHITE_R(X)"},
//           {ITM_TUX,   "TGAUSS_WHITE_LGAUSS_BLACK_R(X)"},
//           {ITM_TM1P,   "T^MINUS_1(P)"},
//           {ITM_WEIBLP,   "WEIBLP"},
//           {ITM_WEIBL,   "WEIBLGAUSS_BLACK_LGAUSS_WHITE_R"},
//           {ITM_WEIBLU,   "WEIBLGAUSS_WHITE_LGAUSS_BLACK_R"},
//           {ITM_WEIBLM1,   "WEIBL^MINUS_1"},
//           {ITM_chi2Px,   "CHI^2P(X)"},
//           {ITM_chi2x,   "CHI^2GAUSS_BLACK_LGAUSS_WHITE_R(X)"},
//           {ITM_chi2ux,   "CHI^2GAUSS_WHITE_LGAUSS_BLACK_R(X)"},
//           {ITM_chi2M1,   "(CHI^2)^MINUS_1"},
//           {ITM_BESTF,   "BESTF"},
//           {ITM_EXPF,   "EXPF"},
//           {ITM_LINF,   "LINF"},
//           {ITM_LOGF,   "LOGF"},
//           {ITM_ORTHOF,   "ORTHOF"},
//           {ITM_POWERF,   "POWERF"},
//           {ITM_GAUSSF,   "GAUSSF"},
//           {ITM_CAUCHF,   "CAUCHF"},
//           {ITM_PARABF,   "PARABF"},
//           {ITM_HYPF,   "HYPF"},
//           {ITM_ROOTF,   "ROOTF"},
//           {ITM_1COMPL,   "1COMPL"},
           {ITM_SNAP,   "SNAP"},
//           {ITM_2COMPL,   "2COMPL"},
           {ITM_ABS,   "ABS"},
//           {ITM_AGM,   "AGM"},
//           {ITM_AGRAPH,   "AGRAPH"},
           {ITM_ALL,   "ALL"},
//           {ITM_ASSIGN,   "ASN"},
//           {ITM_BACK,   "BACK"},
           {ITM_BATT,   "BATT?"},
//           {ITM_BEEP,   "BEEP"},
//           {ITM_BEGINP,   "BEGINP"},
//           {ITM_BN,   "BN"},
//           {ITM_BNS,   "BN^ASTERISK"},
           {ITM_CASE,   "CASE"},
//           {ITM_CLALL,   "CLALL"},
//           {ITM_CLCVAR,   "CLCVAR"},
//           {ITM_CLFALL,   "CLFALL"},
//           {ITM_CLLCD,   "CLLCD"},
//           {ITM_CLMENU,   "CLMENU"},
//           {ITM_CLP,   "CLP"},
//           {ITM_CLPALL,   "CLPALL"},
//           {ITM_CLREGS,   "CLREGS"},
           {ITM_CLSTK,   "CLSTK"},
           {ITM_CLSIGMA,   "CLSUM"},
//           {ITM_CONJ,   "CONJ"},
//           {ITM_CORR,   "CORR"},
//           {ITM_COV,   "COV"},
//           {ITM_BESTFQ,   "BESTF?"},
//           {ITM_CROSS_PROD,   "CROSS"},
//           {ITM_CXtoRE,   "CX>RE"},
//           {ITM_DATE,   "DATE"},
//           {ITM_DATEto,   "DATE>"},
//           {ITM_DAY,   "DAY"},
//           {ITM_DBLR,   "DBLR"},
//           {ITM_DBLMULT,   "DBLCROSS"},
//           {ITM_DBLDIV,   "DBL/"},
//           {ITM_DECOMP,   "DECOMP"},
           {ITM_DEG,   "DEG"},
//           {ITM_DEGto,   "DEG>"},
//           {ITM_SA,   "S(A)"},
//           {ITM_DENMAX,   "DENMAX"},
//           {ITM_DOT_PROD,   "DOT"},
//           {ITM_DSTACK,   "DSTACK"},
//           {ITM_DMS,   "D.MS"},
//           {ITM_DMSto,   "D.MS>"},
//           {ITM_DMY,   "D.MY"},
//           {ITM_DtoJ,   "D>J"},
//           {ITM_DELITM,   "DELITM"},
//           {ITM_EIGVAL,   "EIGVAL"},
//           {ITM_EIGVEC,   "EIGVEC"},
//           {ITM_ENDP,   "ENDP"},
           {ITM_ENG,   "ENG"},
//           {ITM_ENORM,   "ENORM"},
//           {ITM_EQ_DEL,   "EQ.DEL"},
//           {ITM_EQ_EDI,   "EQ.EDI"},
//           {ITM_EQ_NEW,   "EQ.NEW"},
//           {ITM_ERF,   "ERF"},
//           {ITM_ERFC,   "ERFC"},
//           {ITM_ERR,   "ERR"},
//           {ITM_EXITALL,   "EXITALL"},
           {ITM_EXPT,   "EXPT"},
//           {ITM_GET_JUL_GREG,   "J/G?"},
           {ITM_FIB,   "FIB"},
           {ITM_FIX,   "FIX"},
//           {ITM_FLASH,   "FLASH?"},
//           {ITM_FQX,   "F'(X)"},
//           {ITM_FDQX,   "F\(X)"},
//           {ITM_GAP,   "GAP"},
           {ITM_GD,   "GD"},
           {ITM_GDM1,   "GD^-1"},
//           {ITM_GRAD,   "GRAD"},
//           {ITM_GRADto,   "GRAD>"},
//           {ITM_GTOP,   "GTO."},
//           {ITM_HN,   "HN"},
//           {ITM_HNP,   "HNP"},
           {ITM_IM,   "IM"},
           {ITM_INDEX,   "INDEX"},
//           {ITM_IXYZ,   "IXYZ"},
//           {ITM_IGAMMAP,   "IGAMMAP"},
//           {ITM_IGAMMAQ,   "IGAMMAQ"},
           {ITM_IPLUS,   "I+"},
           {ITM_IMINUS,   "I-"},
//           {ITM_JYX,   "JY(X)"},
           {ITM_JPLUS,   "J+"},
           {ITM_JMINUS,   "J-"},
//           {ITM_JUL_GREG,   "J/G"},
//           {ITM_JtoD,   "J>D"},
//           {ITM_KEY,   "KEY"},
//           {ITM_KEYG,   "KEYG"},
//           {ITM_KEYX,   "KEYX"},
           {ITM_sinc,   "SINC"},
//           {ITM_KTYP,   "KTYP?"},
//           {ITM_LASTX,   "LASTX"},
//           {ITM_LEAP,   "LEAP?"},
//           {ITM_Lm,   "LM"},
//           {ITM_LmALPHA,   "LMALPHA"},
//           {ITM_LNBETA,   "LNBETA"},
//           {ITM_LNGAMMA,   "LNGAMMA"},
//           {ITM_LOAD,   "LOAD"},
//           {ITM_LOADP,   "LOADP"},
//           {ITM_LOADR,   "LOADR"},
//           {ITM_LOADSS,   "LOADSS"},
//           {ITM_LOADSIGMA,   "LOADSUM"},
//           {ITM_LocR,   "LOCR"},
//           {ITM_LocRQ,   "LOCR?"},
//           {ITM_LR,   "L.R."},
//           {ITM_MANT,   "MANT"},
//           {ITM_MATX,   "MAT_X"},
//           {ITM_MEM,   "MEM?"},
//           {ITM_MENU,   "MENU"},
//           {ITM_MONTH,   "MONTH"},
//           {ITM_MSG,   "MSG"},
           {ITM_MULPI,   "MULPI"},
//           {ITM_MVAR,   "MVAR"},
//           {ITM_M_DELR,   "M.DELR"},
//           {ITM_M_DIMQ,   "M.DIM?"},
//           {ITM_MDY,   "M.DY"},
//           {ITM_M_GET,   "M.GET"},
//           {ITM_M_GOTO,   "M.GOTO"},
//           {ITM_M_INSR,   "M.INSR"},
//           {ITM_M_LU,   "M.LU"},
           {ITM_M_NEW,   "M.NEW"},
//           {ITM_M_OLD,   "M.OLD"},
//           {ITM_M_PUT,   "M.PUT"},
//           {ITM_M_RR,   "M.R<>R"},
           {ITM_sincpi,   "SINCPI"},
//           {ITM_NOP,   "NOP"},
//           {ITM_OFF,   "OFF"},
//           {ITM_DROPY,   "DROPY"},
//           {ITM_PGMINT,   "PGMINT"},
//           {ITM_PGMSLV,   "PGMSLV"},
//           {ITM_PIXEL,   "PIXEL"},
           {ITM_PLOT,   "PLOT"},
//           {ITM_Pn,   "PN"},
//           {ITM_POINT,   "POINT"},
//           {ITM_LOADV,   "LOADV"},
//           {ITM_POPLR,   "POPLR"},
//           {ITM_PRCL,   "PRCL"},
//           {ITM_PSTO,   "PSTO"},
//           {ITM_PUTK,   "PUTK"},
           {ITM_RAD,   "RAD"},
//           {ITM_RADto,   "RAD>"},
           {ITM_RAN,   "RAN#"},
//           {ITM_RBR,   "REGS.V"},
//           {ITM_RCLCFG,   "RCLCFG"},
           {ITM_RCLEL,   "RCLEL"},
//           {ITM_RCLIJ,   "RCLIJ"},
//           {ITM_RCLS,   "RCLS"},
//           {ITM_RDP,   "RDP"},
           {ITM_RE,   "RE"},
//           {ITM_RECV,   "RECV"},
//           {ITM_RESET,   "RESET"},
//           {ITM_REtoCX,   "RE>CX"},
           {ITM_REexIM,   "RE<>IM"},
//           {ITM_RM,   "RMODE"},
//           {ITM_RMQ,   "RMODE?"},
//           {ITM_RMODE,   "RMODE"},
//           {ITM_RMODEQ,   "RMODE?"},
//           {ITM_DSP,   "DSP"},
//           {ITM_RNORM,   "RNORM"},
           {ITM_EX1,   "E^X-1"},
//           {ITM_RSD,   "RSD"},
//           {ITM_RSUM,   "RSUM"},
//           {ITM_R_CLR,   "R-CLR"},
//           {ITM_R_COPY,   "R-COPY"},
//           {ITM_R_SORT,   "R-SORT"},
//           {ITM_R_SWAP,   "R-SWAP"},
//           {ITM_am,   "PSI(U,M)"},
//           {ITM_STDDEVWEIGHTED,   "S"},
//           {ITM_SAVE,   "SAVE"},
           {ITM_SCI,   "SCI"},
//           {ITM_SDIGS,   "SDIGS?"},
//           {ITM_SEED,   "SEED"},
//           {ITM_SEND,   "SEND"},
//           {ITM_SETCHN,   "SETCHN"},
//           {ITM_SETDAT,   "SETDAT"},
//           {ITM_SETEUR,   "SETEUR"},
//           {ITM_SETIND,   "SETIND"},
//           {ITM_SETJPN,   "SETJPN"},
//           {ITM_SETSIG,   "SETSIG"},
//           {ITM_SETTIM,   "SETTIM"},
//           {ITM_SETUK,   "SETUK"},
//           {ITM_SETUSA,   "SETUSA"},
//           {ITM_SIGN,   "SIGN"},
//           {ITM_SIGNMT,   "SIGNMT"},
//           {ITM_SIM_EQ,   "SIM_EQ"},
//           {ITM_SKIP,   "SKIP"},
//           {ITM_SLVQ,   "SLVQ"},
//           {ITM_SM,   "SM"},
//           {ITM_ISM,   "ISM?"},
//           {ITM_SMW,   "SMW"},
//           {ITM_SOLVE,   "SOLVE"},
//           {ITM_SSIZE,   "SSIZE?"},
//           {ITM_STATUS,   "STATUS"},
//           {ITM_STOCFG,   "STOCFG"},
           {ITM_STOEL,   "STOEL"},
           {ITM_STOIJ,   "STOIJ"},
           {ITM_LN1X,   "LN(1+X)"},
//           {ITM_STOS,   "STOS"},
//           {ITM_SUM,   "SUM"},
//           {ITM_SW,   "SW"},
//           {ITM_SXY,   "SXY"},
//           {ITM_TDISP,   "TDISP"},
           {ITM_TICKS,   "TICKS"},
//           {ITM_TIME,   "TIME"},
//           {ITM_TIMER,   "TIMER"},
//           {ITM_Tn,   "TN"},
//           {ITM_TONE,   "TONE"},
//           {ITM_Tex,   "T<>"},
//           {ITM_ULP,   "ULP?"},
//           {ITM_Un,   "UN"},
//           {ITM_UNITV,   "UNITV"},
//           {ITM_UNSIGN,   "UNSIGN"},
//           {ITM_VARMNU,   "VARMNU"},
//           {ITM_VERS,   "VERS?"},
//           {ITM_IDIVR,   "IDIVR"},
//           {ITM_WDAY,   "WDAY"},
//           {ITM_WHO,   "WHO?"},
//           {ITM_WM,   "WM"},
//           {ITM_WP,   "WP"},
//           {ITM_WM1,   "W^MINUS_1"},
//           {ITM_WSIZE,   "WSIZE"},
//           {ITM_WSIZEQ,   "WSIZE?"},
//           {ITM_XBAR,   "X_MEAN"},
//           {ITM_XG,   "X_GEO"},
//           {ITM_XW,   "X_WTD"},
//           {ITM_XCIRC,   "X_CIRC"},
//           {ITM_XtoDATE,   "X>DATE"},
//           {ITM_XtoALPHA,   "X>ALPHA"},
//           {ITM_M_QR,   "M.QR"},
//           {ITM_YEAR,   "YEAR"},
//           {ITM_YCIRC,   "Y_CIRC"},
//           {ITM_YMD,   "Y.MD"},
//           {ITM_Yex,   "Y<>"},
//           {ITM_Zex,   "Z<>"},
//           {ITM_XMAX,   "XMAX"},
//           {ITM_XMIN,   "XMIN"},
//           {ITM_BETAXY,   "BETA"},
//           {ITM_gammaXY,   "GAMMAXY"},
//           {ITM_GAMMAXY,   "GAMMAXY"},
//           {ITM_GAMMAX,   "GAMMA"},
//           {ITM_YYX,   "YY(X)"},
//           {ITM_DELTAPC,   "DELTA%"},
//           {ITM_SCATTFACT,   "EPSILON"},
//           {ITM_SCATTFACTm,   "EPSILONM"},
//           {ITM_SCATTFACTp,   "EPSILONP"},
//           {ITM_zetaX,   "ZETA(X)"},
//           {ITM_PIn,   "PIN"},
//           {ITM_SIGMAn,   "SUMN"},
//           {ITM_STDDEV,   "SUM"},
//           {ITM_STDDEVPOP,   "SUMW"},
//           {ITM_RANI,   "RANI#"},
//           {ITM_PRINTERX,   "PRINTERX"},
//           {ITM_RANGE,   "RANGE"},
//           {ITM_GETRANGE,   "RANGE?"},
           {ITM_M1X,   "(-1)^X"},
//           {ITM_XMOD,   "CROSSMOD"},
//           {ITM_toDATE,   ">DATE"},
//           {ITM_sn,   "SN(U,M)"},
//           {ITM_cn,   "CN(U,M)"},
//           {ITM_dn,   "DN(U,M)"},
//           {ITM_toHR,   ">HR"},
//           {ITM_toHMS,   ">H.MS"},
//           {ITM_toINT,   ">INT"},
//           {ITM_MPItoR,   "MPI>R"},
//           {ITM_RtoMPI,   "R>MPI"},
           {ITM_toREAL,   ">REAL"},
//           {ITM_DtoDMS,   "D>D.MS"},
//           {ITM_SHUFFLE,   "<>"},
//           {ITM_PC,   "%"},
//           {ITM_PCMRR,   "%MRR"},
//           {ITM_PCT,   "%T"},
//           {ITM_PCSIGMA,   "%SUM"},
//           {ITM_PCPMG,   "%+MG"},
//           {ITM_INTEGRAL,   "INTEGRAL"},
//           {ITM_PMOD,   "^MOD"},
//           {ITM_M_DET,   "|M|"},
//           {ITM_PARALLEL,   "PARL"},
//           {ITM_M_TRANSP,   "[M]^T"},
//           {ITM_M_INV,   "[M]^MINUS_1"},
//           {ITM_MULPIto,   "MULPI>"},
//           {ITM_PRINTERADV,   "PRINTERADV"},
//           {ITM_PRINTERCHAR,   "PRINTERCHAR"},
//           {ITM_PRINTERDLAY,   "PRINTERDLAY"},
//           {ITM_PRINTERLCD,   "PRINTERLCD"},
//           {ITM_PRINTERMODE,   "PRINTERMODE"},
//           {ITM_PRINTERPROG,   "PRINTERPROG"},
//           {ITM_PRINTERR,   "PRINTERR"},
//           {ITM_PRINTERREGS,   "PRINTERREGS"},
//           {ITM_PRINTERSTK,   "PRINTERSTK"},
//           {ITM_PRINTERTAB,   "PRINTERTAB"},
//           {ITM_PRINTERUSER,   "PRINTERUSER"},
//           {ITM_PRINTERWIDTH,   "PRINTERWIDTH"},
//           {ITM_PRINTERSIGMA,   "PRINTERSUM"},
//           {ITM_PRINTERHASH,   "PRINTER#"},
//           {ITM_FBR,   "FBR"},
//           {ITM_Kk,   "K(M)"},
//           {ITM_Ek,   "E(M)"},
//           {ITM_PInk,   "PI(N,M)"},
           {ITM_EXIT1,   "EXIT"},
//           {ITM_ANGLE,   "ANGLE"},
           {ITM_AIM,   "ALPHA"},
           {ITM_dotD,   "DOTD"},
//           {ITM_SHOW,   "SHOW"},
//           {ITM_SYSTEM,   "SYSTEM"},
//           {ITM_DMStoD,   "D.MS>D"},
//           {ITM_XH,   "X_HARM"},
//           {ITM_XRMS,   "X_RMS"},
//           {ITM_DET,   "DET"},
//           {ITM_INVRT,   "INVRT"},
//           {ITM_TRANS,   "TRANS"},
//           {ITM_Fphik,   "F(PHI,M)"},
//           {ITM_Ephik,   "E(PHI,M)"},
//           {ITM_ZETAphik,   "ZETA(PHI,M)"},
//           {ITM_EE_D2Y,   "D>Y"},
//           {ITM_EE_Y2D,   "Y>D"},
//           {ITM_EE_A2S,   "ATOSYM"},
//           {ITM_EE_S2A,   "SYMTOA"},
//           {ITM_EE_EXP_TH,   "E^THETAJ"},
//           {ITM_EE_STO_Z,   "STO3Z"},
//           {ITM_EE_RCL_Z,   "RCL3Z"},
//           {ITM_EE_STO_V,   "STO3V"},
//           {ITM_EE_RCL_V,   "RCL3V"},
//           {ITM_EE_STO_I,   "STO3I"},
//           {ITM_EE_RCL_I,   "RCL3I"},
//           {ITM_EE_STO_V_I,   "3V/3I"},
//           {ITM_EE_STO_IR,   "3Ix3Z"},
//           {ITM_EE_STO_V_Z,   "3V/3Z"},
//           {ITM_EE_X2BAL,   "X>BAL"},
//           {ITM_MATX_A,   "M.A"},
//           {ITM_op_a,   "OP_A"},
//           {ITM_op_a2,   "OP_A^2"},
//           {ITM_op_j,   "OP_J"},
//           {ITM_2BIN,   ">BIN"},
//           {ITM_2OCT,   ">OCT"},
//           {ITM_2DEC,   ">DEC"},
//           {ITM_2HEX,   ">HEX"},
//           {ITM_HR_DEG,   "HOUR"},
           {ITM_MINUTE,   "MIN"},
//           {ITM_SECOND,   "SEC"},
//           {ITM_toTIME,   ">TIME"},
//           {ITM_TIMEto,   "TIME>"},
           {KEY_COMPLEX,   "COMPLEX"},
           {ITM_toPOL2,   ">POLAR"},
           {ITM_toREC2,   ">RECT"},
           {ITM_eRPN_ON,   "ERPN"},
           {ITM_eRPN_OFF,   "RPN"},
           {ITM_SIGFIG,   "SIG"},
//           {ITM_UNIT,   "UNIT"},
           {ITM_ROUND2,   "ROUND"},
           {ITM_ROUNDI2,   "ROUNDI"},
//           {ITM_RI,   ">I"},
//           {ITM_DRG,   "DRG"},
           {CHR_caseUP,   "CASEUP"},
           {CHR_caseDN,   "CASEDN"},
           {ITM_LISTXY,   "LISTXY"},
//           {ITM_SH_ERPN,   "ERPN?"},
//           {ITM_XXEQ,   "X.XEQ"},
           {ITM_STKTO3x1,   "ZYX>M"},
//           {ITM_FLGSV,   "FLAGS.V"},
//           {ITM_CPXI,   "CPXI"},
//           {ITM_CPXJ,   "CPXJ"},
//           {ITM_SSIZE4,   "SSIZE4"},
//           {ITM_SSIZE8,   "SSIZE8"},
           {ITM_POLAR,   "POLAR"},
           {ITM_RECT,   "RECT"},
//           {ITM_ms,   ".MS"},
//           {ITM_DEG2,   ">>DEG"},
//           {ITM_DMS2,   ">>D.MS"},
//           {ITM_GRAD2,   ">>GRAD"},
//           {ITM_MULPI2,   ">>MULPI"},
//           {ITM_RAD2,   ">>RAD"},
//           {ITM_HMS2,   ">>H.MS"},
//           {ITM_X_P1,   "XEQM01"},
//           {ITM_X_P2,   "XEQM02"},
//           {ITM_X_P3,   "XEQM03"},
//           {ITM_X_P4,   "XEQM04"},
//           {ITM_X_P5,   "XEQM05"},
//           {ITM_X_P6,   "XEQM06"},
//           {ITM_X_f1,   "XEQM07"},
//           {ITM_X_f2,   "XEQM08"},
//           {ITM_X_f3,   "XEQM09"},
//           {ITM_X_f4,   "XEQM10"},
//           {ITM_X_f5,   "XEQM11"},
//           {ITM_X_f6,   "XEQM12"},
//           {ITM_X_g1,   "XEQM13"},
//           {ITM_X_g2,   "XEQM14"},
//           {ITM_X_g3,   "XEQM15"},
//           {ITM_X_g4,   "XEQM16"},
//           {ITM_X_g5,   "XEQM17"},
//           {ITM_X_g6,   "XEQM18"},
//           {ITM_XSAVE,   "X.SAVE"},
//           {ITM_XLOAD,   "X.LOAD"},
//           {ITM_CLAIM,   "EXITCLR"},
           {ITM_INTG,   "P_INT"},
           {ITM_DIFF,   "P_DIFF"},
           {ITM_RMS,   "P_RMS"},
           {ITM_SHADE,   "P_SHADE"},
           {ITM_CLGRF,   "CLGRF"},
           {ITM_PLOT_STAT,   "PLSTAT"},
           {ITM_3x1TOSTK,   "M>ZYX"},
           {ITM_PLOTRST,   "PLTRST"},
//           {ITM_T_EXPF,   "EXPF"},
//           {ITM_T_LINF,   "LINF"},
//           {ITM_T_LOGF,   "LOGF"},
//           {ITM_T_ORTHOF,   "ORTHOF"},
//           {ITM_T_POWERF,   "POWERF"},
//           {ITM_T_GAUSSF,   "GAUSSF"},
//           {ITM_T_CAUCHF,   "CAUCHF"},
//           {ITM_T_PARABF,   "PARABF"},
//           {ITM_T_HYPF,   "HYPF"},
//           {ITM_T_ROOTF,   "ROOTF"},
//           {ITM_RSTF,   "RESETF"},


#endif //SAVE_SPACE_DM42_2

};


//SPECIAL CODE TODO FOR XEQMnn
//                      if (strcompare(str,"XEQM01" ) && exec) { *com = ITM_X_P1;} else
//                      if (strcompare(str,"XEQM02" ) && exec) { *com = ITM_X_P2;} else
//                      if (strcompare(str,"XEQM03" ) && exec) { *com = ITM_X_P3;} else
//                      if (strcompare(str,"XEQM04" ) && exec) { *com = ITM_X_P4;} else
//                      if (strcompare(str,"XEQM05" ) && exec) { *com = ITM_X_P5;} else
//                      if (strcompare(str,"XEQM06" ) && exec) { *com = ITM_X_P6;} else
//                      if (strcompare(str,"XEQM07" ) && exec) { *com = ITM_X_f1;} else
//                      if (strcompare(str,"XEQM08" ) && exec) { *com = ITM_X_f2;} else
//                      if (strcompare(str,"XEQM09" ) && exec) { *com = ITM_X_f3;} else
//                      if (strcompare(str,"XEQM10" ) && exec) { *com = ITM_X_f4;} else
//                      if (strcompare(str,"XEQM11" ) && exec) { *com = ITM_X_f5;} else
//                      if (strcompare(str,"XEQM12" ) && exec) { *com = ITM_X_f6;} else
//                      if (strcompare(str,"XEQM13" ) && exec) { *com = ITM_X_g1;} else
//                      if (strcompare(str,"XEQM14" ) && exec) { *com = ITM_X_g2;} else
//                      if (strcompare(str,"XEQM15" ) && exec) { *com = ITM_X_g3;} else
//                      if (strcompare(str,"XEQM16" ) && exec) { *com = ITM_X_g4;} else
//                      if (strcompare(str,"XEQM17" ) && exec) { *com = ITM_X_g5;} else
//                      if (strcompare(str,"XEQM18" ) && exec) { *com = ITM_X_g6;} else

#ifndef SAVE_SPACE_DM42_2
  bool_t checkindexes(int16_t *com, char *str, bool_t exec) {
    *com = 0;
    uint_fast16_t n = nbrOfElements(indexOfFunctions);
    for (uint_fast16_t i = 0; i < n; i++) {
      if (strcompare(str, indexOfFunctions[i].itemName)) {
        *com = indexOfFunctions[i].itemNr;
        break;
      }
    }

    if (*com != 0) {
      return true;
    }
    else {
      return false;
    }
  }
#endif //SAVE_SPACE_DM42_2


void execute_string(const char *inputstring, bool_t exec1, bool_t namescan) {
#ifndef SAVE_SPACE_DM42_2
  #ifndef TESTSUITE_BUILD
    #if (VERBOSE_LEVEL > 0)
      uint32_t ttt = getUptimeMs();
      while(ttt + 300 != getUptimeMs()){};
      print_linestr(inputstring,true);
      print_linestr("",false);
    #endif

        if(exec1) namescan = false;     //no scanning option if tasked to execute
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


        hourGlassIconEnabled = true;
        cancelFilename = true;
        showHideHourGlass();
        #ifdef DMCP_BUILD
          lcd_refresh();
        #else // !DMCP_BUILD
          refreshLcd(NULL);
        #endif // DMCP_BUILD

                                                   //If !gotlabels, means it is a scouting pass/parse to find and mark the goto labels M1-M4
      while(!gotlabels || (gotlabels && exec) ){   //scheme to use for label scouting and name processing in "false", and to do a two parse exec 
        #ifdef PC_BUILD_VERBOSE1
          #ifdef PC_BUILD
            printf("\n------Starting parse ------- Indexes: M1:%d M2:%d M3:%d M4:%d   EXEC:%d\n",ix_m1, ix_m2, ix_m3, ix_m4, exec);
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

        while (inputstring[ix]!=0 && ix < XEQ_STR_LENGTH_LONG)

      {

  //			if( (inputstring[ix]==13 || inputstring[ix]==10) && (aa[0]==13 || aa[0]==10) ) {
  	//			ix++;
  		//	} else 
  			{
          strcpy(bb,aa);
          aa[0] = inputstring[ix];
          aa[1] = 0;
          #ifdef PC_BUILD_VERBOSE0
            #ifdef PC_BUILD
              printf("##--$ |%s|%s| --gotlabels=%i exec=%i \n",bb,aa,gotlabels,exec);
            #endif
          #endif
  //print_linestr(aa,false);
          switch(bb[0]) {//COMMAND can start after any of these: space, tab, cr, lf, comma, beginning of file
            case 32:
            case 8 :
            case 13:
            case 10:
            case 44:
            case 0 : if( //COMMAND WORD START DETECTION +-*/ 0-9; A-Z; %; >; (; a-z; . 
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
             #ifdef PC_BUILD_VERBOSE0
               #ifdef PC_BUILD
                 printf("##++ Cancel Comments\n");
               #endif
             #endif
            state_comments=false;
          } else
          switch(aa[0]) {
            case '/': if(bb[0] == '/' && state_comments == false) {//ADDED  STATE, SO //always switches on the comment, but not off. CR/LF cancels it
                        state_comments = true;           // Switch comment state on
                        state_commands = false;
                        state_quotes   = false;
                        commandnumber[0]=0;
                        #ifdef PC_BUILD_VERBOSE0
                          #ifdef PC_BUILD
                            printf("##++ Start Comments\n");
                          #endif
                        #endif
                      }
                       break;
            case 34: if(!state_comments && !state_commands) {   // " Toggle quote state
                         state_quotes   = !state_quotes;
                       }
                       break;

            case 13: //cr
            case 10: //lf
            case 8 : //tab
            case ',': //,
            case ' ': 
  //print_linestr(commandnumber,false);
                     #ifdef PC_BUILD_VERBOSE0
                       #ifdef PC_BUILD
                         printf("@@@ %s\n",commandnumber);
                       #endif
                     #endif
                     if(state_commands){
                        state_commands = false;                // Waiting for delimiter to close off and send command number: nnn<                 
                        #ifdef PC_BUILD_VERBOSE0
                          #ifdef PC_BUILD
                            printf("\nCommand/number detected:(tempjm=%d)(gotoinprogress=%d) %45s ",temporaryInformation,gotoinprogress,commandnumber);
                          #endif
                        #endif
  //print_linestr(commandnumber,false);
                        
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

                        if(checkindexes(&commno, commandnumber, exec)) {
                        	sprintf(commandnumber,"%d", commno);
  				                #ifdef PC_BUILD_VERBOSE0
  				                  #ifdef PC_BUILD
                              printf("## no:%i",commno);
  				                  #endif
  				                #endif
                        } else
                        if (strcompare(commandnumber,"DSZ"    )) {sprintf(commandnumber,"%d", ITM_DEC); gotoinprogress = 9;}      else //EXPECTING FOLLOWING OPERAND "nn"
                         if (strcompare(commandnumber,"ISZ"   )) {sprintf(commandnumber,"%d", ITM_INC); gotoinprogress = 9;}       else //EXPECTING FOLLOWING OPERAND "nn"
                          if (strcompare(commandnumber,"LBL"))       {xeqlblinprogress = 10; }                                      else //EXPECTING FOLLOWING OPERAND Mn
                            if (strcompare(commandnumber,"XEQC43"))   {starttoken = 1; }                                             else //EXPECTING FOLLOWING OPERAND Mn
                             if (strcompare(commandnumber,"XEQLBL"))    {
                                 #if (VERBOSE_LEVEL > 0)
                                   print_linestr("->XEQLBL",false);
                                 #endif
                                 xeqlblinprogress =  1; 
                                 starttoken = 1;
                              } else //EXPECTING 2 OPERANDS nn XXXXXX
                               if (strcompare(commandnumber,"GTO"   ))    {
                                    #ifdef PC_BUILD
                                      printf("   >>> Loop GTO Jump %d:go\n",loopnumber++);
                                    #endif
  	                                if(exec) {
  	                              	  go = true; 
  	                              	  gotoinprogress = 1;
  	                                  force_refresh();
  	                                }
                              } else
                               if (strcompare(commandnumber,"GSB"   ))    {
                                    #ifdef PC_BUILD
                                      printf("   >>> Sub  GSB Jump %d\n",loopnumber++);
                                    #endif
                                    if(exec) {
                                      go = true; 
                                      gotoinprogress = 1; 
                                      ix_m = ix;
                                      force_refresh();
                                      #ifdef PC_BUILD
                                        printf("   >>> Sub  GSB Jump %d:go storing return address %d\n",loopnumber++, ix_m);
                                      #endif
                                    }
                               } else
                                if (strcompare(commandnumber,"RTN"))       {
                                    if(exec) {
                                      ix = ix_m+2; 
                                      ix_m = 0;
                                      force_refresh();
                                      #ifdef PC_BUILD
                                        printf("   >>> Sub  RTN to return address %d\n", ix);
                                      #endif
                                    }
                               } else
                                 if (strcompare(commandnumber,"GTO_SZ"))    {
                                     if(exec) {
                                       go = (temporaryInformation == TI_FALSE); 
                                       gotoinprogress = 1; 
                                     }
                                 } else
                                   if (strcompare(commandnumber,"END"))       {
                                      ix = stringByteLength(inputstring)-2;
                                    } else
                                      if (strcompare(commandnumber,"RETURN"))    {
                                          if(exec) {
                                            ix = stringByteLength(inputstring)-2;
                                          }
                                      } else
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
                          goto exec_exit;
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
                              if(!exec) {
                                strcpy(indexOfItemsXEQM + (no-1)*8, tmpp);        // At Exec time, the XEQM label is changed to the command number. So the re-allocation of the name can only happen in the !exec state
                                if(namescan) {
                                  //printf("\n### Namescan end %s\n",tmpp);
                                  goto exec_exit; //If in name scan mode, only need to process string up to here
                                }
                              }
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
                            flipPixel(indic_x, indic_y-1);
                          break;
  //HERE DEFAULT !!
  //NOT IN PROGRESS                        
                          default:                 //NOT IN PROGRESS
                            no = atoi(commandnumber);       //Will force all unknown commands to have no number, and invalid command and RETURN MARK etc. to 0
                            //printf("   Command send %s EXEC=%d no=%d ",commandnumber,exec,no);
                            if(no > LAST_ITEM-1) {no = 0;}
                            if(no!=0 && exec) {
                              flipPixel(indic_x++, indic_y);
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
              //printf("#%s",aa);
            }   // accumulate string
          }
          ix++;
        }

      } //while

      gotlabels = true;                              //allow to run only once, unless
      if(!exec) exec = exec1; else exec = false;     //exec must run, and ensure it runs only once.
    }
    #ifdef PC_BUILD_VERBOSE0
      #ifdef PC_BUILD
        printf(">>>end gotlabels=%i exec=%i \n",gotlabels,exec);
      #endif
    #endif

    exec_exit:
    exec = false;     //exec must run, and ensure it runs only once.
  #endif //TESTSUITE_BUILD
  	running_program_jm = false;
  	#ifdef PC_BUILD_VERBOSE0
  	  #ifdef PC_BUILD
  		   	printf("##++ END Exiting\n");
  	  #endif
  	#endif
  	return;                              
#endif //SAVE_SPACE_DM42_2
}


 
void replaceFF(char* FF, char* line2) {
  int16_t ix =0;
  if(FF[0]>='0' && FF[0]<='9' && FF[1]>='0' && FF[1]<='9' && FF[2]==0) {
    while(line2[ix] != 0 && ix+10<stringByteLength(line2)) {
      if(line2[ix]==88 /*X*/ && line2[ix+1]==69 /*E*/ && line2[ix+2]==81 /*Q*/ && line2[ix+3]==76 /*L*/ && line2[ix+4]==66 /*B*/ && line2[ix+5]==76 /*L*/ && line2[ix+6]==32 && line2[ix+7]==70 /*F*/ && line2[ix+8]==70 /*F*/) {
        line2[ix+7]=FF[0];
        line2[ix+8]=FF[1];
      }
      ix++;
    }
  }
}

void XEQMENU_Selection(uint16_t selection, char *line1, bool_t exec, bool_t scanning) {
#ifndef SAVE_SPACE_DM42_2
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


#ifdef DMCP_BUILD
  #define pgmpath "PROGRAMS"
#else
  #define pgmpath "res/PROGRAMS"
#endif
  strcpy(fn_short,"XEQMINDEX.TXT");
  strcpy(fn_long, "");
  strcpy(fallback,"XEQM01:HELP;");

  #if (VERBOSE_LEVEL >= 1) 
    strcpy(tmp,fn_short); 
    strcat(tmp," A: Loading XEQMENU mapping"); 
    print_linestr(tmp,false);
  #endif

  import_string_from_filename(line1,pgmpath,fn_short,fn_long,fallback,!SCAN);

  #if (VERBOSE_LEVEL >= 1) 
   sprintf(tmp, " B: XEQMENU mapping Loaded: %u bytes.\n",(uint16_t)stringByteLength(line1) );
   print_linestr(tmp,false);
  #endif

  #if (VERBOSE_LEVEL >= 2)
    #ifdef DMCP_BUILD
      press_key();
    #endif
  #endif

  int16_t ix = 0;
  int16_t iy = 0;
  sprintf(nn,"%2d",selection);                   //Create string value for 00
  if(nn[0]==' ') nn[0]='0';
  if(nn[1]==' ') nn[1]='0';
  strcpy(fn_short,"XEQM");                        //Build default short file name XEQMnn
  strcat(fn_short,nn);
  strcpy(fn_long,fn_short);

                                            //Find XEQMnn in the replacement token file         
  while(line1[ix] != 0 && ix+6<stringByteLength(line1)) {
     if(line1[ix]=='X' && line1[ix+1]=='E' && line1[ix+2]=='Q' && line1[ix+3]=='M' && line1[ix+4]==nn[0] && line1[ix+5]==nn[1] && line1[ix+6]==':') {
       ix = ix + 7;
       iy = ix;                             //If found, find the replacement text after the colon until before the semi-colon
       while(line1[ix] != 0 && ix<stringByteLength(line1)) {
          if(line1[ix] == ';' ) {line1[ix]=0; strcpy(fn_long,line1 + iy); break;}     //Replace file name with content from replacement string
          ix++;
       }
     } 
     ix++;
  }
  strcat(fn_short,".TXT");                        //Add .TXT
  strcat(fn_long,".TXT");                         //Add .TXT

  #if (VERBOSE_LEVEL >= 1) 
    sprintf(tmp," C: Trying %s then %s.",fn_short,fn_long);
    print_linestr(tmp,false);
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
    sprintf(tmp,"  Fallback:%s",fallback); print_linestr(tmp,false);
  #endif

  import_string_from_filename(line1,pgmpath,fn_short,fn_long,fallback,scanning); 

  #if (VERBOSE_LEVEL >= 1) 
   sprintf(tmp, " D: PGM Loaded: %u bytes.\n",(uint16_t)stringByteLength(line1) );
   print_linestr(tmp,false);
  #endif

  replaceFF(nn,line1); 

  #if (VERBOSE_LEVEL >= 1) 
   sprintf(tmp, " E: FF: %u bytes.\n",(uint16_t)stringByteLength(line1) );
   print_linestr(tmp,false);
   print_linestr(line1,false);
  #endif


  #if (VERBOSE_LEVEL >= 2) 
    #ifdef DMCP_BUILD
      press_key();
    #endif
  #endif


  #if (VERBOSE_LEVEL >= 1) 
    clearScreen_old(false, true, true);
  #endif

  displaywords(line1);       //output  is  in  tmpString

  strcpy(line1,tmpString);  
  #if (VERBOSE_LEVEL >= 2) 
    #ifdef DMCP_BUILD
      press_key();
    #endif
  #endif

  #if (VERBOSE_LEVEL >= 1) 
    clearScreen_old(false, true, true);
  #endif

  execute_string(line1,exec, scanning);

  #if (VERBOSE_LEVEL >= 2) 
    #ifdef DMCP_BUILD
      press_key();
      clearScreen_old(false, true, true);
    #endif
  #endif

#endif
#endif //SAVE_SPACE_DM42_2
}



void fnXEQMENU(uint16_t XEQM_no) {
  #ifndef TESTSUITE_BUILD
  clearScreen_old(false, true, true);
  print_linestr("Loading XEQM program file:",true);

  char line[XEQ_STR_LENGTH_LONG];
  XEQMENU_Selection( XEQM_no, line, EXEC, !SCAN);

  //calcMode = CM_BUG_ON_SCREEN;
  //temporaryInformation = TI_NO_INFO;
  #endif
}                                               // DOES NOT RETURN TO ##C in items.



void XEQMENU_loadAllfromdisk(void) {
#ifndef SAVE_SPACE_DM42_2
#ifndef TESTSUITE_BUILD
//uint16_t Delay;
      clearScreen_old(false, true, true);
      print_inlinestr("",true);
      print_inlinestr("Loading XEQM:",false);

      char line[XEQ_STR_LENGTH_LONG];
      
      char tmp[2];
      tmp[1]=0;
      uint8_t ix = 1;
      while(ix<=18) {
        tmp[0]=48+ix+ (ix > 9 ? 65-48-10 : 0);
        print_inlinestr(tmp,false);
        XEQMENU_Selection( ix, line, !EXEC, SCAN);
        ix++;
      }
    #endif
#endif //SAVE_SPACE_DM42_2
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
#ifndef SAVE_SPACE_DM42_2
  char tt[40]; 
  if(getRegisterDataType(REGISTER_X) == dtString) {
    xcopy(tmpString + TMP_STR_LENGTH/2, REGISTER_STRING_DATA(REGISTER_X), stringByteLength(REGISTER_STRING_DATA(REGISTER_X))+1);
    tt[0]=0;

    sprintf(tt,"XEQM%02u.TXT",XEQM_no);

    #ifdef PC_BUILD
    printf(">>> string ready  ## %s:%s\n",tt,tmpString + TMP_STR_LENGTH/2);
    //uint16_t ix = 0;while (ix!=20) {printf("%d:%d=\n",ix,tmpString[ix]);ix++;}
    stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
    printf(">>> string in utf ## %s:%s\n",tt, tmpString);
    //ix = 0;while (ix!=20) {printf("%d:%d=\n",ix,ll[ix]);ix++;}
    #endif

    #ifndef TESTSUITE_BUILD
      stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
      if(tt[0]!=0) export_string_to_filename(tmpString, OVERWRITE, "res/PROGRAMS", tt);
    #endif
  }
#endif //SAVE_SPACE_DM42_2
}


void fnXEQMLOAD (uint16_t XEQM_no) {                                  //DISK to X-REGISTER
#ifndef SAVE_SPACE_DM42_2
  #ifdef PC_BUILD
    printf("LOAD %d\n",XEQM_no);
  #endif
  char line1[XEQ_STR_LENGTH_LONG];
  line1[0]=0;
  XEQMENU_Selection(XEQM_no, line1, !EXEC, !SCAN);
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
  reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), amNone);
  strcpy(REGISTER_STRING_DATA(REGISTER_X),line1 + TMP_STR_LENGTH/2);
#endif //SAVE_SPACE_DM42_2
}




void fnXEQMEDIT (uint16_t unusedButMandatoryParameter) {
          if(aimBuffer[0] != 0) {          //JM if something already in the AIMB|UFFER when X-EDIT is called, store this in the stack first
            setSystemFlag(FLAG_ASLIFT);
            liftStack();
            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
            copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_Y);

            int16_t len = stringByteLength(aimBuffer) + 1;
            reallocateRegister(REGISTER_Z, dtString, TO_BLOCKS(len), amNone);
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
  if(calcMode == CM_AIM && (getRegisterDataType(REGISTER_Y) == dtReal34 || getRegisterDataType(REGISTER_Y) == dtComplex34  || getRegisterDataType(REGISTER_Y) == dtLongInteger   || getRegisterDataType(REGISTER_Y) == dtShortInteger    || getRegisterDataType(REGISTER_Y) == dtTime   || getRegisterDataType(REGISTER_Y) == dtDate)) {
    if(stringByteLength(REGISTER_STRING_DATA(REGISTER_Y)) < AIM_BUFFER_LENGTH) {

      if(eRPN) {      //JM NEWERPN 
        setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
      }                                        //JM NEWERPN 

      char tmp[2];
      tmp[0]=0;
      int16_t len = stringByteLength(tmp) + 1;

      reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), amNone);           //Make blank string in X
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
    char line1[XEQ_STR_LENGTH_LONG];
    line1[0]=0;
    strcpy(line1," ");
    int16_t len = stringByteLength(line1);
    if(eRPN) {      //JM NEWERPN 
      setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
    }                                        //JM NEWERPN 
    liftStack();
    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), amNone);
    strcpy(REGISTER_STRING_DATA(REGISTER_X),line1);
    fnXEQMEDIT(0);
  }
  last_CM=252;
  refreshScreen();
  last_CM=251;
  refreshScreen();
}



void fnXEQMexecute(char *line1) {
  displaywords(line1); 
  execute_string(line1,!EXEC,  !SCAN); //Run to catch all label names
  execute_string(line1, EXEC,  !SCAN); //Run to execute   
}



void fnXEQMXXEQ (uint16_t unusedButMandatoryParameter) {
#ifndef SAVE_SPACE_DM42_2
  char line1[XEQ_STR_LENGTH_LONG];
  if(getRegisterDataType(REGISTER_X) == dtString) {
    xcopy(line1, REGISTER_STRING_DATA(REGISTER_X), stringByteLength(REGISTER_STRING_DATA(REGISTER_X)) + 1);
    fnDrop(0);
    fnXEQMexecute(line1);
  }
#endif //SAVE_SPACE_DM42_2
}



void fnXEQNEW (uint16_t unusedButMandatoryParameter) {
#ifndef SAVE_SPACE_DM42_2
  fnStrtoX("XEQC43 XEQLBL 01 XXXXXX ");
  fnXEQMEDIT(0);
#endif //SAVE_SPACE_DM42_2
}




