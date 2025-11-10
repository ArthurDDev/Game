TARGET = game
SOURCES = src/*.c
ALLEGRO_FLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

all:
	gcc $(SOURCES) -o $(TARGET) $(ALLEGRO_FLAGS)

play:
	rm -rf ./build
	mkdir ./build/
	gcc $(SOURCES) -o ./build/$(TARGET) $(ALLEGRO_FLAGS)
	./build/$(TARGET)
	rm -rf ./build

valgrind:
	rm -rf ./build
	mkdir ./build/
	gcc $(SOURCES) -o ./build/$(TARGET) $(ALLEGRO_FLAGS)
	valgrind ./build/$(TARGET)
	rm -rf ./build

clean:
	rm $(TARGET) ./*.o