all:
	gcc pokemon.c player.c battle_queue.c battle.c pokemon_data_list.c item_data_list.c item.c ability.c ability_data_list.c move_data_list.c move.c tiles.c main.c util.c `sdl2-config --cflags --libs` -lm -lSDL2_image -osdl -lGLEW -lGL -lassimp -lGLU -g
