#!/bin/bash
counter=0
if [ $# -eq 1 ]
then
   for i in $1/*                        #when the path is entered from the terminal
	do
	if [ -s $i ]; then              #file size is greater than zero.
	continue
	else
	counter=$((counter+1))     #empty file and to delete
	rm -rf $i
	fi
	done
echo "$counter zero-length files are removed from the directory: $1"    #size of total deleting files 

else
	for i in *			#when the path is not entered
	do
	if [ -s $i ]; then
	continue
	else
	counter=$((counter+1))
	rm -rf $i
	fi
	done
echo "$counter zero-length files are removed from the current directory"
fi









