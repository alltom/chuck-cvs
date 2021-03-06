#!/bin/sh 

# chuck_autotest [files...]

# this will run chuck on every file passes as an argument
# the "test" is whether the last line of output is the string "success"

# on pass, it will print the filename, success message, and the first line
# of the file as a comment

# on fail, it prints an error message
# then spits out the chuck output
# and the body of the .ck file with numbered lines

# note, any file not printing "success" fails

test_output="chuck_autotest_log.txt"
tmp_file="/tmp/chuck_test"
chuck_bin="chuck"

# simple header

while [ `echo $1 | cut -c1` = "-" ]
do
    if [ $1 = "--body" ] 
    then
      catbody="yes"
    elif [ $1 = "--out" ]
    then
      shift
      test_output=$1
    elif [ $1 = "--bin" ]
    then
      shift
      chuck_bin=$1
    fi
    shift
done


echo '---chuck auto-test log---' > $test_output
date >> $test_output
echo host=$HOST >> $test_output
echo machine=$HOSTTYPE:$VENDOR:$OSTYPE >> $test_output
echo '-------------------------' >> $test_output

#loop on all input files

for file in $@
do

#execute the test

echo "$file...";

#..timeout killer ( unix only )
#./autotest_timer --bin $chuck_bin 60 $file & $chuck_bin $file 2> $tmp_file

$chuck_bin $file 2> $tmp_file


#check the output for the correct flags

lastline=`tail -n 1 $tmp_file`

comment=`head -n 1 $file`

failtrue=`echo $comment | grep -n 'fail-correct' | cut -c1`

if [ "$lastline" = "\"success\" : (string)" ] && [ "$failtrue" = "" ]
then 
    echo "     - test succeeded"
    echo "$file		success			$comment" >> $test_output
elif [ $failtrue ]
then
    echo "     - test succeeded ( fail-correct )"
    echo "$file		success (fail-correct)	$comment" >> $test_output
else 
    echo "     - test failed"
    echo "$file		error			$comment" >> $test_output
    echo "  >>> $file test output" >> $test_output
    sed -e 's/^/	/' $tmp_file >> $test_output
    if [ $catbody ]
    then
	echo "  >>> $file contents" >> $test_output
	cat -n $file >> $test_output
        echo "" >> $test_output
    fi
    echo "  >>> $file end error message" >> $test_output
fi
done
