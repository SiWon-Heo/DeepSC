#!/bin/bash
FILENAME0="_0.c"
FILENAME1="_1.c"

mkdir ../data/chrome_debian/parsed_10to19
mkdir ../data/chrome_debian/raw_code_10to19

for i in {10..19}
do
	./slicer.sh raw_code $i$FILENAME0 1 $i$FILENAME0/
	./slicer.sh raw_code $i$FILENAME1 1 $i$FILENAME1/
	mv parsed/tmp/$i$FILENAME0 ../data/chrome_debian/parsed_10to19/
	mv parsed/tmp/$i$FILENAME1 ../data/chrome_debian/parsed_10to19/
	mv tmp/$i$FILENAME0.c ../data/chrome_debian/raw_code_10to19/
	mv tmp/$i$FILENAME1.c ../data/chrome_debian/raw_code_10to19/
	rm -r *.c
done



