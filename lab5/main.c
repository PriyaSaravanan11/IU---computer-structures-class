//main.c for lab6
/* main.c
 * Levi Roth and Priya Saravanan
 * levroth and psaravan
 * Contents: code to run working implementation of velocity visualization
 * Date created: 2/14
 * Lab 5
 * C335 

 */


#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>



void next_axis();
void led_off_check_positive();
void led_off_check_negative();
void led_on_check_positive();
void led_off_check_positive();

float values[3];  

char * axis_name = "xyz";

int axis=0;

int pressed = 0;

int main(void){

  f3d_uart_init();
  
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_led_init();

  f3d_user_btn_init();

  f3d_gyro_init();
  

  while(1){

    // checks for user input 
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t) RESET) {
      // gathers data
      f3d_gyro_getdata(values);
     
      // reads button press
      while (user_btn_read() == 1) {
	pressed = 1;
      }
      // checks that button was pushed
      if (pressed == 1) {
	next_axis();
	pressed = 0;
      }

      // checks for negative or positive values and adjusts lights
      if (values[axis]>0) {
	led_off_check_positive();
	led_on_check_positive();
      } else {
	led_off_check_negative();
	led_on_check_negative();
      }
      // prints values
      printf("%c: %f\n", axis_name[axis] ,values[axis]);
    }
    
    // switches on character to change axis
    switch(USART_ReceiveData(USART1)) {
      
    case 'x':
    case 'X':
      axis = 0;
      break;
    case 'y':
    case 'Y':
      axis = 1;
      break;
    case 'z':
    case 'Z':
      axis = 2;
      break;
    }
      
  }
   
}


void next_axis() {
  axis = (axis+1)%3;
}

void led_off_check_positive() {
  
  f3d_led_off(1);
  f3d_led_off(2);
  f3d_led_off(3);
  f3d_led_off(4);

  if (values[axis] < 5) {
    f3d_led_off(0);
  }
  if (values[axis] < 125) {
    f3d_led_off(7);
  }
  if (values[axis] < 250) {
    f3d_led_off(6);
  }
  if (values[axis] < 375) {
    f3d_led_off(5);
  }
  if (values[axis] < 500) {
    f3d_led_off(4);
  }
}

void led_on_check_positive() {
  
  if (values[axis] >= 5) {
    f3d_led_on(0);
  }
  if (values[axis] >= 125) {
    f3d_led_on(7);
  }
  if (values[axis] >= 250) {
    f3d_led_on(6);
  }
  if (values[axis] >= 375) {
    f3d_led_on(5);
  }
  if (values[axis] == 500) {
    f3d_led_on(4);
  }
}


void led_off_check_negative() {
  
  f3d_led_off(7);
  f3d_led_off(6);
  f3d_led_off(5);
  f3d_led_off(4);

  if (values[axis] > -5) {
    f3d_led_off(0);
  }
  if (values[axis] > -125) {
    f3d_led_off(1);
  }
  if (values[axis] > -250) {
    f3d_led_off(2);
  }
  if (values[axis] > -375) {
    f3d_led_off(3);
  }
  if (values[axis] > -500) {
    f3d_led_off(4);
  }
}

void led_on_check_negative() {

  if (values[axis] <= -5) {
    f3d_led_on(0);
  }
  if (values[axis] <= -125) {
    f3d_led_on(1);
  }
  if (values[axis] <= -250) {
    f3d_led_on(2);
  }
  if (values[axis] <= -375) {
    f3d_led_on(3);
  }
  if (values[axis] == -500) {
    f3d_led_on(4);
  }
}


void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}