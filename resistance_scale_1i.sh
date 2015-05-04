#!/bin/bash
rm -f resultat/scale/$1-$2_scale_ind$3.dat
echo "resultat image $1-$2.pgm" >> resultat/scale/$1-$2_scale_ind$3.dat

for scale in `seq 1 20`;do
SCALE=`echo "$scale * 0.25" | bc`
	echo SCALE=$SCALE
	./build/imgScale -i database/$1/$1-$2.pgm -o tmp.pgm -s $SCALE 2> /dev/null
	res=$(./build/main -ind$3 tmp.pgm)
	echo res=$res
	echo $SCALE $res>> resultat/scale/$1-$2_scale_ind$3.dat
done
