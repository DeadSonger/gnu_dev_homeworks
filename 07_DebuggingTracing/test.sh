#!/bin/bash

echo "Testing:"

has_error=0

echo -ne "\t Test simple version: \t\t\t"
test_out=`./main`
if [ "$test_out" == "Hello, I'm main!" ]; then echo PASSED; else echo FAILED; has_error=1; fi

echo -ne "\t Test sign version: \t\t\t"
test_out=`LD_PRELOAD=$(pwd)/libwrapper_sign.so ./main | wc -l`
if [ "$test_out" > 32 ]; then echo PASSED; else echo FAILED; has_error=1; fi

echo -ne "\t Test custom version: \t\t\t"
simple_out=`./main`
sample=`printf "%s\n%s\n" '"Custom text"' "$simple_out"`
test_out=`LD_PRELOAD=$(pwd)/libwrapper_custom.so ./main`
if [ "$test_out" == "$sample" ]; then echo PASSED; else echo FAILED; has_error=1; fi

echo -ne "\t Test args version without args: \t"
test_out=`LD_PRELOAD=$(pwd)/libwrapper_args.so ./main | grep argc`
if [ "$test_out" == "Main info: argc = 1" ]; then echo PASSED; else echo FAILED; has_error=1; fi

echo -ne "\t Test args version with args: \t\t"
test_out=`LD_PRELOAD=$(pwd)/libwrapper_args.so ./main a b c | grep argc`
if [ "$test_out" == "Main info: argc = 4" ]; then echo PASSED; else echo FAILED; has_error=1; fi

exit $has_error

