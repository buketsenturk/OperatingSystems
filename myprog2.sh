#!/bin/bash

echo "Enter a sequence of numbers followed by 'end'"  #informing user in terminal

                                            
maximum=0         #to

while :           #infinite loop
	do
		read Input                  #to read the data entered into the terminal
		if [ $Input == "end" ]      #to exit
		then
			break               #to exit
		fi
		if [ $Input -gt $maximum ]    # control statement between terminal and maximum  
		then
			maximum=$Input               
		fi	    
		
	done
echo "Maximum : $maximum"                     #to show the result

