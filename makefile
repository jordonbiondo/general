

test/general_tests: test/general_tests.c src/general.c
	gcc -g -std=gnu99 -Wall -Werror test/general_tests.c -o test/general_tests

test: test/general_tests

run-test:
	./test/general_tests -v | ./test/greatest/greenest
