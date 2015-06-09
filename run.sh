#!/bin/sh
a=1

while [ $a -lt $2 ]
do
	echo ./main $1 $a
	timeout 900 time ./main $1 $a
    a=`expr $a + 1`
done