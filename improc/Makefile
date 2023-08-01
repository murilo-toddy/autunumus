BUILD_DIRECTORY=build
MAIN=main

all:
	cmake -B $(BUILD_DIRECTORY)

compile:
	make -C $(BUILD_DIRECTORY)

run:
	./$(BUILD_DIRECTORY)/$(MAIN)

exec: compile run

setup_run: all exec 

