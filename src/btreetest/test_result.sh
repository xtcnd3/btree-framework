#!/bin/sh

RESULTS=`find $1/* -type f | grep result.txt | xargs grep -v -e "0" -f | sed -e 's#:.*$##g'`
NUMTESTS=`find $1/* -type f | grep cmdline.txt | wc -l`

FAILS=0

for RESULT in $RESULTS
do
	echo $RESULT

	FAILS=`expr $FAILS + 1`
done

echo -n $FAILS
echo -n " of "
echo -n $NUMTESTS
echo " tests failed!"

