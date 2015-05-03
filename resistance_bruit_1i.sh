
#!/bin/bash
rm -f resultat/bruit/$1_$2_bruit.dat
echo "resultat image $1-$2.pgm" >> resultat/bruit/$1_$2_bruit.dat

for bruit in `seq 1 19`;do
NOISE=`echo "$bruit *0.05" | bc`
	echo NOISE=$NOISE
	./build/imgAddNoise -i database/$1/$1-$2.pgm -o tmp.pgm -n 0$NOISE 2> /dev/null
	res=$(./build/main -ind$3 tmp.pgm)
	echo res=$res
	echo $NOISE $res>> resultat/bruit/$1_$2_bruit.dat
done

