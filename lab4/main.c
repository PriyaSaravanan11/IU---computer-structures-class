#include <f3d_uart.h>
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 

/**********************************************************
* main.c
*
* Has include file for drivers, loop function, and function to read char with escape key to stop the function and get number of lines, words and characters
*
* Author: John Kaefer and Priya Saravanan (jkaefer and psaravan)
* Date Created: 2/2
* Last Modified by: Priya
* Date Last Modified: 2/9
* Assignment: Lab 4
* Part of: C335 Labs - Spring 2018
*/

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int  main(){
  f3d_uart_init();

  // putchar('a');
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  int c;
  int chars;
  int lines;
  int words;
  int state; //initializing variables
  
  
 
  while ((c = getchar()) != 0x1b){
   //chars every iteration
    chars++;
//counting lines every \n 
    if((char)c=='\n'){
      lines++;}
//we have state=1 to stand for the fact that we're inside a word, ans then state automatically changed to 0
//to mean a word counted.
    if(((char)c=='\n'||(char)c=='\t'||(char)c=='\r'||(char)c==' '||(char)c=='\v'||(char)c=='\f')&&state==1){
    state=0;
    words++;
    }
//we are back inside a word
    if(!((char)c=='\n'||(char)c=='\t'||(char)c=='\r'||(char)c==' '||(char)c=='\v')||(char)c=='\f'){
    state=1;
  } 
  }
  printf("%d %d %d \n", lines,words,chars);
}



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
