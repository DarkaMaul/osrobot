#Thanks to the wallev3 project

INC_DIR = -I ./lib -I ./includes
OBJ_DIR = obj/
C_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix $(OBJ_DIR),$(notdir $(C_FILES:.c=.o)))

CFLAGS = $(INC_DIR) -Wall
LDFLAGS = -lm -lbluetooth -pthread

osRobot: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ lib/ev3c.a $(LDFLAGS)

obj/%.o: src/%.c
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $^

clean:
	rm -rf $(OBJ_DIR)
	rm -f wallev3

.PHONY:	clean osRobot
	
