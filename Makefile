CC=gcc
FLAGS=-pedantic -Wall -g

%.o: %.c %.h
	$(CC) $(FLAGS) $< -c

data_base_tests: db_tests.c db.o ui.o utils.o common.o linked_list.o iterator.o hash_table.o entry.o
	$(CC) $(FLAGS) $^ -lcunit -o $@

tests: data_base_tests test.txt
	./data_base_tests < test.txt

memtests: data_base_tests test.txt
	valgrind --leak-check=full ./data_base_tests < test.txt

clean:
	rm *.o data_base_tests
	
.PHONY: clean tests memtests
