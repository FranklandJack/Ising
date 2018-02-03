# Give plot a title.
set title 'Energy on Each Configuration'

# Label axis
set ylabel '<E>'

# Label axis
set xlabel 'Configuration'

# Plot data.
plot filename using 1:2

# Plot data to a .png file and save it.
set terminal png
set out 'energy.png'
plot filename using 1:2 with lines

# Reset gnuplot preferences.
reset