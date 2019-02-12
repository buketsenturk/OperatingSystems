#!/bin/bash


while read numbers; do   #to read number in file   
	counter=0        #to total stars
	while [ $counter -ne $numbers ]    #to synchronize the number of stars to the number in the file
	do
		printf "*"
		counter=$((counter+1))    
	done
	echo ""	       #to move to the new line for the stars when move to the new line in the file 
done < "$1.txt"        #to retrieve the file name from the terminal           
