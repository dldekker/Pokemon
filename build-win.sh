gcc sprite.c pokemon.c player.c battle_queue.c battle.c \
pokemon_data_list.c item_data_list.c item.c ability.c \
ability_data_list.c move_data_list.c move.c tiles.c main.c \
util.c -L./lib -I./include -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lglew32 \
-lopengl32 -opokemon.exe -g

