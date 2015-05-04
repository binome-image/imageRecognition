clear
reset
unset key
# Make the x axis labels easier to read.
set xtics rotate out
# Select histogram data
set style data histogram
# Give the bars a plain fill pattern, and draw a solid line around them.
set style fill solid border

set fontsize 25

set term pdfcairo
set output "ind1.pdf"
set title "Intervalle résultat pour l'indicateurs 1"
set xlabel "Classes"
set ylabel "Valeur de l'indicateur 1"
set pointsize 0.001
set boxwidth 0.5
set style fill solid
plot "resultat_cat_ind1.dat" using 1:($3+$4)/2:3:4:xticlabels(2) with errorbars

