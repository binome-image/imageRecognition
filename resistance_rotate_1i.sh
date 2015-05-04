#!/bin/bash
rm -f resultat/rotate/$1-$2_rotate_ind$3.dat
echo "resultat image $1-$2.pgm" >> resultat/rotate/$1-$2_rotate_ind$3.dat

for angle in `seq 1 20`;do
ANGLE=`echo "$angle *0.15" | bc`
	echo ANGLE=$ANGLE
	./build/imgRotate -i database/$1/$1-$2.pgm -o tmp.pgm -n $ANGLE 2> /dev/null
	res=$(./build/main -ind$3 tmp.pgm)
	echo res=$res
	echo $ANGLE $res>> resultat/rotate/$1-$2_rotate_ind$3.dat
done
