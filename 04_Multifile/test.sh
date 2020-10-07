#!/bin/bash

progs=$@

echo $progs

for p in $progs; do
	./$p &> test_$p"_out_without_params"
	./$p 1 &> test_$p"_out_with_1_param"
	./$p 1 2 3 &> test_$p"_out_with_3_params"
done

for test_case in 'without_params' 'with_1_param' 'with_3_params'; do
	result=true
	for p in $progs; do
		result=[[ $result ]] && [[ `cmp test_$p"_out_"$test_case test_$1"_out_"$test_case` ]] 
	done
	if [[ $result ]]; then
		echo -e Test Case: $test_case "\t\t\t" DONE
	else
		echo -e Test Case: $test_case "\t\t\t" FAIL
	fi
done

for p in $progs; do
	rm -f test_$p"_out_"*
done

