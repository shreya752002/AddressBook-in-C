# Project4 (Intro to C) - CS2110: Fall 2020
# GCC flags from the syllabus (each flag described for the curious minds!)
# Flag info credit: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html

# Flags code is expected to compile with
CFLAGS =
CFLAGS += -std=c99 # Using the C99 standard
CFLAGS += -Wall # This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros
CFLAGS += -pedantic # Issue all the warnings demanded by strict ISO C and ISO C++; reject all programs that use forbidden extensions, and some other programs that do not follow ISO C and ISO C++
CFLAGS += -Wextra # This enables some extra warning flags that are not enabled by -Wall
CFLAGS += -Werror # Make all warnings into errors
CFLAGS += -O0 # Reduce compilation time and make debugging produce the expected results. This is the default.
CFLAGS += -Wstrict-prototypes # Warn if a function is declared or defined without specifying the argument types
CFLAGS += -Wold-style-definition # Warn if an old-style function definition is used. A warning is given even if there is a previous prototype
CFLAGS += -g # Generate debugging information
CFLAGS += -Werror=vla # Generate an error if variable-length arrays (bad practice in C!) are used
CFLAGS += -fcf-protection=none # Workaround for a bug that can make debugging annoying: https://stackoverflow.com/questions/64697087/gdb-shows-incorrect-arguments-of-functions-for-stack-frames

.PHONY: student clean run-tests tests run-case run-gdb run-valgrind

# Name of Test Executable
TEST_EXEC = tests
ARCH = $(shell uname -m)

STUDENT_C_FILES = \
	book.c

# Source files to compile the test suite
TEST_OFILES = \
	test/$(ARCH)/main.o \
	test/book_suite.c \
	test/$(ARCH)/test_utils.o

# Libraries needed for Check
CHECK_LIBS = $(shell pkg-config --cflags --libs check)

# Add current directory to the list of directories to be searched for header files during preprocessing
INCLUDE = -I .
INCLUDE += -include test/fake_malloc.h

run-tests: tests
	@ ./$(TEST_EXEC)

tests: clean
	@ gcc -fno-asm -static $(CFLAGS) $(STUDENT_C_FILES) $(INCLUDE) $(TEST_OFILES) $(CHECK_LIBS) -o $(TEST_EXEC)

# Run main.c -- Used to run student defined main method
student: clean
	@ gcc -fno-asm -static $(CFLAGS) $(STUDENT_C_FILES) main.c
	@ ./a.out

# Clean Project
clean:
	@rm -f *.o *.out $(TEST_EXEC)

# To run a specific test case (or all similar to tests)
run-case: tests
	@ ./$(TEST_EXEC) $(TEST)

# To run gdb on testcase(s)
run-gdb: tests
	@ CK_FORK=no gdb --args ./$(TEST_EXEC) $(TEST)

# To prevent leak false postives, tell check not to fork.
# See: http://check.sourceforge.net/doc/check_html/check_4.html#Finding-Memory-Leaks
run-valgrind: clean
	@ gcc -fno-asm $(CFLAGS) $(STUDENT_C_FILES) $(INCLUDE) $(TEST_OFILES) $(CHECK_LIBS) -o $(TEST_EXEC)
	@ CK_FORK=no valgrind --quiet --leak-check=full --error-exitcode=1 --show-leak-kinds=all --errors-for-leak-kinds=all --undef-value-errors=no ./$(TEST_EXEC) $(TEST)

