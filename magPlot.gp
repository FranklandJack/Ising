# Give plot a title.
set title 'Magnetisation on Each Configuration'

# Label axis
set ylabel '<M>'

# Label axis
set xlabel 'Configuration'

# Plot data.
plot filename using 1:2

# Plot data to a .png file and save it.
set terminal png
set out 'magnetisation.png'
plot filename using 1:2 with lines

# Reset gnuplot preferences.
reset