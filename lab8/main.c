/**********************************************************
* main.c
*
* code for nunchuk displays
* Author: Levi Roth and Priya Saravanan (levroth and psaravan)
* Date Created: 3/2
* Last Modified by: -
* Date Last Modified: -
* Assignment: Lab8
* Part of: C335 Spring 18
*
*
* Filename: main.c
* Description:
* Author:
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_i2c2.h>
#include <f3d_gyro.h>
#include <f3d_nunchuk.h>
#include <stdio.h>
#include <math.h>

#define TIMER 20000

struct coord {
  int x;
  int y;
};

// Function headers
void pitch(float * val, float * ans);
void roll(float * val, float * ans);
void tilt(float * val, float * ans);
float compass(float * mag, float * acc);
void draw_axis(uint16_t col);
void pt_graph(float * val, uint16_t co);
void cp_graph(float val, uint16_t col);
void check_main_screen();

// radius for compass circle
int radius = 30;

// stores values for magnetometer
float mag[3];

// stores values from accelerometer
float acc[3];

// provides conversion for tilting purposes
float tilting[3];

// stores the button from the previous loop
int prev_pressed = 0;
// stores the current button 
int pressed = 0;

// determines which function to be on
int switch_prog = 0;

// location of the compass pointer
int mag_x = -1;
int mag_y = -1;

// slopes to convert to pixels
float roll_slope = ((ST7735_width-1)/2)/(M_PI);
float pitch_slope = ((ST7735_height-5)/2)/(M_PI);

// structure to store accelerometer pointer position
struct coord curr = {0,0};


// color scheme
uint16_t bg_color = BLACK;
uint16_t tx_color = WHITE;

// nunchuk test struct
nunchuk_t currN = {127,128,0,0,0,0,0};
nunchuk_t * currNp = &currN;

nunchuk_t prevN = {127,128,0,0,0,0,0};
nunchuk_t * prevNp = &prevN;

float acc_nun[3];
float ang_nun[3];

// gyro inits
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

int scale = 2;

int length[] = {1,1,1};


int main(void) {

  f3d_uart_init();
  delay(100);
  f3d_i2c1_init();
  delay(100);
  f3d_i2c2_init();
  delay(100);
  f3d_accel_init();
  delay(100);
  f3d_mag_init();
  delay(100);
  f3d_nunchuk_init();
  delay(100);
  // f3d_nunchuk_init2();
  delay(100);

  
  

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  
  // Set up your inits before you go ahead
  
  f3d_lcd_init();
  f3d_user_btn_init();
  f3d_gyro_init();
  f3d_lcd_fillScreen2(bg_color);  

  check_main_screen();

  while(1) {
    
    prevNp->jx = currNp->jx;
    prevNp->jy = currNp->jy;
    prevNp->ax = currNp->ax;
    prevNp->ay = currNp->ay;
    prevNp->az = currNp->az;
    prevNp->c = currNp->c;
    prevNp->z = currNp->z;

    f3d_nunchuk_read(currNp);
    
    acc_nun[0] = (currNp->ay - 512);
    acc_nun[1] = (currNp->ax - 512)*(-1);
    acc_nun[2] = (currNp->az - 512);
    
    printf("%c %c %d %d %d %c %c\n",
	   currNp->jx,
	   currNp->jy,
	   currNp->ax, 
	   currNp->ay,
	   currNp->az, 
	   currNp->c,
	   currNp->z);

    
    // stores values
    f3d_accel_read(acc);
    f3d_mag_read(mag);
    f3d_gyro_getdata(values);
    
    // calculates tilts
    pitch(acc, tilting);
    roll(acc, tilting);
    tilt(acc, tilting);

    // nunchuk tilt
    pitch(acc_nun, ang_nun);
    roll(acc_nun, ang_nun);
    tilt(acc_nun, ang_nun);
    

    // this checks if button was just released and switches program

    if ((prevNp->jx == 255 && currNp->jx != 255) || 
	(prevNp->c == 1 && currNp->c != 1)) {
      switch_prog = (switch_prog + 1)%4;
      check_main_screen();
    } else if ((prevNp->jx == 0 && currNp->jx != 0) ||
	       (prevNp->z == 1 && currNp->z !=1)) {
      if (switch_prog == 0) {
	switch_prog = 3;
      } else {
	switch_prog = switch_prog - 1;
      }
      check_main_screen();
    }
    
    
  
      
    // calls the specified function
   
    switch(switch_prog) {
    case 0:
     
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
      print_bar(length[0], 45, ST7735_height/2-1, WHITE);
      print_bar(length[1], 65, ST7735_height/2-1, YELLOW);
      print_bar(length[2], 85, ST7735_height/2-1, CYAN);
      break;
    case 1:
      pt_graph(tilting, CYAN);
      f3d_lcd_drawString(0,0, "Accel", MAGENTA, bg_color);
      break;
    case 2:
      cp_graph(compass(mag,tilting), RED);
      f3d_lcd_drawString(0,0, "Compass", MAGENTA, bg_color);
      break;
    case 3: 
      pt_graph(ang_nun, YELLOW);
      f3d_lcd_drawString(0,0, "Nunchuk", MAGENTA, bg_color);
      break;
    default:
      printf("not valid\n");
    }
  }
}
      
 
void check_main_screen() {
  switch(switch_prog) {
  case 0: 
	
    f3d_lcd_fillScreen2(bg_color);

    // set up of axis labels
    f3d_lcd_drawChar(44, ST7735_height/2+7,'x', WHITE, bg_color);
    f3d_lcd_drawChar(64, ST7735_height/2+7,'y', WHITE, bg_color);
    f3d_lcd_drawChar(84, ST7735_height/2+7,'z', WHITE, bg_color);
	
    f3d_lcd_drawString(10, 
		       ST7735_height/2+20,
		       "Gyroscope Analysis",
		       MAGENTA,
		       bg_color);
    break;
  case 1:
    f3d_lcd_fillScreen2(bg_color);
    draw_axis(WHITE);
    f3d_lcd_drawString(0,0, "Accel", MAGENTA, bg_color);
    break;
  case 2: 
    f3d_lcd_fillScreen2(bg_color);
    draw_axis(WHITE);
    f3d_lcd_drawString(0,0, "Compass", MAGENTA, bg_color);
    break;
  case 3:
    f3d_lcd_fillScreen2(bg_color);
    draw_axis(WHITE);
    f3d_lcd_drawString(0,0, "Nunchuk", MAGENTA, bg_color);
    break;
  default: 
    f3d_lcd_fillScreen2(bg_color);
    break;
  }
}



// graphs the compass
void cp_graph(float val, uint16_t col) {	
  // previous location
  int prev_x = mag_x;
  int prev_y = mag_y;

  // provides conversion to pixel coordinates based on the quadrant location
  if (val < 0) {
    if (val > M_PI/-2) {
      mag_x = (int) (0 - fabs(radius*sinf(fabs(val))));
      mag_y = (int) (0 - fabs(radius*cosf(fabs(val))));
    } else {
      mag_x = (int) (0 - fabs(radius*cosf(fabs(val+M_PI/2))));
      mag_y = (int) (fabs(radius*sinf(fabs(val+M_PI/2))));
    }
  } else {
    if (val < M_PI/2) {
      mag_x = (int) (fabs(radius*sinf(fabs(val))));
      mag_y = (int) (0 - fabs(radius*cosf(fabs(val))));
    } else {
      mag_x = (int) (fabs(radius*cosf(fabs(val-M_PI/2))));
      mag_y = (int) (fabs(radius*sinf(fabs(val-M_PI/2))));
    }
  }

  // scales to origin at center
  mag_x += (ST7735_width-1)/2;
  mag_y = (ST7735_height-1)/2 - mag_y;
  // undoes previous pointer and redraws
  f3d_lcd_drawChar(prev_x, prev_y, '*', bg_color, bg_color);
  draw_axis(WHITE);
  f3d_lcd_drawChar(mag_x, mag_y,'*', col, bg_color);
 
  // prevents extreme flashing
  delay(100);
}

// graphs tilt location
void pt_graph(float * val, uint16_t col) { 
  
  // stores previous points
  int prev_x = curr.x;
  int prev_y = curr.y;


  // converts to pixels based on trig and quadrant location
  if (val[2] < 0) {
    if (val[1] < 0) {
      curr.x = (ST7735_width-1)/2 + roll_slope * (M_PI + val[1]);
    } else {
      curr.x = (ST7735_width-1)/2 - roll_slope * (M_PI - val[1]);
    }
  } else {
    if (val [1] < 0) {
      curr.x = (ST7735_width-1)/2 - roll_slope * (val[1]);
    } else {
      curr.x = (ST7735_width-1)/2 - roll_slope * (val[1]);
    }
  }

  if (val[2] < 0) {
    if (val[0] < 0) {
      curr.y = (ST7735_height-1)/2 + pitch_slope * (M_PI + val[0]);
    } else {
      curr.y = (ST7735_height-1)/2 - pitch_slope * (M_PI - val[0]);
    }
  } else {
    if (val [0] < 0) {
      curr.y = (ST7735_height-1)/2 - pitch_slope * (val[0]);
    } else {
      curr.y = (ST7735_height-1)/2 - pitch_slope * (val[0]);
    }
  }

  // checks if need to reprint
  if (prev_x == curr.x && prev_y == curr.y) {

  } else {

    f3d_lcd_drawChar(prev_x, prev_y, '*', bg_color, bg_color);

    draw_axis(WHITE);

    f3d_lcd_drawChar(curr.x, curr.y,'*', col, bg_color);
  }
}


// draws axes		   
void draw_axis(uint16_t col) {
  uint8_t x, y;
  y = (ST7735_height-1)/2;

  for(x = 0; x < ST7735_width; x++){
    f3d_lcd_drawPixel(x, y, col);
  }
  x = (ST7735_width-1)/2;
  for (y = 0; y < ST7735_height; y++) {
    f3d_lcd_drawPixel(x, y, col);
  }
}

//x axis
void pitch(float * val, float * ans) {
  ans[0] = (atan2f(val[0],sqrtf(val[1]*val[1]+val[2]*val[2])));
}

//y axis
void roll(float * val, float * ans) {
  ans[1] = (atan2f(val[1],sqrtf(val[0]*val[0]+val[2]*val[2])));
}

//z axis
void tilt(float * val, float * ans) {
  ans[2] = (atan2f(val[2],sqrtf(val[1]*val[1]+val[0]*val[0])));
  
}

// grabs compass data based on tilt and mag reading
float compass(float * mag, float * tilting) {
  
  return (atan2f(mag[0]*sin(tilting[0]) + 
		 mag[1]*cos(tilting[1]) - 
		 mag[2]*sin(tilting[1])*cos(tilting[0])
		 ,
		 mag[0]*cos(tilting[0]) + 
		 mag[2]*sin(tilting[0])));
  
  // return atan2f(mag[1], mag[0]);

}


// gyro Functions

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



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */