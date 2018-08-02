/**********************************************************
* mywc.c
*
* Word count function that counts words ,lines and characters with escape key to stop the counting
*
* Author: John Kaefer and Priya Saravanan (jkaefer and psaravan)
* Date Created: 2/2
* Last Modified by: Priya
* Date Last Modified: 2/9
* Assignment: Lab 4
* Part of: C335 Labs - Spring 2018
*/

#include <f3d_uart.h>
#include <stm32f30x.h>
int c;
int chars;
int words;
int state;
int lines; //initializing characters
void main(){
 f3d_uart_init();
 // while(1){
  while((c=getchar())!= 0x1b){
    putchar(c);
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
  chars-=2;
  printf("%d %d %d \n", lines,words,chars);
  //}
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* mywc.c ends here */
