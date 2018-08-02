
/**********************************************************
* main.c
*
* code that shows visualization on lcd
*
* Author: Levi Roth and Priya Saravanan (levroth and psaravan)
* Date Created: 2/20
* Last Modified by:
* Date Last Modified:
* Assignment: Lab 6
* Part of: C335 Labs - Spring 2018
*/
/**********************************************************
* main.c
*
* code that shows visualization on lcd
*
* Author: Levi Roth and Priya Saravanan (levroth and psaravan)
* Date Created: 2/20
* Last Modified by: 
* Date Last Modified:
* Assignment: Lab 6
* Part of: C335 Labs - Spring 2018
*/
#include <stm32f30x.h>
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>

#define TIMER 20000

void next_axis();
void lcd_on_check_positive(int ax);
void lcd_on_check_negative(int ax);
void print_lable();
void print_bar(int value, uint8_t x, uint8_t y, uint16_t color);

float values[3];  

char * axis_name = "xyz";

float slope = ((100)/2)/500;

int axis=0;

int pressed = 0;

int scale = 2;

uint16_t bg_color = BLACK;

uint16_t tx_color = WHITE;

int length[] = {1,1,1};

int main(void){



  f3d_uart_init();
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);


  f3d_lcd_init();


  f3d_led_init();

  f3d_user_btn_init();

  f3d_gyro_init();

  // initial fill
  f3d_lcd_fillScreen2(bg_color);
  
  // set up of axis labels
  f3d_lcd_drawChar(44, ST7735_height/2+7,'x', WHITE, bg_color);
  f3d_lcd_drawChar(64, ST7735_height/2+7,'y', WHITE, bg_color);
  f3d_lcd_drawChar(84, ST7735_height/2+7,'z', WHITE, bg_color);

  // label print
  f3d_lcd_drawString(10, ST7735_height/2+20, "Gyroscope Analysis", MAGENTA, bg_color);

  while(1){
    
    // gathers data
    f3d_gyro_getdata(values);

    // checks if positive or negative values to handle separately
    if (values[0] >= 0) {
      lcd_on_check_positive(0);
    } else {
      lcd_on_check_negative(0);
    }
    if (values[1] >= 0) {
      lcd_on_check_positive(1);
    } else {
      lcd_on_check_negative(1);
    }
    if (values[2] >= 0) {
      lcd_on_check_positive(2);
    } else {
      lcd_on_check_negative(2);
    }
  

    // printing x, y , z values
    print_bar(length[0], 45, ST7735_height/2-1, WHITE);
    print_bar(length[1], 65, ST7735_height/2-1, YELLOW);
    print_bar(length[2], 85, ST7735_height/2-1, CYAN);
    
  }
  
  
}

  
// function that prints based on scaled length at given position
void print_bar(int value, uint8_t x, uint8_t y, uint16_t color) {
  uint8_t i;
  for (i = 0; i < value; i++) {
    f3d_lcd_drawChar(x-2, y-i*4, '*', color, color); 
    f3d_lcd_drawChar(x+2, y-i*4, '*', color, color);
  }
  for (i = value; i <= 5*scale; i++) {
    f3d_lcd_drawChar(x-2, y-i*4, '*', bg_color, bg_color); 
    f3d_lcd_drawChar(x+2, y-i*4, '*', bg_color, bg_color);
  }
  
}

void print_lable() {
  f3d_lcd_drawChar(0,0,axis_name[axis], tx_color, bg_color);
}

void next_axis() {
  axis = (axis+1)%3;
}


void lcd_on_check_positive(int ax) {
  
  if (values[ax] >= 5) {
    length[ax] = 1*scale;
  }
  if (values[ax] >= 125) {
    length[ax] = 2*scale;
  }
  if (values[ax] >= 250) {
    length[ax] = 3*scale;
  }
  if (values[ax] >= 375) {
    length[ax] = 4*scale;
  }
  if (values[ax] == 500) {
    length[ax] = 5*scale;
  }
}


void lcd_on_check_negative(int ax) {

  if (values[ax] <= -5) {
    length[ax] = 1*scale;
  }
  if (values[ax] <= -125) {
    length[ax] = 2*scale;
  }
  if (values[ax] <= -250) {
    length[ax] = 3*scale;
  }
  if (values[ax] <= -375) {
    length[ax] = 4*scale;
  }
  if (values[ax] == -500) {
    length[ax] = 5*scale;
  }
}




void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
