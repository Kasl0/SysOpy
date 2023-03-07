PROGRAM = main

all:
	make test_static_O0
	make test_static_Os
	make test_static_O3

	make test_shared_O0
	make test_shared_Os
	make test_shared_O3

	make test_dynamic_O0
	make test_dynamic_Os
	make test_dynamic_O3

test_static_O0:
	../Zad2/$(PROGRAM)_static_O0 < input.txt > results_static_O0.txt
test_static_Os:
	../Zad2/$(PROGRAM)_static_Os < input.txt > results_static_Os.txt
test_static_O3:
	../Zad2/$(PROGRAM)_static_O3 < input.txt > results_static_O3.txt

test_shared_O0:
	../Zad2/$(PROGRAM)_shared_O0 < input.txt > results_shared_O0.txt
test_shared_Os:
	../Zad2/$(PROGRAM)_shared_Os < input.txt > results_shared_Os.txt
test_shared_O3:
	../Zad2/$(PROGRAM)_shared_O3 < input.txt > results_shared_O3.txt

test_dynamic_O0:
	../Zad2/$(PROGRAM)_dynamic_O0 < input.txt > results_dynamic_O0.txt
test_dynamic_Os:
	../Zad2/$(PROGRAM)_dynamic_Os < input.txt > results_dynamic_Os.txt
test_dynamic_O3:
	../Zad2/$(PROGRAM)_dynamic_O3 < input.txt > results_dynamic_O3.txt

clean:
	rm -f *.a *.o *.so results_*
