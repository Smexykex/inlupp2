# IOOPM Inlupp 2 Simple Webstore Backend

## Build instructions

- `make store` builds the program to the executable file `store`.

- `make testdb` builds and runs the test suite for the database functions.

- `make testui` builds and runs the test suite for the interface functions.

- `make memtestdb` (MacOS `make testdbleaks`) builds and runs the database
  functions test suite under a leak checking program.

- `make memtestui` (MacOS `make testuileaks`) builds and runs the interface
  functions test suite under a leak checking program.

- `make clean` deletes all generated files from the directory.

## Note on test coverage

The test suite has a coverage of 97.96%. The only function without 100% coverage
is `location_equals` which is a trivial function that simply compares the shelf
strings of two locations.

## Instructions for use

- After building the program using the build instructions above, the program can
  be launched from the executable `store`.

- The program simulates a simple webstore backend where items can be added and
  allocated stock in particualar locations.

- Shopping carts can also be created, which each represent a user instance, the
  program tracks the available stock of an item by reserving items that are in
  shopping carts so that it is always possible for all carts to check out.
