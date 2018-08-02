Report - Lab 6
Priya Saravanan 
psaravan

## Questions
1. Printf will print out whatever the code is at the point where the printf is located. One benefit could be that the user can see what the value is during
 that part of the code but it doesn't provide the functionality that a debugger provides (like breaking) and going to the next step will be hard because
 there would have to be a lot of printf statements
2. The function is called in the higher level so that users don't have to make the initializations or deal with it. 
3. LCD_CS_DEASSERT() deasserts the chips and reset lines so it stops communications between master and slave. 
4. glcdfont.c stores pixel representations of ascii characters. Then drawchar goes through every bit in the ascii character pixel representation and draws that on the screnn until the whole character is drawn.
5. Time 1: 10.17
Time 2: 3.5
Differnece: 6.6
fillScreen2 function creates an array that "colors" a whole lines instead of fillScreen that just puts the color pixel by pixel.

## Difficulties
The initializations for the pins were quite confusing and took us a while. The other difficult part was getting the display right on the LCD screen. That took a while to do. 