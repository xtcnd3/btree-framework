#!/bin/sh

NUMTESTS=`find $1/* -type f | grep cmdline.txt | wc -l`

RESULT_FILE_SET=`find $1/* -type f | grep cmdline.txt | sed 's#cmdline.txt#result.txt#g'`

FAILED=0
PASSED=0
MISSING=0

for RESULT_FILE in $RESULT_FILE_SET
do
	if [ -f $RESULT_FILE ]; then
		PASS=`cat $RESULT_FILE | grep -e "^0$" | wc -l`

		PASSED=`expr $PASSED + $PASS`
	else
		echo "missing: "$RESULT_FILE

		MISSING=`expr $MISSING + 1`
	fi
done

for RESULT_FILE in $RESULT_FILE_SET
do
	if [ -f $RESULT_FILE ]; then
		RETURN_CODE=`cat $RESULT_FILE`

		if [ "$RETURN_CODE" == "" ]; then
			RETURN_CODE="return code not present!"
		fi

		if [ "$RETURN_CODE" != 0 ]; then
			echo "failed: "$RESULT_FILE

			FAILED=`expr $FAILED + 1`
		fi
	fi
done

if [ $MISSING -ne 0 ]; then
	echo -n $MISSING
	echo -n " of "
	echo -n $NUMTESTS
	echo " test results are missing!"
fi

if [ $FAILED -ne 0 ]; then
	echo -n $FAILED
	echo -n " of "
	echo -n $NUMTESTS
	echo " tests failed!"
fi

echo -n $PASSED
echo -n " of "
echo -n $NUMTESTS
echo " tests passed!"

