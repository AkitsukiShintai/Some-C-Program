VALGRIND_OPTIONS=-q --leak-check=full
OSTYPE := $(shell uname)
	ifeq ($(OSTYPE),Linux)
	CYGWIN=
	else
	CYGWIN=-Wl,--enable-auto-import
endif

gcc_c:
	echo Compile all with C
	gcc -c splitter.c -Wall -Wextra -ansi -pedantic
	gcc -c driver.c -Wall -Wextra -ansi -pedantic
	gcc -o $@.exe splitter.o driver.o -Wall -Wextra -ansi -pedantic

gcc_c_cpp:
	echo Compile with C and C++
	gcc -c splitter.c -Wall -Wextra -ansi -pedantic
	g++ -c driver.cpp -Wall -Wextra  -ansi -pedantic
	g++ -o $@.exe splitter.o driver.o $(CYGWIN) -Wall -Wextra  -ansi -pedantic

gcc_cpp:
	echo Compile all with C++
	g++ -c splitter.c -Wall -Wextra -ansi -pedantic
	g++ -c driver.cpp -Wall -Wextra -ansi -pedantic
	g++ -o $@.exe splitter.o driver.o $(CYGWIN) -Wall -Wextra -ansi -pedantic

drmem:
	g++ -fpermissive -m32 -g -fno-inline -fno-omit-frame-pointer splitter.c driver.cpp -o $@.exe

#bin/drmemory.pl -- ./myapp args_to_myapp
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16:
	echo "running test$@"
	watchdog 200 ./$(PRG) $@ | grep "+++" >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@
mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8 mem9 mem10 mem11 mem12 mem13 mem14 mem15 mem16:
	echo "running memory test $@"
	watchdog 1000 valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null 2>difference$@

clean:
	rm -f *.o *.exe 
