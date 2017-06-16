myvm: myvm.c myvm.h
	gcc -o $@ myvm.c

clean: 
	rm  myvm