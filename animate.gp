# Set a title.
set title "Spins"

# Remove border.
unset border

# Remove x and y ticks.
unset xtics
unset ytics

# Remove color gradient.
unset key
unset surface
unset colorbox

# For ascpect ratio.
set size square

# Set spin colours.
set palette defined (-1 "red", 1 "blue")

# This stops an error when all spins are the same.
set cbrange[-1:1]

# Do the plot.
p filename matrix with image
while(1){
	replot
	pause 0.3
}