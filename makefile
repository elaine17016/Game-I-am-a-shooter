ALLEGRO_LIBRARIES := allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_video-5
ALLEGRO_FLAGS := $(shell pkg-config --cflags --libs $(ALLEGRO_LIBRARIES)) -lallegro  #need to add -lallegro_main to let main to be seen ,i dont know why 
# when error:yld[14393]: symbol not found in flat namespace '__al_mangled_main' Abort trap: 6  occurs ,just remove  -lallegro_main

CC := gcc
OUT:= game
MAIN:= mac_main.o GameWindow.o scene.o global.o object.o

all: $(MAIN)
	$(CC) $(MAIN) -o $(OUT) $(ALLEGRO_FLAGS)
    
clean: 
	rm $(OUT)
