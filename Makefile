all : test
test : test.cpp Coroutine.cpp Scheduler.cpp 
	g++ -g  -o $@ $^ 

clean :
	rm test
