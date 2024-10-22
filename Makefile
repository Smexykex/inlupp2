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

uitest: common.o db.o utils.o hash_table.o
	$(CC) $(FLAGS) ui.c common.c db.c utils.c hash_table.c entry.c linked_list.c iterator.c -o ui.out

clean:
	rm *.o data_base_tests
	
.PHONY: clean tests memtests
