#!/bin/sh

OUTFILE=$1
BINARY=$2
BASEDIR=$3

LIST_TESTS=`find $BASEDIR/* -type f | grep "cmdline.txt"`
LIST_RESULTS=`echo $LIST_TESTS | sed 's#cmdline.txt#result.txt#g'`

echo -n "tests: " > $OUTFILE
echo $LIST_RESULTS >> $OUTFILE
echo "" >> $OUTFILE

for TEST in $LIST_TESTS
do
	RESULT=`echo $TEST | sed 's#cmdline.txt#result.txt#g'`
	TESTDIR=`echo $TEST | sed 's#cmdline.txt##g'`
	VALIDATION=`echo $TEST | grep validation | wc -l`
	VALGRIND=`echo $TEST | grep valgrind | wc -l`

	# dos2unix has been attached for the script to work with any line ending under unix/linux and cygwin
	CMDLINE=`cat $TEST | dos2unix`

	echo -n $RESULT >> $OUTFILE
	echo -n ": " >> $OUTFILE
	echo $TEST >> $OUTFILE

	echo -ne "\\t" >> $OUTFILE
	echo -n "cd " >> $OUTFILE
	echo -n $TESTDIR >> $OUTFILE
	echo "; \\" >> $OUTFILE

	echo -ne "\\t" >> $OUTFILE
	echo -n $BINARY >> $OUTFILE
	echo -n " " >> $OUTFILE
	echo -n $CMDLINE >> $OUTFILE
	echo "; \\" >> $OUTFILE

	echo -ne "\\t" >> $OUTFILE
	echo "echo \$\$? > result.txt" >> $OUTFILE

	if [ $VALIDATION -ne 0 ]; then
		if [ $VALGRIND -ne 0 ]; then
			echo -ne "\\t" >> $OUTFILE
			echo "cat $TESTDIR./valgrind.log > valgrind_report.txt" >> $OUTFILE

			echo -ne "\\t" >> $OUTFILE
			echo "rm -fv $TESTDIR./valgrind.log" >> $OUTFILE
		fi
	fi

	echo "" >> $OUTFILE
done

echo "clean: " >> $OUTFILE
echo -ne "\\t " >> $OUTFILE
echo -n "rm -fv " >> $OUTFILE
echo $LIST_RESULTS >> $OUTFILE
echo "" >> $OUTFILE

echo "clean_failed: " >> $OUTFILE
echo -ne "\\t " >> $OUTFILE
echo "\$(eval \$@FAILED_TESTS := \$(shell find ./* -type f | grep result.txt | xargs grep -v -e \"0\" -f | sed -e 's#:.*\$\$##g'))" >> $OUTFILE

echo -ne "\\t " >> $OUTFILE
echo -n "rm -fv \$(\$@FAILED_TESTS)" >> $OUTFILE
echo "" >> $OUTFILE

echo "" >> $OUTFILE
echo ".PHONY: clean clean_failed tests" >> $OUTFILE
echo "" >> $OUTFILE

