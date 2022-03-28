APP			= chaos.exe
CC			= g++
SRCS		= chaos.cpp
OBJS		:= $(SRCS: .cpp=.o)
LINKS		= -lsfml-graphics -lsfml-window -lsfml-system -lGL #GL is for opengl

$(APP): chaos.o
	$(CC) $(OBJS) -o $(APP) $(LINKS)

chaos.o: chaos.cpp
	$(CC) -c -std=c++2a $(SRCS)

run: $(APP)
	./$(APP) $(ARGS)

clean:
	rm chaos.exe *.o
	