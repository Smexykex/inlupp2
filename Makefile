CC=gcc
FLAGS=-pedantic -Wall -g

inlupp1_common.o: ../inlupp1/common.c ../inlupp1/common.h
	$(CC) $(FLAGS) $< -c -o inlupp1_common.o

linked_list.o: ../inlupp1/linked_list.c ../inlupp1/linked_list.h
	$(CC) $(FLAGS) $< -c

iterator.o: ../inlupp1/iterator.c ../inlupp1/iterator.h
	$(CC) $(FLAGS) $< -c

hash_table.o: ../inlupp1/hash_table.c ../inlupp1/hash_table.h
	$(CC) $(FLAGS) $< -c

%.o: %.c %.h
	$(CC) $(FLAGS) $< -c

data_base_tests: db_tests.c db.o ui.o utils.o common.o inlupp1_common.o linked_list.o iterator.o hash_table.o
	$(CC) $(FLAGS) $^ -lcunit -o $@

tests: data_base_tests test.txt
	./data_base_tests < test.txt

memtests: data_base_tests test.txt
	valgrind --leak-check=full ./data_base_tests < test.txt

clean:
	rm *.o data_base_tests
	
.PHONY: clean tests memtests