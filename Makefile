CC=gcc
FLAGS=-pedantic -Wall -g
TEST_FILES=test_files/add_test.txt \
           test_files/remove_test.txt \
           test_files/edit_test.txt \
					 test_files/replenish_test.txt

%.o: %.c %.h
	$(CC) $(FLAGS) $< -c

data_base_tests: db_tests.c db.o ui.o utils.o common.o linked_list.o iterator.o hash_table.o entry.o
	$(CC) $(FLAGS) $^ -lcunit -o $@

testdb: data_base_tests
	./data_base_tests

memtestdb: data_base_tests
	valgrind --leak-check=full ./data_base_tests

ui_tests: ui_tests.c db.o ui.o utils.o common.o linked_list.o iterator.o hash_table.o entry.o
	$(CC) $(FLAGS) $^ -lcunit -o $@

all_tests.txt: $(TEST_FILES)
	cat $(TEST_FILES) > ./test_files/all_tests.txt

testui: ui_tests all_tests.txt  
	./ui_tests < ./test_files/all_tests.txt

memtestui: ui_tests all_tests.txt
	valgrind --leak-check=full ./ui_tests < ./test_files/all_tests.txt

# Check leaks on macOS
testdbleaks: testdb
	leaks -quiet -atExit -- ./data_base_tests

testuileaks: testui
	leaks -quiet -atExit -- ./ui_tests < ./test_files/all_tests.txt

clean:
	rm *.o data_base_tests ui_tests

.PHONY: clean testdb memtestdb memtestui
