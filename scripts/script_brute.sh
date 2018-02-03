#!/bin/sh
echo "Starting Script file \n\n" > "output_brute.txt"
for FILE in *.graph
do
		echo "Running on $FILE \n\n" >> "output_brute.txt"
        timeout 30m ./bruteforce < "$FILE" >> "output_brute.txt"
        echo "\n Time Limit Exceeded on $FILE \n\n" >> "output_brute.txt"
done
