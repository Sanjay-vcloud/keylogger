
CC := gcc
OBJECTS = keytrace.o main.o
trace: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

keytrace.o : logger.c 
	$(CC) $(CFLAGS) -c $< -o $@

main.o : main.c logger.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) trace
	rm -f *.out
	rm -f *.o