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


/********************************************//** //JM
 * \file jmgraph.c Graphing module
 ***********************************************/

/* ADDITIONAL C43 functions and routines */

//#define DISPLOADING

#include "wp43s.h"
#include "math.h"

char                 filename_csv[40]; //JMMAX                //JM_CSV Changed from 60 to 40 to save 20 bytes.
uint32_t             mem__32;                                 //JM_CSV


void stackregister_csv_out(int16_t reg_b, int16_t reg_e) {
#ifndef TESTSUITE_BUILD
  char csv[TMP_STR_LENGTH];
  char csvp[TMP_STR_LENGTH];
  csvp[0] = 0;

  int16_t ix;
  ix = reg_b;
  while (ix <= reg_e) {
    if((ix>=100)&&(ix<=111)) {
       switch (ix)
       {
        case 100:   strcpy(csv, "X");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_X, csvp); break;
        case 101:   strcpy(csv, "Y");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_Y, csvp); break;
        case 102:   strcpy(csv, "Z");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_Z, csvp); break;
        case 103:   strcpy(csv, "T");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_T, csvp); break;
        case 104:   strcpy(csv, "A");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_A, csvp); break;
        case 105:   strcpy(csv, "B");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_B, csvp); break;
        case 106:   strcpy(csv, "C");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_C, csvp); break;
        case 107:   strcpy(csv, "D");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_D, csvp); break;
        case 108:   strcpy(csv, "L");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_L, csvp); break;
        case 109:   strcpy(csv, "I");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_I, csvp); break;
        case 110:   strcpy(csv, "J");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_J, csvp); break;
        case 111:   strcpy(csv, "K");strcat(csv, CSV_TAB);copyRegisterToClipboardString(REGISTER_K, csvp); break;
        default:break;
        }
      strcat(csv, csvp);
      strcat(csv, CSV_NEWLINE);
      } else
    if((ix>=0)&&(ix<=99)) {
      sprintf(csv, "%sR%02d%s%s", CSV_STR, ix, CSV_STR, CSV_TAB);
      copyRegisterToClipboardString(ix, csvp);
      strcat(csv, csvp);
      strcat(csv, CSV_NEWLINE);
    } else
    if((ix>=112)&&(ix<=112+100)) {
      sprintf(csv, "%sU%02d%s%s", CSV_STR, ix-100, CSV_STR, CSV_TAB);
      copyRegisterToClipboardString(ix, csvp);
      strcat(csv, csvp);
      strcat(csv, CSV_NEWLINE);
    }

    export_append_line(csv);                    //Output append to CSV file

    ++ix;
  }

#endif
}



void fnP_All_Regs(uint16_t unusedParamButMandatory){
  #if defined (DMCP_BUILD)
  check_create_dir("DATA");  
  make_date_filename(filename_csv,"DATA\\",".REGS.TSV");
  #endif


  switch (unusedParamButMandatory)
  {
  case 0:           //All registers
    {
       stackregister_csv_out(REGISTER_X,REGISTER_D);
       stackregister_csv_out(REGISTER_D,REGISTER_K);
       stackregister_csv_out(0,99);
       //stackregister_csv_out(FIRST_LOCAL_REGISTER,FIRST_LOCAL_REGISTER+100);
    }
    break;
  case 1:           //Stack only
    {
       stackregister_csv_out(REGISTER_X,REGISTER_D);
    }
    break;
  case 2:           //Global Registers
    {
       stackregister_csv_out(0,99);
    }
    break;
  case 3:           //USER Registers
    {
       stackregister_csv_out(FIRST_LOCAL_REGISTER,FIRST_LOCAL_REGISTER+100);
    }
    break;
  default:
    break;
  }
}


//###################################################################################
//###################################################################################
//###################################################################################

#ifdef DMCP_BUILD

TCHAR* f_gets (
  TCHAR* buff,  /* Pointer to the buffer to store read string */
  int len,    /* Size of string buffer (items) */
  FIL* fp     /* Pointer to the file object */
)
{
  int nc = 0;
  TCHAR *p = buff;
  BYTE s[4];
  UINT rc;
  DWORD dc;

      /* Byte-by-byte read without any conversion (ANSI/OEM API) */
  len -= 1; /* Make a room for the terminator */
  while (nc < len) {
    f_read(fp, s, 1, &rc);  /* Get a byte */
    if (rc != 1) break;   /* EOF? */
    dc = s[0];
    if (dc == '\r') continue;
    *p++ = (TCHAR)dc; nc++;
    if (dc == '\n') break;
  }

  *p = 0;   /* Terminate the string */
  return nc ? buff : 0; /* When no data read due to EOF or error, return with error. */
}



TCHAR* f_getsline (
  TCHAR* buff,  /* Pointer to the buffer to store read string */
  int len,    /* Size of string buffer (items) */
  FIL* fp     /* Pointer to the file object */
)
{
  int nc = 0;
  TCHAR *p = buff;
  BYTE s[2];
  UINT rc;
  BYTE sc;
  s[1]=0;
      /* Byte-by-byte read without any conversion (ANSI/OEM API) */
  len -= 1; /* Make a room for the terminator */
  while (nc < len) {
    f_read(fp, s, 1, &rc);  /* Get a byte */
    if (rc != 1) break;   /* EOF? */
    sc = s[0];
    *p++ = (TCHAR)sc; nc++;
  }

  *p = 0;   /* Terminate the string */
  return nc ? buff : 0; /* When no data read due to EOF or error, return with error. */
}


/*-----------------------------------------------------------------------*/
/* Put a Character to the File (sub-functions)                           */
/*-----------------------------------------------------------------------*/

/* Putchar output buffer and work area */

typedef struct {
  FIL *fp;    /* Ptr to the writing file */
  int idx, nchr;  /* Write index of buf[] (-1:error), number of encoding units written */
  BYTE buf[64]; /* Write buffer */
} putbuff;


/* Buffered write with code conversion */
static void putc_bfd (putbuff* pb, TCHAR c)
{
  UINT n;
  int i, nc;


  if (c == '\n') {    /* LF -> CRLF conversion */
    putc_bfd(pb, '\r');
  }

  i = pb->idx;        /* Write index of pb->buf[] */
  if (i < 0) return;
  nc = pb->nchr;      /* Write unit counter */


  /* ANSI/OEM input (without re-encoding) */
  pb->buf[i++] = (BYTE)c;


  if (i >= (int)(sizeof pb->buf) - 4) { /* Write buffered characters to the file */
    f_write(pb->fp, pb->buf, (UINT)i, &n);
    i = (n == (UINT)i) ? 0 : -1;
  }
  pb->idx = i;
  pb->nchr = nc + 1;
}


/* Flush remaining characters in the buffer */
static int putc_flush (putbuff* pb)
{
  UINT nw;

  if (   pb->idx >= 0 /* Flush buffered characters to the file */
    && f_write(pb->fp, pb->buf, (UINT)pb->idx, &nw) == FR_OK
    && (UINT)pb->idx == nw) return pb->nchr;
  return EOF;
}


/* Initialize write buffer */

/* Fill memory block */
static void mem_set (void* dst, int val, UINT cnt)
{
  BYTE *d = (BYTE*)dst;

  do {
    *d++ = (BYTE)val;
  } while (--cnt);
}


static void putc_init (putbuff* pb, FIL* fp)
{
  mem_set(pb, 0, sizeof (putbuff));
  pb->fp = fp;
}



int f_putc (
  TCHAR c,  /* A character to be output */
  FIL* fp   /* Pointer to the file object */
)
{
  putbuff pb;


  putc_init(&pb, fp);
  putc_bfd(&pb, c); /* Put the character */
  return putc_flush(&pb);
}




/*-----------------------------------------------------------------------*/
/* Put a String to the File                                              */
/*-----------------------------------------------------------------------*/

int f_puts (
  const TCHAR* str, /* Pointer to the string to be output */
  FIL* fp       /* Pointer to the file object */
)
{
  putbuff pb;


  putc_init(&pb, fp);
  while (*str) putc_bfd(&pb, *str++);   /* Put the string */
  return putc_flush(&pb);
}

/*-----------------------------------------------------------------------*/
//###################################################################################
//###################################################################################


int16_t export_append_string_to_file(const char line1[TMP_STR_LENGTH], const char filedir[40]) {
char line[100];               /* Line buffer */
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */


    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write access error ID001--> %d    \n",fr);    print_linestr(line,true);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, filedir, FA_OPEN_APPEND | FA_WRITE);
    if (fr) {
      sprintf(line,"File open error ID001--> %d    \n",fr);       print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Seek to end of the file to append data */
    fr = f_lseek(&fil, f_size(&fil));
    if (fr) {
      sprintf(line,"Seek error ID001--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Create string and output */
    fr = f_puts(line1, &fil);
    if (fr == -1) {
      sprintf(line,"Write error ID001--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"File close error ID001--> %d    \n",fr);     print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);
 
    return 0;
  }





int16_t export_string_to_file(const char line1[TMP_STR_LENGTH]) {
char dirfile[40];
    //Create file name
    strcpy(dirfile,"PROGRAMS\\C43_LOG.TXT");
    sprintf(tmpStr3000,"%s%s",line1,CSV_NEWLINE);
    check_create_dir("PROGRAMS");      
    if(export_append_string_to_file(tmpStr3000, dirfile) != 0) {
      //ERROR ALREADY ANNOUNCED
      return 1;
    }
  return 0;
}



void make_TSV_dir_name(void){
    uint32_t tmp__32;                                                 //JM_CSV

    tmp__32 = getUptimeMs();                                      //KEEP PERSISTENT FILE NAME FOR A PERIOD
    if ((mem__32 == 0) || (tmp__32 > mem__32 + 120000)) {
      //Create file name
      check_create_dir("DATA");  
      make_date_filename(filename_csv,"DATA\\",".STAT.TSV");
      check_create_dir("DATA");  
    }
    mem__32 = tmp__32;
}


int16_t export_xy_to_file(double x, double y){
  make_TSV_dir_name();

  sprintf(tmpStr3000,"%.16e%s%.16e%s",x,CSV_TAB,y,CSV_NEWLINE);
  if(export_append_string_to_file(tmpStr3000, filename_csv) != 0) {
    //ERROR ALREADY ANNOUNCED
    return 1;
  }
  return 0;
}





int16_t import_string_from_filename(char *line1, char *filename, char *fallback) {
char line[TMP_STR_LENGTH];        /* Line buffer */

    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    //Create file name
    check_create_dir("PROGRAMS");  
    strcpy(filename_csv,"PROGRAMS\\");
    strcat(filename_csv,filename);
    strcat(filename_csv,".TXT");
    
    /* Opens an existing file. */
    fr = f_open(&fil, filename_csv, FA_READ );   //| FA_OPEN_EXISTING
    if (fr != FR_OK) {
      if(fr == 4) {
        sprintf(line,"File not found PGM--> %d    \n",fr);
        print_linestr(line,false);
        sprintf(line,"File: %s \n",filename_csv);
        print_linestr(line,false);
      } else {
        sprintf(line,"File read open error ID004 PGM--> %d    \n",fr);        
        print_linestr(line,false);
      }
      f_close(&fil);
      //return (int)fr;
      strcpy(line1, fallback);
      return 0;
    }

    /* Read if open */
    line1[0]=0;
    f_getsline(line1, TMP_STR_LENGTH, &fil);
    f_close(&fil);
    return 0;
  }



//#####################################################################################
//#####################################################################################



int16_t export_append_line(char *inputstring){
char line[100];               /* Line buffer */
    FIL fil;                      /* File object */
    FRESULT fr;                   /* FatFs return code */

    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write access error--> %d    \n",fr);     print_linestr(line,true);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }


    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, filename_csv, FA_OPEN_APPEND | FA_WRITE);
    if (fr) {
      sprintf(line,"File open error--> %d    \n",fr);       print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Seek to end of the file to append data */
    fr = f_lseek(&fil, f_size(&fil));
    if (fr) {
      sprintf(line,"Seek error--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    fr = f_puts(inputstring, &fil);


    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"File close error--> %d    \n",fr);      print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);
 
    return 0;
  }




//**********************************************************************************************************
#elif PC_BUILD

int16_t import_string_from_filename(char *line1,  char *filename, char *fallback) {

  FILE *infile;
  char dirfile[40];
  char onechar[2];

  strcpy(dirfile,"PROGRAMS/");
  strcat(dirfile,filename);
  strcat(dirfile,".TXT");
  
  infile = fopen(dirfile, "rb");
  if (infile == NULL) {
    printf("Cannot load %s\n",dirfile);
    strcpy(line1, fallback);
    return 0;
  }

  tmpStr3000[0]=0;
  onechar[1]=0;
  while(fread(&onechar,1,1,infile)) {
  	strcat(tmpStr3000,onechar);
  }
  fclose(infile);
  printf("Loaded >>> %s\n",dirfile);
  //printf("Loaded >>> %s |%s|\n",dirfile,tmpStr3000);

  strcpy(line1,tmpStr3000);

return 0;
}





int16_t export_string_to_file(const char line1[TMP_STR_LENGTH]) {

  FILE *outfile;
  char dirfile[40];
  uint16_t fr = 0;
  char line[100];               /* Line buffer */

  strcpy(dirfile,"PROGRAMS/C43_LOG.TXT");

  outfile = fopen(dirfile, "ab");
  if (outfile == NULL) {
    printf("export_string_to_file: Cannot open to append: %s %s\n",dirfile,line1);
    return 1;
  }

  sprintf(tmpStr3000,"%s%s",line1,CSV_NEWLINE);
  fr = fputs(tmpStr3000, outfile);
  //printf(">>> %d\n",fr);
  if (fr == 0) {
    sprintf(line,"export_string_to_file: Write error--> %d    \n",fr);            
    //print_linestr(line,false);
    printf(line1);
    fclose(outfile);
    return (int)fr;
  } else {
    printf("Exported to %s: %s\n",dirfile,line1);
    fclose(outfile);
  }
  return 0;  
}



int16_t export_append_line(char *inputstring){

  FILE *outfile;
  uint16_t fr = 0;
  char line[100];               /* Line buffer */

//  strcpy(dirfile,"PROGRAMS/C43_LOG.TXT");

  outfile = fopen(filename_csv, "ab");
  if (outfile == NULL) {
    printf("export_string_to_file: Cannot open to append: %s %s\n",filename_csv,inputstring);
    return 1;
  }

  fr = fputs(inputstring, outfile);

  #if !defined(__MINGW64__)
    if (fr == 0) {
      sprintf(line,"export_string_to_file: Write error--> %d %s\n",fr,inputstring);            
      //print_linestr(line,false);
      printf(line);
      fclose(outfile);
      return (int)fr;
    } 
    else
  #endif
  {
    printf("Exported to %s: %s\n",filename_csv,inputstring);
    fclose(outfile);
  }
  return 0;  
}


void make_TSV_dir_name(void){
    uint32_t tmp__32;                                                 //JM_CSV
    time_t rawTime;
    struct tm *timeInfo;
    

    tmp__32 = getUptimeMs();                                      //KEEP PERSISTENT FILE NAME FOR A PERIOD
    if ((mem__32 == 0) || (tmp__32 > mem__32 + 120000)) {
      //Create file name
      //make_date_filename(filename_csv,"/DATA/",".STAT.TSV");
      time(&rawTime);
      timeInfo = localtime(&rawTime);
      strftime(filename_csv, filenamelen, "%Y%m%d-%H%M%S00.STAT.TSV", timeInfo);
    }
    mem__32 = tmp__32;
}


int16_t export_xy_to_file(double x, double y){
  char line[100];               /* Line buffer */
  make_TSV_dir_name();
  sprintf(line, "%.16e%s%.16e%s",x,CSV_TAB,y,CSV_NEWLINE);
  export_append_line(line);
  return 0;
}


#endif



//################################################################################################

void displaywords(char *line1) {  //Preprocessor and display
  char bb[2];
  char aa[2];
  bool_t state_comments=false;
  aa[1]=0;
  bb[1]=0;
  bb[0]=0;
  print_linestr("Code:",true);
  //printf("4:%s\n",line1);

  tmpStr3000[0]=32;tmpStr3000[1]=0;

  int16_t ix = 0;
  while (line1[ix] != 0) {
    aa[0]=line1[ix];
    bb[0]=line1[ix+1];

    if ((aa[0]==47 && bb[0]==47)) {         //start and stop comment mode on double slash
      state_comments=!state_comments;
      ix++; //skip the second slash
    } 
    else 
    if (state_comments && (aa[0]==13 || aa[0]==10))  {    //stop comment mode at end of line
      state_comments=false;
      strcat(tmpStr3000," ");
    } 
    else
      if(!state_comments) {                //proceed only if not comment mode
        switch(aa[0]) {
          case 32:                         //remove all whitespace
          case 8:
          case 13:
          case 10:
          case 44: if(strlen(tmpStr3000)!=0) {
                     if(tmpStr3000[strlen(tmpStr3000)-1] != 32) {
                       strcat(tmpStr3000," ");
                     }
                   }
                   break;
          default: strcat(tmpStr3000,aa);
        }    
      }

    ix++;
  }
  //printf("5:%s\n",line1);

  #ifdef DISPLOADING
  char ll[50];
  ll[0]=0;
  ll[0]=0;
  #endif
  aa[0]=0; aa[1]=0;                  //remove consequtive spaces
  ix = 1;
  line1[0]=0;
  while (tmpStr3000[ix] != 0) {
    aa[0]=tmpStr3000[ix];    
    if (tmpStr3000[ix-1] != 32) {
      strcat(line1,aa);
      #ifdef DISPLOADING
      strcat(ll,aa);
      if(strlen(ll)>30 && aa[0] == 32) {
        print_linestr(ll,false);
        ll[0]=0;
      }
      #endif
    } else {
      if(aa[0] != 32) {
        strcat(line1,aa);
        #ifdef DISPLOADING
        strcat(ll,aa);          
        if(strlen(ll)>36) {
          print_linestr(ll,false);
          ll[0]=0;
        }
        #endif
      }
    }
    ix++;
  }
  #ifdef DISPLOADING
  if(ll[0]!=0) {
    print_linestr(ll,false);
  }
  #endif
//printf("6:%s\n",line1);


}






int16_t line_x,line_y;

void print_inlinestr(const char line1[TMP_STR_LENGTH], bool_t endline) {
#ifndef TESTSUITE_BUILD
    char l1[TMP_STR_LENGTH];    //Clip the string at 40
    strcpy(l1, line1);
    l1[40]=0;    l1[41]=0;    l1[42]=0;


    if(line_y < SCREEN_HEIGHT) { 
        line_x = showString(l1, &standardFont, line_x, line_y, vmNormal, true, true);
    }
    if(endline) {
      line_y += 20;
      line_x = 0;
    }
    force_refresh();
#endif
}

void print_linestr(const char line1[TMP_STR_LENGTH], bool_t line_init) {
#ifndef TESTSUITE_BUILD
    char l1[TMP_STR_LENGTH];    //Clip the string at 40
    strcpy(l1, line1);
    l1[40]=0;    l1[41]=0;    l1[42]=0;

    if(line_init) {line_y = 20;}
    if(line_y < SCREEN_HEIGHT) { 
        line_x = showString(l1, &standardFont, 1, line_y, vmNormal, true, true);
    }
    line_y += 20;
    line_x = 0;
    force_refresh();
#endif
}

void print_numberstr(const char line1[TMP_STR_LENGTH], bool_t line_init) {     //ONLY N=ASCII NUMBERS AND E AND .
#ifndef TESTSUITE_BUILD
    if(line_init) {line_y = 20;}
    if(line_y < SCREEN_HEIGHT) { 
        int16_t cnt = 0;
        char tt[2];
        while(line1[cnt] != 0) {
          tt[0]=line1[cnt]; tt[1]=0;
          line_x = showString(tt, &standardFont, cnt * 8, line_y, vmNormal, true, true);
          cnt++;
        }
    }
    line_y += 20;
    line_x = 0;
    force_refresh();
#endif
}



