#!/bin/bash

test_to_remove='test_file.txt'

touch "$test_to_remove"
test_out=`LD_PRELOAD=$(pwd)/libwrapper.so rm "$test_to_remove"`
if test -f "$test_to_remove"
then
	rm "$test_to_remove"
	exit 1
else
	exit 0
fi


