APP			= mendelbrot.exe
CC			= g++
SRCS		= mendelbrot.cpp
OBJS		:= $(SRCS: .cpp=.o)
LINKS		= -lsfml-graphics -lsfml-window -lsfml-system -lGL #GL is for opengl

$(APP): mendelbrot.o
	$(CC) $(OBJS) -o $(APP) $(LINKS)

mendelbrot.o: mendelbrot.cpp
	$(CC) -c $(SRCS)

run: $(APP)
	./$(APP) $(ARGS)

clean:
	rm mendelbrot.exe *.o
	