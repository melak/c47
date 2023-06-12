/* This file is part of WP WP43.
 *
 * WP43 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP WP43 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP43.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */


/********************************************//** //JM
 * \file graphText.c TEXTFILES module
 ***********************************************/

/* ADDITIONAL C43 functions and routines */

//#define DISPLOADING

#include "c43Extensions/graphText.h"

#include "charString.h"
#include "math.h"
#include "screen.h"
#include "time.h"
#include "timer.h"
#include "c43Extensions/textfiles.h"
#include "c43Extensions/xeqm.h"
#include <string.h>

#include "wp43.h"

char                 filename_csv[40]; //JMMAX                //JM_CSV Changed from 60 to 40 to save 20 bytes.
uint32_t             mem__32;                                 //JM_CSV
bool_t               cancelFilename = false;

#ifdef DMCP_BUILD
//###################################################################################
/*-DMCP-*/
/*-DMCP-*/ TCHAR* f_gets ( //DMCP_BUILD ONLY
/*-DMCP-*/   TCHAR* buff,  /* Pointer to the buffer to store read string */
/*-DMCP-*/   int len,    /* Size of string buffer (items) */
/*-DMCP-*/   FIL* fp     /* Pointer to the file object */
/*-DMCP-*/ )
/*-DMCP-*/ {
/*-DMCP-*/   int nc = 0;
/*-DMCP-*/   TCHAR *p = buff;
/*-DMCP-*/   BYTE s[4];
/*-DMCP-*/   UINT rc;
/*-DMCP-*/   DWORD dc;
/*-DMCP-*/
/*-DMCP-*/       /* Byte-by-byte read without any conversion (ANSI/OEM API) */
/*-DMCP-*/   len -= 1; /* Make a room for the terminator */
/*-DMCP-*/   while (nc < len) {
/*-DMCP-*/     f_read(fp, s, 1, &rc);  /* Get a byte */
/*-DMCP-*/     if (rc != 1) break;   /* EOF? */
/*-DMCP-*/     dc = s[0];
/*-DMCP-*/     if (dc == '\r') continue;
/*-DMCP-*/     *p++ = (TCHAR)dc; nc++;
/*-DMCP-*/     if (dc == '\n') break;
/*-DMCP-*/   }
/*-DMCP-*/
/*-DMCP-*/   *p = 0;   /* Terminate the string */
/*-DMCP-*/   return nc ? buff : 0; /* When no data read due to EOF or error, return with error. */
/*-DMCP-*/ }
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/ TCHAR* f_getsline ( //DMCP_BUILD ONLY
/*-DMCP-*/   TCHAR* buff,  /* Pointer to the buffer to store read string */
/*-DMCP-*/   int len,    /* Size of string buffer (items) */
/*-DMCP-*/   FIL* fp     /* Pointer to the file object */
/*-DMCP-*/ )
/*-DMCP-*/ {
/*-DMCP-*/   int nc = 0;
/*-DMCP-*/   TCHAR *p = buff;
/*-DMCP-*/   BYTE s[2];
/*-DMCP-*/   UINT rc;
/*-DMCP-*/   BYTE sc;
/*-DMCP-*/   s[1]=0;
/*-DMCP-*/       /* Byte-by-byte read without any conversion (ANSI/OEM API) */
/*-DMCP-*/   len -= 1; /* Make a room for the terminator */
/*-DMCP-*/   while (nc < len) {
/*-DMCP-*/     f_read(fp, s, 1, &rc);  /* Get a byte */
/*-DMCP-*/     if (rc != 1) break;   /* EOF? */
/*-DMCP-*/     sc = s[0];
/*-DMCP-*/     *p++ = (TCHAR)sc; nc++;
/*-DMCP-*/   }
/*-DMCP-*/
/*-DMCP-*/   *p = 0;   /* Terminate the string */
/*-DMCP-*/   return nc ? buff : 0; /* When no data read due to EOF or error, return with error. */
/*-DMCP-*/ }
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/ /*-----------------------------------------------------------------------*/
/*-DMCP-*/ /* Put a Character to the File (sub-functions)                           */
/*-DMCP-*/ /*-----------------------------------------------------------------------*/
/*-DMCP-*/
/*-DMCP-*/ /* Putchar output buffer and work area */
/*-DMCP-*/
/*-DMCP-*/ typedef struct { //DMCP_BUILD ONLY
/*-DMCP-*/   FIL *fp;    /* Ptr to the writing file */
/*-DMCP-*/   int idx, nchr;  /* Write index of buf[] (-1:error), number of encoding units written */
/*-DMCP-*/   BYTE buf[64]; /* Write buffer */
/*-DMCP-*/ } putbuff;
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/ /* Buffered write with code conversion */
/*-DMCP-*/ static void putc_bfd (putbuff* pb, TCHAR c) //DMCP_BUILD ONLY
/*-DMCP-*/ {
/*-DMCP-*/   UINT n;
/*-DMCP-*/   int i, nc;
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/   if (c == '\n') {    /* LF -> CRLF conversion */
/*-DMCP-*/     putc_bfd(pb, '\r');
/*-DMCP-*/   }
/*-DMCP-*/
/*-DMCP-*/   i = pb->idx;        /* Write index of pb->buf[] */
/*-DMCP-*/   if (i < 0) return;
/*-DMCP-*/   nc = pb->nchr;      /* Write unit counter */
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/   /* ANSI/OEM input (without re-encoding) */
/*-DMCP-*/   pb->buf[i++] = (BYTE)c;
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/   if (i >= (int)(sizeof pb->buf) - 4) { /* Write buffered characters to the file */
/*-DMCP-*/     f_write(pb->fp, pb->buf, (UINT)i, &n);
/*-DMCP-*/     i = (n == (UINT)i) ? 0 : -1;
/*-DMCP-*/   }
/*-DMCP-*/   pb->idx = i;
/*-DMCP-*/   pb->nchr = nc + 1;
/*-DMCP-*/ }
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/ /* Flush remaining characters in the buffer */
/*-DMCP-*/ static int putc_flush (putbuff* pb) //DMCP_BUILD ONLY
/*-DMCP-*/ {
/*-DMCP-*/   UINT nw;
/*-DMCP-*/
/*-DMCP-*/   if (   pb->idx >= 0 /* Flush buffered characters to the file */
/*-DMCP-*/     && f_write(pb->fp, pb->buf, (UINT)pb->idx, &nw) == FR_OK
/*-DMCP-*/     && (UINT)pb->idx == nw) return pb->nchr;
/*-DMCP-*/   return EOF;
/*-DMCP-*/ }
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/ /* Initialize write buffer */
/*-DMCP-*/
/*-DMCP-*/ /* Fill memory block */
/*-DMCP-*/ static void mem_set (void* dst, int val, UINT cnt) //DMCP_BUILD ONLY
/*-DMCP-*/ {
/*-DMCP-*/   BYTE *d = (BYTE*)dst;
/*-DMCP-*/
/*-DMCP-*/   do {
/*-DMCP-*/     *d++ = (BYTE)val;
/*-DMCP-*/   } while (--cnt);
/*-DMCP-*/ }
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/ static void putc_init (putbuff* pb, FIL* fp) //DMCP_BUILD ONLY
/*-DMCP-*/ {
/*-DMCP-*/   mem_set(pb, 0, sizeof (putbuff));
/*-DMCP-*/   pb->fp = fp;
/*-DMCP-*/ }
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/ int f_putc ( //DMCP_BUILD ONLY
/*-DMCP-*/   TCHAR c,  /* A character to be output */
/*-DMCP-*/   FIL* fp   /* Pointer to the file object */
/*-DMCP-*/ )
/*-DMCP-*/ {
/*-DMCP-*/   putbuff pb;
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/   putc_init(&pb, fp);
/*-DMCP-*/   putc_bfd(&pb, c); /* Put the character */
/*-DMCP-*/   return putc_flush(&pb);
/*-DMCP-*/ }
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/ /*-----------------------------------------------------------------------*/
/*-DMCP-*/ /* Put a String to the File                                              */
/*-DMCP-*/ /*-----------------------------------------------------------------------*/
/*-DMCP-*/
/*-DMCP-*/ int f_puts ( //DMCP_BUILD ONLY
/*-DMCP-*/   const TCHAR* str, /* Pointer to the string to be output */
/*-DMCP-*/   FIL* fp       /* Pointer to the file object */
/*-DMCP-*/ )
/*-DMCP-*/ {
/*-DMCP-*/   putbuff pb;
/*-DMCP-*/
/*-DMCP-*/
/*-DMCP-*/   putc_init(&pb, fp);
/*-DMCP-*/   while (*str) putc_bfd(&pb, *str++);   /* Put the string */
/*-DMCP-*/   return putc_flush(&pb);
/*-DMCP-*/ }
/*-DMCP-*/
/*-DMCP-*/
//###################################################################################


//DMCP_BUILD
int16_t export_append_string_to_file(const char line1[TMP_STR_LENGTH], uint8_t mode, const char filedir[40]) { //DMCP_BUILD
char line[200];               /* Line buffer */
    FIL fil;                      /* File object */
    int fr;                   /* FatFs return code */


    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write error ID001--> %d    \n",fr);    print_linestr(line,true);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Opens an existing file. If not exist, creates a new file. */
    if(mode == APPEND) fr = f_open(&fil, filedir, FA_OPEN_APPEND | FA_WRITE); else fr = f_open(&fil, filedir, FA_WRITE|FA_CREATE_ALWAYS);
    if (fr) {
      sprintf(line,"File open error ID002--> %d    \n",fr);       print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* Seek to end of the file to append data */
    if(mode == APPEND) {
      fr = f_lseek(&fil, f_size(&fil));
      if (fr) {
        sprintf(line,"Seek error ID003--> %d    \n",fr);            print_linestr(line,false);
        f_close(&fil);
        sys_disk_write_enable(0);
        return (int)fr;
      }
    }

    /* Create string and output */
    fr = f_puts(line1, &fil);
    if (fr == EOF) {
      sprintf(line,"Write error ID004--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"Close error ID005--> %d    \n",fr);     print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);

    return 0;
  }


static FIL fil;               /* File object */
int16_t export_append_string_to_file_n(const char *line1, uint8_t mode, const char filedir[40]) { //DMCP_BUILD
char line[200];               /* Line buffer */
    int fr;                   /* FatFs return code */

  switch (mode) {
    case OPEN:
    case APPEND:
      /* Prepare to write */
      sys_disk_write_enable(1);
      fr = sys_is_disk_write_enable();
      if (fr==0) {
        sprintf(line,"Write error ID001--> %d    \n",fr);    print_linestr(line,true);
        f_close(&fil);
        sys_disk_write_enable(0);
        return (int)fr;
      }
      /* Opens an existing file. If not exist, creates a new file. */
      if(mode == APPEND) {
        fr = f_open(&fil, filedir, FA_OPEN_APPEND | FA_WRITE);
      } else {
        fr = f_open(&fil, filedir, FA_WRITE|FA_CREATE_ALWAYS);
      }
      if (fr) {
        sprintf(line,"File open error ID002--> %d    \n",fr);       print_linestr(line,false);
        f_close(&fil);
        sys_disk_write_enable(0);
        return (int)fr;
      }
      /* Seek to end of the file to append data */
      if(mode == APPEND) {
        fr = f_lseek(&fil, f_size(&fil));
        if (fr) {
          sprintf(line,"Seek error ID003--> %d    \n",fr);            print_linestr(line,false);
          f_close(&fil);
          sys_disk_write_enable(0);
          return (int)fr;
        }
      }
      break;

    case WRITE:
      /* Create string and output */
      fr = f_puts(line1, &fil);
      if (fr == EOF) {
        sprintf(line,"Write error ID004--> %d    \n",fr);            print_linestr(line,false);
        f_close(&fil);
        sys_disk_write_enable(0);
        return (int)fr;
      }
      break;

    case CLOSE:
      /* close the file */
      fr = f_close(&fil);
      if (fr) {
        sprintf(line,"Close error ID005--> %d    \n",fr);     print_linestr(line,false);
        f_close(&fil);
        sys_disk_write_enable(0);
        return (int)fr;
      }
      sys_disk_write_enable(0);
      break;

    default:
       break;
    }

    return 0;
  }


int16_t open_text(const char *dirname, const char *dirfile) { //DMCP_BUILD
    char rr[2];
    rr[0]=0;
    check_create_dir(dirname);
    if(export_append_string_to_file_n(rr, OPEN, dirfile)) return 1;
    return 0;
}

int16_t close_text(const char *dirfile) { //DMCP_BUILD
    char rr[2];
    rr[0]=0;
    if(export_append_string_to_file_n(rr, CLOSE, dirfile)) return 1;
    return 0;
}

int16_t save_text(const char *line1, uint8_t mode1, uint8_t mode2, uint8_t mode3, int16_t  nn, const char *dirfile) { //DMCP_BUILD
    char rr[2];
    rr[1]=0;
    switch(mode1) {
      case OPEN:
      case APPEND:
        if(export_append_string_to_file_n(rr, mode1, dirfile)) return 1;
        break;
      default: break;
    }

    int16_t ii, jj;
    switch(mode2) {
      case WRITE:
        ii = min(nn,(int16_t)(strlen(line1)));
        jj = ii;
        while(ii!=0) {
          rr[0]=line1[jj-ii];
          if(export_append_string_to_file_n(rr, mode2, dirfile) != 0) return 1;
        ii--;
        }
        break;
      default: break;
    }

    switch(mode3) {
      case CLOSE:
        if(export_append_string_to_file_n(rr, mode3, dirfile)) return 1;
        break;
      default: break;
    }


    return 0;
}



//DMCP_BUILD
int16_t export_string_to_filename(const char line1[TMP_STR_LENGTH], uint8_t mode, char *dirname, char *filename) { //DMCP_BUILD
char dirfile[40];
    //Create file name
    strcpy(dirfile,dirname);
    strcat(dirfile,"\\");
    strcat(dirfile,filename);

    sprintf(tmpString,"%s%s",line1,CSV_NEWLINE);
    check_create_dir(dirname);
    if(export_append_string_to_file(tmpString, mode, dirfile) != 0) {
      //ERROR ALREADY ANNOUNCED
      return 1;
    }
  return 0;
}



//DMCP_BUILD
int16_t import_string_from_filename(char *line1,  char *dirname,  char *filename_short,  char *filename,  char *fallback, bool_t scanning) { //DMCP_BUILD

    #if (VERBOSE_LEVEL >= 2)
      print_inlinestr("From dir:",false);
      print_inlinestr(dirname,false);
      print_inlinestr(", ",true);
    #endif

    #if (VERBOSE_LEVEL >= 2)
      char line[300];               /* Line buffer */
    #endif

    char dirfile[200];
    dirfile[0]=0;
    FIL fil;                      /* File object */
    int fr;                   /* FatFs return code */

    //Create dir name
    check_create_dir(dirname);

    strcpy(dirfile,dirname);
    strcat(dirfile,"\\");
    strcat(dirfile,filename_short);

    #if (VERBOSE_LEVEL >= 1)
      print_inlinestr("1: reading:",false);
      print_inlinestr(dirfile,false);
      print_inlinestr(" ",true);
    #endif

    /* Opens an existing file. */
    fr = f_open(&fil, dirfile, FA_READ );   //| FA_OPEN_EXISTING
    if (fr != FR_OK) {
      #if (VERBOSE_LEVEL >= 1)
        print_inlinestr("Not open. ",false);
        #if (VERBOSE_LEVEL >= 2)
          if(fr == 4) {
            sprintf(line,"Not found ID006 --> %d ",fr); print_inlinestr(line,false);
            sprintf(line,"File: %s \n",dirfile);        print_inlinestr(line,false);
          } else if(fr == 5) {
            sprintf(line,"No path ID007 --> %d ",fr);   print_inlinestr(line,false);
            sprintf(line,"File: %s \n",dirfile);        print_inlinestr(line,false);
          } else {
            sprintf(line,"File open error --> %d ",fr); print_inlinestr(line,false);
            sprintf(line,"File: %s \n",dirfile);        print_inlinestr(line,false);
          }
        #endif
      #endif
      #if (VERBOSE_LEVEL >= 2)
        print_inlinestr(".",true);
      #endif
      f_close(&fil);

      if(filename[0]!=0) {
        strcpy(dirfile,dirname);
        strcat(dirfile,"\\");
        strcat(dirfile,filename);
        #if (VERBOSE_LEVEL >= 1)
          print_inlinestr("2: reading:",false);
          print_inlinestr(dirfile,false);
          print_inlinestr(" ",true);
        #endif

        /* Opens an existing file. */
        fr = f_open(&fil, dirfile, FA_READ );   //| FA_OPEN_EXISTING
        if (fr != FR_OK) {
          #if (VERBOSE_LEVEL >= 1)
            print_inlinestr("Not open. ",false);
            #if (VERBOSE_LEVEL >= 2)
              if(fr == 4) {
                sprintf(line,"Not found ID007 --> %d ",fr); print_inlinestr(line,false);
                sprintf(line,"File: %s \n",dirfile);        print_inlinestr(line,false);
              } else {
                sprintf(line,"File open error --> %d ",fr); print_inlinestr(line,false);
              }
            #endif
          #endif
          #if (VERBOSE_LEVEL >= 1)
            print_inlinestr(". Using fallback.",true);
          #endif
          f_close(&fil);
          strcpy(line1, fallback);
          return 1;
        }
      }
      else {
        #if (VERBOSE_LEVEL >= 1)
          print_inlinestr("Using fallback.",true);
        #endif
        strcpy(line1, fallback);
        return 1;
      }
    }

    #if (VERBOSE_LEVEL >= 1)
      print_inlinestr("reading...",false);
    #endif

    /* Read if open */
    line1[0]=0;
    f_getsline(line1, (scanning ? min(100,TMP_STR_LENGTH) : TMP_STR_LENGTH), &fil);
    f_close(&fil);

    #if (VERBOSE_LEVEL >= 1)
      print_inlinestr("read:",true);
      print_inlinestr(line1,true);
    #endif

    if(stringByteLength(line1) >= TMP_STR_LENGTH-1) {
      strcpy(line1, fallback);
      #if (VERBOSE_LEVEL >= 1)
        print_inlinestr("ERROR too long file using fallback",true);
      #endif
      return 1;
    }

    return 0;
  }



//DMCP_BUILD
void create_filename(char *fn){ //DMCP_BUILD //fn must be in format ".STAT.TSV"
    uint32_t tmp__32;                                                 //JM_CSV

    tmp__32 = getUptimeMs();                                      //KEEP PERSISTENT FILE NAME FOR A PERIOD
    if ( cancelFilename || (mem__32 == 0) || (tmp__32 > mem__32 + 120000) || (stringByteLength(filename_csv) > 10 && !strcompare(filename_csv + (stringByteLength(filename_csv) - 9),fn  ) ) ) {
      //Create file name
      check_create_dir("DATA");
      make_date_filename(filename_csv,"DATA\\",fn);
      check_create_dir("DATA");
    }
    mem__32 = tmp__32;
    cancelFilename = false;
}


//DMCP_BUILD
int16_t export_xy_to_file(float x, float y){
  char line[TMP_STR_LENGTH];               /* Line buffer */
  create_filename(".STAT.TSV");
  sprintf(line,"%.16e%s%.16e%s",x,CSV_TAB,y,CSV_NEWLINE);
  if(export_append_string_to_file(line, APPEND, filename_csv) != 0) {
    //ERROR ALREADY ANNOUNCED
    return 1;
  }
  return 0;
}


// /DMCP_BUILD
int16_t export_append_line_short(char *inputstring){ //DMCP_BUILD
char line[200];               /* Line buffer */
    FIL fil;                      /* File object */
    int fr;                   /* FatFs return code */

    /* Prepare to write */
    sys_disk_write_enable(1);
    fr = sys_is_disk_write_enable();
    if (fr==0) {
      sprintf(line,"Write error--> %d    \n",fr);     print_linestr(line,false);
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
    #if (VERBOSE_LEVEL >= 1)
      sprintf(line,"wrote %d, %s\n",fr,inputstring);        print_linestr(line,false);
    #endif
    if(fr == 0) {
      sprintf(line,"Write error--> %d    \n",fr);            print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    /* close the file */
    fr = f_close(&fil);
    if (fr) {
      sprintf(line,"File close error--> %d    \n",fr);      print_linestr(line,false);
      f_close(&fil);
      sys_disk_write_enable(0);
      return (int)fr;
    }

    sys_disk_write_enable(0);


    #if (VERBOSE_LEVEL >= 1)
      print_linestr("-closed return-",false);
    #endif


    return 0;
  }


int16_t export_append_line(char *inputstring){ //DMCP_BUILD
int ix=0;
char tmp[200];
int fr;
#define linesize 128

  while(stringByteLength(inputstring + ix) > linesize) {
     xcopy(tmp, inputstring + ix, linesize);
     tmp[linesize]=0;
     fr = export_append_line_short(tmp);
     if(fr) return (int)fr;
     ix += linesize;
  }
  if(stringByteLength(inputstring + ix) > 0) {
     fr = export_append_line_short(inputstring + ix);
     if(fr) return (int)fr;
  }
  return 0;
}


//ENDIF DMCP_BUILD

//**********************************************************************************************************
#elif PC_BUILD //PC_BUILD
int16_t export_string_to_filename(const char line1[TMP_STR_LENGTH], uint8_t mode, char *dirname, char *filename) { //PC_BUILD

  FILE *outfile;
  char dirfile[40];
  uint16_t fr = 0;
  char line[200];               /* Line buffer */

  strcpy(dirfile,dirname);
  strcat(dirfile,"/");
  strcat(dirfile,filename);

  if(mode == APPEND) outfile = fopen(dirfile, "ab"); else outfile = fopen(dirfile, "wb");
  if (outfile == NULL) {
    printf("Cannot open ID008: %s %s\n",dirfile,line1);
    return 1;
  }

  sprintf(tmpString,"%s%s",line1,CSV_NEWLINE);
  fr = fputs(tmpString, outfile);
  if (fr == 0) {
    sprintf(line,"Write error ID009 --> %d    \n",fr);
    //print_linestr(line,false);
    printf("%s",line1);
    fclose(outfile);
    return (int)fr;
  } else {
    printf("Exported to %s: %s\n",dirfile,line1);
    fclose(outfile);
  }
  return 0;
}

// PC_BUILD
int16_t import_string_from_filename(char *line1,  char *dirname,   char *filename_short,  char *filename,  char *fallback, bool_t scanning) { //PC_BUILD

  #if (VERBOSE_LEVEL >= 2)
    print_inlinestr("From dir:",false);
    print_inlinestr(dirname,false);
    print_inlinestr(", ",true);
  #endif

  char dirfile[200];
  dirfile[0]=0;
  FILE *infile;
  char onechar[2];

  strcpy(dirfile,dirname);
  strcat(dirfile,"/");
  strcat(dirfile,filename_short);

  #if (VERBOSE_LEVEL >= 1)
    print_inlinestr("1: reading:",false);
    print_inlinestr(dirfile,false);
    print_inlinestr(" ",true);
  #endif

  /* Opens an existing file. */
  infile = fopen(dirfile, "rb");
  if (infile == NULL) {
    #if (VERBOSE_LEVEL >= 1)
      #ifdef PC_BUILD
        printf("Cannot load ID010 %s\n",dirfile);
      #endif
      print_inlinestr("Not open. ",true);
    #endif

    if(filename[0]!=0) {
      strcpy(dirfile,dirname);
      strcat(dirfile,"/");
      strcat(dirfile,filename);
      #if (VERBOSE_LEVEL >= 1)
        print_inlinestr("2: reading:",false);
        print_inlinestr(dirfile,false);
        print_inlinestr(" ",true);
      #endif

      /* Opens an existing file. */
      infile = fopen(dirfile, "rb");
      if (infile == NULL) {
        #if (VERBOSE_LEVEL >= 1)
          #ifdef PC_BUILD
            printf("Cannot load %s\n",dirfile);
          #endif
          print_inlinestr("Fallback.",true);
        #endif
        strcpy(line1, fallback);
        return 1;
      }
    }
    else {
      #if (VERBOSE_LEVEL >= 1)
        #ifdef PC_BUILD
          printf("Cannot load %s\n",dirfile);
        #endif
        print_inlinestr("Fallback.",true);
      #endif
      strcpy(line1, fallback);
      return 1;
    }
  }

  #if (VERBOSE_LEVEL >= 1)
    print_inlinestr("reading...",false);
  #endif

  /* Read if open */
  line1[0]=0;
  onechar[1]=0;
  while(fread(&onechar,1,1,infile)) {
    strcat(line1,onechar);
  }
  fclose(infile);
  #if (VERBOSE_LEVEL >= 1)
    #ifdef PC_BUILD
      printf("Loaded >>> %s\n",dirfile);
    #endif
    print_inlinestr("read:",true);
    print_inlinestr(line1,true);
  #endif

  #if (VERBOSE_LEVEL >= 2)
    #ifdef PC_BUILD
      printf("Loaded %s |%s|\n",dirfile,line1);
    #endif
  #endif


  if(stringByteLength(line1) >= TMP_STR_LENGTH-1) {
    strcpy(line1, fallback);
    #if (VERBOSE_LEVEL >= 1)
      print_inlinestr("ERROR too long file using fallback",true);
    #endif
    printf("ERROR too long file using fallback\n");
    return 1;
  }

return 0;
}



// PC_BUILD
int16_t export_append_line(char *inputstring){  //PC_BUILD

  FILE *outfile;

//  strcpy(dirfile,"PROGRAMS/C43_LOG.TXT");

  outfile = fopen(filename_csv, "ab");
  if (outfile == NULL) {
    printf("Cannot open to append ID011: %s %s\n",filename_csv,inputstring);
    return 1;
  }


    uint16_t fr = 0;
    char line[200];               /* Line buffer */
    fr = fputs(inputstring, outfile);
    if (fr == 0) {
      sprintf(line,"Write error ID012 --> %d %s\n",fr,inputstring);
      //print_linestr(line,false);
      printf("%s",line);
      fclose(outfile);
      return (int)fr;
    } else {
      printf("Exported to %s: %s\n",filename_csv,inputstring);
      fclose(outfile);
    }
    return 0;
  }


// PC_BUILD
void create_filename(char *fn){  //PC_BUILD //fn must be in format ".STAT.TSV"
    uint32_t tmp__32;                                                 //JM_CSV
    time_t rawTime;
    struct tm *timeInfo;

    tmp__32 = getUptimeMs();                                          //KEEP PERSISTENT FILE NAME FOR A PERIOD
    if (cancelFilename || (mem__32 == 0) || (tmp__32 > mem__32 + 120000)  || (stringByteLength(filename_csv) > 10 && !strcompare(filename_csv + (stringByteLength(filename_csv) - 9),fn  ) ) ) {
      //Create file name
      time(&rawTime);
      timeInfo = localtime(&rawTime);
      strftime(filename_csv, filenamelen, "%Y%m%d-%H%M%S00", timeInfo);
      strcat(filename_csv, fn);
    }
    mem__32 = tmp__32;
    cancelFilename = false;
}


int16_t export_xy_to_file(float x, float y) { //PC_BUILD
  char line[200];               /* Line buffer */
  create_filename(".STAT.TSV");
  sprintf(line, "%.16e%s%.16e%s",x,CSV_TAB,y,CSV_NEWLINE);
  export_append_line(line);
  return 0;
}


#endif



//################################################################################################




int16_t g_line_x, g_line_y;


void print_linestr(const char *line1, bool_t line_init) {
#ifndef TESTSUITE_BUILD
    char l1[200];
    l1[0]=0;
    int16_t ix = 0;
    int16_t ixx;

    if(line_init) {
      g_line_y = 20;
      g_line_x = 0;
    }
    strcat(l1,"-");
    ixx = stringByteLength(line1);
    while(ix<ixx && ix<98 && stringWidth(l1, &standardFont, true, true) < SCREEN_WIDTH-12-g_line_x) {
       xcopy(l1, line1, ix+1);
       l1[ix+1+1]=0;
       ix = stringNextGlyph(line1, ix);
    }
    while(stringByteLength(l1) < 200 && stringWidth(l1, &standardFont, true, true) < SCREEN_WIDTH-12-g_line_x) {
       strcat(l1,".");
    }

    if(g_line_y < SCREEN_HEIGHT) {
        ixx = showString(l1, &standardFont, (uint32_t) g_line_x, (uint32_t) g_line_y, vmNormal, true, true);
    }
    g_line_y += 20;
    if(g_line_y > SCREEN_HEIGHT - 20) {
      g_line_y = 40;
      g_line_x += 4;
    }
    force_refresh(timed);
#endif
}

void print_numberstr(const char *line1, bool_t line_init) {     //ONLY N=ASCII NUMBERS AND E AND . //FIXED FONT
#ifndef TESTSUITE_BUILD
    if(line_init) {g_line_y = 20;}
    if(g_line_y < SCREEN_HEIGHT) {
        int16_t cnt = 0;
        char tt[2];
        while(line1[cnt] != 0 && g_line_x < SCREEN_WIDTH-8 +1) {
          tt[0]=line1[cnt]; tt[1]=0;
          g_line_x = showString(tt, &standardFont, (uint32_t)cnt * 8, (uint32_t) g_line_y, vmNormal, true, true);
          cnt++;
        }
    }
    g_line_y += 20;
    g_line_x = 0;
    force_refresh(timed);
#endif
}

