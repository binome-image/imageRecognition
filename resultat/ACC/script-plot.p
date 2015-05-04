clear
reset
unset key
# Make the x axis labels easier to read.
set xtics rotate out
# Select histogram data
set style data histogram
# Give the bars a plain fill pattern, and draw a solid line around them.
set style fill solid border

set xtics font "Bold,8"
set ytics font "Bold,8"
set term pdfcairo
set output "ind3.pdf"
set title "Intervalle résultat pour l'indicateurs 3"
set xlabel "Classes"
set ylabel "Valeur de l'indicateur 3"
set pointsize 0.001
set boxwidth 0.5
set style fill solid
plot "resultat_cat_ind3_acc.dat" using 1:($3+$4)/2:3:4:xticlabels(2) with errorbars

