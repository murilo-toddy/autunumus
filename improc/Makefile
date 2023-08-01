BUILD_DIRECTORY=build
MAIN=main

all:
	cmake -B $(BUILD_DIRECTORY)

compile:
	make -C $(BUILD_DIRECTORY)

run:
	./$(BUILD_DIRECTORY)/$(MAIN)

run_sample:
	./$(BUILD_DIRECTORY)/$(MAIN) sample

exec: compile run

exec_sample: compile run_sample

setup_run: all exec 

setup_run_sample: all exec_sample

