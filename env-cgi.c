#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 80
#define EXTRA 5
/* 4 for field name "data", 1 for "=" */
#define MAXINPUT MAXLEN+EXTRA+2
#define DATAFILE  = "var/www/html/main.json"


void unencode(char *src, char *last, char *dest)
{
 for(; src != last; src++, dest++)
   if(*src == '+')
     *dest = ' ';
   else if(*src == '%') {
     int code;
     if(sscanf(src+1, "%2x", &code) != 1) code = '?';
     *dest = code;
     src +=2; }     
   else
     *dest = *src;
 *dest = '\n';
 *++dest = '\0';
}

int main(void)
{
  char *lenstr;
  char input[MAXINPUT], data[MAXINPUT];
  long len;
  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
  printf("<TITLE>Response</TITLE>\n");
  lenstr = getenv("CONTENT_LENGTH");
  sscanf(lenstr,"%ld",&len)!=1;
  fgets(input, len+1, stdin);
  unencode(input+EXTRA, input+len, data);
  printf("%s",data);
  return 0;
}