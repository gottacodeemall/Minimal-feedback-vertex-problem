#!/bin/sh
echo "Starting Script file \n\n" > "output_para.txt"
for FILE in 'ls *.graph | sort -k 5n'
do
	echo "Running on $FILE \n\n" >> "output_para.txt"
        timeout 20m ./prog < "$FILE" >> "output_para.txt"
        echo "\n Time Limit Exceeded on $FILE \n\n" >> "output_para.txt"
done
