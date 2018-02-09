set terminal png nocrop enhanced font "verdana,8" size 640,300
set xlabel "Temperature"

set ylabel "Specific Heat"
set output "TC.png"
p 'TC.dat' w errorbars 

set ylabel "Energy"
set output "TE.png"
p 'TE.dat' w errorbars

set ylabel "|Magnetisation|"
set output "TM.png"
p 'TM.dat' w errorbars

set ylabel "Susceptibility"
set output "TX.png"
p 'TX.dat' w errorbars