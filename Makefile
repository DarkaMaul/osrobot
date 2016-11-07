#Thanks to the wallev3 project

INC_DIR = -I ./ev3c -I ./includes -I ./ev3c/include
OBJ_DIR = obj/
BIN_DIR = bin/
C_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix $(OBJ_DIR),$(notdir $(C_FILES:.c=.o)))

CFLAGS = $(INC_DIR) -Wall
LDFLAGS = -lm -lbluetooth -pthread

bin/testOsRobot: ev3c.a $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) ev3c/lib/ev3c.a

bin/osRobot: ev3c.a $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) ev3c/lib/ev3c.a $(LDFLAGS)

ev3c.a:
	@$(MAKE) -C ev3c -s

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $^

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(BIN_DIR)

.PHONY:	clean bin/osRobot bin/testOsRobot
.DEFAULT: bin/testOsRobot
