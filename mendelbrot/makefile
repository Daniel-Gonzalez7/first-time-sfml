APP			= mandelbrot.exe
CC			= g++
SRCS		= mandelbrot.cpp
OBJS		:= $(SRCS: .cpp=.o)
LINKS		= -lsfml-graphics -lsfml-window -lsfml-system -lGL #GL is for opengl

$(APP): mandelbrot.o
	$(CC) $(OBJS) -o $(APP) $(LINKS)

mandelbrot.o: mandelbrot.cpp
	$(CC) -c $(SRCS)

run: $(APP)
	./$(APP) $(ARGS)

clean:
	rm mandelbrot.exe *.o
	