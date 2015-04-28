set term pdfcairo
set output "courbe1.pdf"
set pointsize 0.001
set boxwidth 0.5
set style fill solid
plot "resultat_cat_ind1_acc.dat" using 1:($3+$4)/2:3:4 with errorbars
