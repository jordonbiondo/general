

test/general_tests: test/general_tests.c src/object.c src/object.h
	gcc -g -std=gnu99 -flto -o3 -Wall -Werror test/general_tests.c -o test/general_tests

test: test/general_tests

run-test:
	./test/general_tests -v 
