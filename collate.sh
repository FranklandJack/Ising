# If the data files that contain the collated data do not already exists create them.
rm TE.dat
rm TM.dat
rm TX.dat
rm TC.dat

touch TE.dat
touch TM.dat
touch TX.dat
touch TC.dat

# Iterate through each set of results.
for results in "$@"*
do
	# Get the temperature from the input file and append it to each collated data file.s
	awk '/^Temperature: /{printf $NF}' $results/input.txt >> TE.dat
	awk '/^Temperature: /{printf $NF}' $results/input.txt >> TM.dat
	awk '/^Temperature: /{printf $NF}' $results/input.txt >> TX.dat
	awk '/^Temperature: /{printf $NF}' $results/input.txt >> TC.dat

	# Put an empty column in each file.
	printf " " >> TE.dat
	printf " " >> TM.dat
	printf " " >> TX.dat
	printf " " >> TC.dat
 
	# Get the values and error of E, M, X and C from the results file and append them with the temperature.
	awk '/^E: /{printf $(NF-2)}' $results/results.txt >> TE.dat
	# Put an empty column in each file.
	printf " " >> TE.dat
	awk '/^E: /{print $(NF)}' $results/results.txt >> TE.dat

	awk '/^M: /{printf $(NF-2)}' $results/results.txt >> TM.dat
	# Put an empty column in each file.
	printf " " >> TM.dat
	awk '/^M: /{print $(NF)}' $results/results.txt >> TM.dat

	awk '/^X: /{printf $(NF-2)}' $results/results.txt >> TX.dat
	# Put an empty column in each file.
	printf " " >> TX.dat
	awk '/^X: /{print $(NF)}' $results/results.txt >> TX.dat 

	awk '/^C: /{printf $(NF-2)}' $results/results.txt >> TC.dat
	# Put an empty column in each file.
	printf " " >> TC.dat
	awk '/^C: /{print $(NF)}' $results/results.txt >> TC.dat

done

# Remove duplicate lines from files.
sort -u TE.dat -o TE.dat
sort -u TM.dat -o TM.dat
sort -u TX.dat -o TX.dat
sort -u TC.dat -o TC.dat