CC=gcc
FLAGS=-pedantic -Wall -g

inlupp1_common.o: common.c common.h
	$(CC) $(FLAGS) $< -c -o inlupp1_common.o

linked_list.o: linked_list.c linked_list.h
	$(CC) $(FLAGS) $< -c

iterator.o: iterator.c iterator.h
	$(CC) $(FLAGS) $< -c

hash_table.o: hash_table.c hash_table.h
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
