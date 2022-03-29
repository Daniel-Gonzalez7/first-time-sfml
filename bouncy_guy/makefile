APP			= sfml-app
CC			= g++
SRCS		= main.cpp
OBJS		:= $(SRCS: .cpp=.o)
LINKS		= -lsfml-graphics -lsfml-window -lsfml-system -lGL #GL is for opengl

sfml-app: main.o
	$(CC) $(OBJS) -o  $(APP) $(LINKS)

main.o: main.cpp
	$(CC) -c $(SRCS)

run: $(APP)
	./$(APP) $(ARGS)

clean:
	rm sfml-app *.o