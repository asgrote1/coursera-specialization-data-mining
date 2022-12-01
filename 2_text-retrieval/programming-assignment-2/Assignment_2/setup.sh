#!/bin/bash

if [ ! -d ../meta ]
then
	echo "Error. Make sure the folder meta is in the same directory and rerun this script"
	exit 1
fi

echo "MeTA Found"

echo "Copying Assignment 2 data files to MeTA"

\cp -f Data/moocs-judging-queries.txt Data/moocs-qrels.txt Data/moocs-queries.txt ../meta/data/moocs/
if [ ! $? -eq 0 ]; then echo "Error. Make sure the script has enough writing privileges"; exit 1; fi

\mkdir build
\cp -rf Assignment2 build/
if [ ! $? -eq 0 ]; then echo "Error. Make sure the script has enough writing privileges"; exit 1; fi

echo "Files copied successfully!"
exit 0
