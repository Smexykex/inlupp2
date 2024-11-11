CC=gcc
FLAGS=-pedantic -Wall -g
TEST_FILES=test_files/add_test.txt \
           test_files/remove_test.txt \
           test_files/edit_test.txt \
		   test_files/replenish_test.txt \
		   test_files/cart_add_test.txt \
		   test_files/cart_remove_test.txt

%.o: %.c %.h
	$(CC) $(FLAGS) $< -c

store: store.o ui.o db.o ask_question.o common.o linked_list.o iterator.o hash_table.o entry.o
	$(CC) $(FLAGS) $^ -o $@

db_tests: db_tests.c db.o ui.o ask_question.o common.o linked_list.o iterator.o hash_table.o entry.o
	$(CC) $(FLAGS) $^ -lcunit -o $@

testdb: db_tests
	./db_tests

memtestdb: db_tests
	valgrind --leak-check=full ./db_tests

testdbcoverage: db_tests.c common.o linked_list.o iterator.o hash_table.o entry.o
	$(CC) $(FLAGS) --coverage $^ -lcunit -o db_tests db.c && \
	./db_tests && gcov -adcfu db_tests-db.gcda db_tests-db.gcno

ui_tests: ui_tests.c db.o ui.o ask_question.o common.o linked_list.o iterator.o hash_table.o entry.o
	$(CC) $(FLAGS) $^ -lcunit -o $@

all_tests.txt: $(TEST_FILES)
	cat $(TEST_FILES) > ./test_files/all_tests.txt

testui: ui_tests all_tests.txt  
	./ui_tests < ./test_files/all_tests.txt

memtestui: ui_tests all_tests.txt
	valgrind --leak-check=full ./ui_tests < ./test_files/all_tests.txt

# Check leaks on macOS
testdbleaks: testdb
	leaks -quiet -atExit -- ./db_tests

testuileaks: testui
	leaks -quiet -atExit -- ./ui_tests < ./test_files/all_tests.txt

clean:
	rm -rf *.o db_tests ui_tests ./test_files/all_tests.txt store *.dSYM \
	*.gcno *.gcda *.gcov

.PHONY: clean testdb memtestdb memtestui
