CC=gcc
FLAGS=-pedantic -Wall -g

%.o: %.c %.h
	$(CC) $(FLAGS) $< -c

data_base_tests: db_tests.c db.o ui.o utils.o common.o linked_list.o iterator.o hash_table.o entry.o
	$(CC) $(FLAGS) $^ -lcunit -o $@

tests: data_base_tests test.txt
	./data_base_tests < test.txt

ui_tests: ui_tests.c db.o ui.o utils.o common.o linked_list.o iterator.o hash_table.o entry.o
	$(CC) $(FLAGS) $^ -lcunit -o $@

testui: ui_tests ui_testfile.txt
	./ui_tests < ui_testfile.txt

# Check leaks on macOS
testuileaks: ui_tests ui_testfile.txt
	leaks -quiet -atExit -- ./ui_tests < ui_testfile.txt

memtests: data_base_tests test.txt
	valgrind --leak-check=full ./data_base_tests < test.txt

clean:
	rm *.o data_base_tests ui_tests

.PHONY: clean tests memtests
