#ifndef _BUTTON_H_

#define _BUTTON_H_
#define MESSAGE_ID 1122

typedef struct{
    long int messageNum;
    unsigned short keyInput;
    unsigned short type;
    int pressed;
} BUTTON_MSG_T;

int buttonInit(void);
int buttonExit(void);
int buttonStart(void);

#endif
