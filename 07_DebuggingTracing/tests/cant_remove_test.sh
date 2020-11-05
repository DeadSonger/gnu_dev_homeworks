#!/bin/bash

test_to_cant_remove='test_FIX.txt'

touch "$test_to_cant_remove"
test_out=`LD_PRELOAD=$(pwd)/libwrapper.so rm "$test_to_cant_remove" &>1`
status=$?
echo status = $status
if [ $status -ne 0 ] && [ -f "$test_to_cant_remove" ] 
then
	rm "$test_to_cant_remove"
	exit 0
else
	exit 1
fi


