#!/bin/bash

print_header()
{
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 71 ]; then
		padding=$(((71 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

result1=0
result2=0
result=0
ZERO=0
VALGRIND="valgrind --leak-check=full --track-origins=yes -q --log-file=rezultat_valgrind.txt"
BEST="100"
NO_TESTS=0

print_header "Tema 3"

echo "=================================== README ==================================="
if test -f "README"; then
	echo "+0.0: Verificare README ................................................. PASS"
	result=$(awk "BEGIN {print $result+5; exit}")
else
	echo "-5.0: Verificare README ................................................ FAIL"
fi
make build > /dev/null 2>&1

#Cerința 1 
echo "================================= CERINTA 1 =================================="
for i in {0..9}
do
	fileIn="input/cerinta1/test"$i".in"
	fileRef="ref/cerinta1/test"$i".ref"
	fileOut="tema3.out"
    fileOutDirectory="output/cerinta1/test"$i".out"
	cp "$fileIn" "tema3.in"
        rm $fileOut > /dev/null 2>&1
	timeout 20 ./tema3 1 > /dev/null 2>&1
    cp $fileOut $fileOutDirectory
	diff <(head -n 1 $fileOut) <(head -n 1 $fileRef) -w > /dev/null
	EXIT_CODE=$?
	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+0.0: Cerința 1 a) Test $idx .............................................. PASS"
		result=$(awk "BEGIN {print $result+1.5; exit}")
		result1=$(awk "BEGIN {print $result1+1.5; exit}")
	else
		echo "-1.5.0: Cerința 1 a) Test $idx .............................................. FAIL"
        print $EXIT_CODE
	fi

	diff <(tail -n +1 $fileOut) <(tail -n +1 $fileRef) -w > /dev/null
	EXIT_CODE=$?
	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+0.0: Cerința 1 b) Test $idx .............................................. PASS"
		result=$(awk "BEGIN {print $result+2.5; exit}")
		result1=$(awk "BEGIN {print $result1+2.5; exit}")
	else
		echo "-2.5.0: Cerința 1 b) Test $idx .............................................. FAIL"
        print $EXIT_CODE
	fi
done

rm $fileOut

#Cerința 2 
echo "================================= CERINTA 2 =================================="
for i in {0..9}
do
	fileIn="input/cerinta2/test"$i".in"
	fileRef="ref/cerinta2/test"$i".ref"
	fileOut="tema3.out"
	fileOutDirectory="output/cerinta2/test"$i".out"
	cp "$fileIn" "tema3.in"
        rm $fileOut > /dev/null 2>&1
	timeout 20 ./tema3 2 > /dev/null 2>&1
	cp $fileOut $fileOutDirectory
	diff $fileOut $fileRef -w > /dev/null
	EXIT_CODE=$?
	if (( i < 10 )); then
		idx=" $i"
	else
		idx=$i
	fi
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+0.0: Cerința 2 Test $idx ................................................. PASS"
		result=$(awk "BEGIN {print $result+4; exit}")
		result2=$(awk "BEGIN {print $result2+4; exit}")
	else
		echo "-4.0: Cerința 2 Test $idx ................................................. FAIL"
		print $EXIT_CODE
	fi
done


echo "================================== VALGRIND =================================="
no_of_valgrind="1"
valgrind_not_ok="0"

for i in {0..9}
do
	fileIn="input/cerinta1/test"$i".in"
	fileOut="tema3.out"
	cp "$fileIn" "tema3.in"
	$VALGRIND ./tema3 1 > /dev/null 2>&1
	if [[ ! -z $(cat rezultat_valgrind.txt) ]]; then
		echo "-0.0: VALGRIND Cerinta 1 Test $i ....................................... FAIL"
		no_of_valgrind=$(awk "BEGIN {print "0"; exit}")
	else
		printf "+0.0: VALGRIND Cerinta 1 Test $i ......................................... PASS\n"
	fi

	fileIn="input/cerinta2/test"$i".in"
	fileOut="tema3.out"
	cp "$fileIn" "tema3.in"
	$VALGRIND ./tema3 2 > /dev/null 2>&1
	if [[ ! -z $(cat rezultat_valgrind.txt) ]]; then
		echo "-0.0: VALGRIND Cerinta 2 Test $i .......................................... FAIL"
		no_of_valgrind=$(awk "BEGIN {print "0"; exit}")
	else
		printf "+0.0: VALGRIND Cerinta 2 Test $i ......................................... PASS\n"
	fi
done

echo "=================================== BONUS ===================================="
if [[ ! $no_of_valgrind -eq 0 ]]; then
	printf "+0.0: BONUS VALGRIND .................................................... PASS\n"
    result=$(awk "BEGIN {print $result+20; exit}")
else
	echo "-20.0: BONUS VALGRIND ................................................... FAIL"
fi

echo "================================ CHECKSTYLE =================================="
printf "+0.0: CHECKSTYLE ........................................................ PASS\n"
result=$(awk "BEGIN {print $result+15; exit}")

echo "================================= REZULTATE =================================="
printf "Cerința 1 : "$result1"\n"
printf "Cerința 2 : "$result2"\n"
printf "Punctajul Acumulat este: "$result"/100p!\n"
make clean > /dev/null 2>&1
