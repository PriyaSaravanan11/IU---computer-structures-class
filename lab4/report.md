Priya Saravanan
psaravan
report.md
2/9/2018

1) 3 - TX, RX and ground
2) Alternate functions are for the connection of other peripherals rather than using GPIO pins
3) baud rate: the reciprocal of the period which is the time it takes to transfer one bit
4) we need that because newlibs are independent of operating systems. printf won't work since stubs are needed for the board to be able to do what getchar() and putchar() does. 
5) Probably when we are working with ports and want to see if any data is being transmitted,  Salae can be run to analyze to visualize the data. If it shows some bits, then the ports are working. 
6) Difficulties: We had dificulties trying to modify the wordcount file for this lab. we then had some difficulties with getting the Salae output too but it turns out we plugged the wires into the analyzer the wrong way. Looking at the visualizer was interesting. 