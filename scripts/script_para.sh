#!/bin/sh
echo "Starting Script file \n\n" > "output_para.txt"
for FILE in *.graph
do
		echo "Running on $FILE \n\n" >> "output_para.txt"
        timeout 30 ./prog < "$FILE" >> "output_para.txt"
        echo "\n Time Limit Exceeded on $FILE \n\n" >> "output_para.txt"
done
