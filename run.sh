#!/bin/sh
a=0

while [ $a -lt $2 ]
do
	echo ./main 1 `expr 33 - $a `
	time ./main 1 `expr 33 - $a `
    a=`expr $a + 1`
done