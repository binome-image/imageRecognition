#!/bin/bash

rm -f resultat_cat.dat
rm -f resultat_liste.dat
echo "resultat image" >> resultat_cat_ind1_acc.dat
echo "resultat image" >> resultat_liste_ind1_acc.dat
numero=1
for entree in `ls database/`;do
	
	echo $entree
	min=0
	max=0
		for exemple in `ls database/$entree/`;do
		echo $exemple
		res=$(./build/PGMReader database/$entree/$exemple)
		echo $exemple $res >> resultat_liste_ind1_acc.dat
			st1=`echo "$min == 0" | bc`
			if [  "$st1" -eq "1" ]; then
				min=`echo $res`
			else
  				st2=`echo "$res < $min" | bc`
					if [  "$st2" -eq "1" ]; then
						min=`echo $res`
					fi
			fi
		st2=`echo "$res < $max" | bc`
			if [  "$st2" -eq "0" ]; then
				max=`echo $res`
			fi
		done
	echo $numero $entree $min $max>> resultat_cat_ind1_acc.dat
	numero=`echo "$numero+1" | bc`
done


