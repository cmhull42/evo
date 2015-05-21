CC = g++
CFLAGS =  -g -c -Wall
LDFLAGS =
SOURCES = evo.cpp
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = evo

CFLAGS += `pkg-config --cflags glfw3 glew --static`
LDFLAGS += `pkg-config --libs glfw3 glew --static`

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
