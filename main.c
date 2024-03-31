#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

void clear_term()
{
    for (int i = 0; i < 50; i++)
    {
        printf("                                                                                                                                                                                                                  \n");
    }
}

int main()
{
    int x=1;
    int mistakes=0;
    int starttime=0;
    struct termios state, new_state;
    tcgetattr(0, &state);
    new_state=state;
    new_state.c_lflag &= ~(ECHO | ICANON |ECHOE| ISIG);
    new_state.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_state);

    FILE * f = fopen("text", "r");
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    fseek(f, 0, SEEK_SET);
   
    char *text = malloc(len);
    fread(text, len, 1, f);
    
    clear_term();
    gotoxy(0, 1);
    printf("%s", text);
    gotoxy(0, 3);
    printf("mistakes: 0");
    gotoxy(15, 3);
    printf("time: 0");
    gotoxy(1, 1);
    int first_time=1;
    for (;;)
    {
        if (x == len)
        {
            gotoxy(0, 1);
            clear_term();
            gotoxy(0, 1);
            printf("Text: %s\n", text);
            if (mistakes) printf("Mistakes: %d\n", mistakes);
            else printf("Mistakes: 0, PERFECT!\n");
            printf("Accuracy %%: %.2f%%\n", (((float)len-(float)mistakes)/(float)len)*100);
            printf("Time: %d\n", (int)time(NULL) - starttime);
            printf("WPM: %.2f\n", ((float)len/(float)(time(NULL)-starttime))*60/5);
            
            exit(0);
        }
        gotoxy(x, 0);
        char a = getc(stdin);
        if (a=='\033')
        {
            exit(0);
        }
        if (a == text[x-1])
        {
            x++;
        }
        else
        {
            mistakes++;
            gotoxy(0, 3);
            printf("mistakes: %d", mistakes);
        }
        if (first_time)
        {
            starttime=time(NULL);
            first_time=0;
        }
        gotoxy(15, 3);
        printf("time: %d", time(NULL) - starttime);
    }


}
        
