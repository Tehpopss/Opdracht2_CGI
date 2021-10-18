#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAXLEN 80
#define EXTRA 5
/* 4 for field name "data", 1 for "=" */
#define MAXINPUT MAXLEN + EXTRA + 2

void add_to_json(char *key, char *value);

void unencode(char *src, char *last, char *dest)
{
    for (; src != last; src++, dest++)
        if (*src == '+')
            *dest = ' ';
        else if (*src == '%')
        {
            int code;
            if (sscanf(src + 1, "%2x", &code) != 1)
                code = '?';
            *dest = code;
            src += 2;
        }
        else
            *dest = *src;
    //*dest = '\n';
    *++dest = '\0';
}

int main(void)
{
    char *lenstr;
    char input[MAXINPUT], data[MAXINPUT];
    char key[50], value[50];
    long len;
    printf("%s", "Set-Cookie:id=25565;");
    printf("%s%c%c\n", "Content-Type:application/json", 13, 10);
    //printf("%s%c%c\n", "Content-Type:application/json;charset=iso-8859-1", 13, 10);

    lenstr = getenv("CONTENT_LENGTH");
    sscanf(lenstr, "%ld", &len) != 1;
    fgets(input, len + 1, stdin);
    unencode(input + EXTRA, input + len, data);
    //printf("%s", data);
    char *token = strtok(data, "&");
    add_to_json("name", token);
    return 0;
}

void add_to_json(char *key, char *value)
{
    FILE *fp;
    char buff[255];
    char ch = 'a';

    char *ctime_no_newline;
    time_t tm = time(NULL);

    ctime_no_newline = strtok(ctime(&tm), "\n");

    fp = fopen("/var/www/html/main.json", "r+");

    //printf("\nFile content:\n");
    while ((ch = getc(fp)) != ']')
        ;

    fseek(fp, -1, SEEK_CUR);

    sprintf(buff, ",{\"%s\": \"%s\", \"submit_date\": \"%s\"}]", key, value, ctime_no_newline);
    fputs(buff, fp);

    //printf("\nEnd of file\n");

    // close file
    fclose(fp);

    // write to http request so JS can process JSON
    sprintf(buff, "{\"%s\": \"%s\", \"submit_date\": \"%s\"}", key, value, ctime_no_newline);
    printf("%s", buff);
}