NETWORK_NAME:=slamnet
SLAM_NAME:=slam
SLAM_INPUT_NAME:=slam_input

LOGFILE:=logs.out

setup_network:
	@if ! docker network inspect $(NETWORK_NAME); then \
		docker network create $(NETWORK_NAME); \
	fi

setup: setup_network

build_input:
	docker build -t $(SLAM_INPUT_NAME) -f docker/Dockerfile.input .

build_slam:
	docker build -t $(SLAM_NAME) -f docker/Dockerfile.slam .

build: setup build_input build_slam

run_slam:
	docker run --rm --name slam_container --network $(NETWORK_NAME) $(SLAM_NAME) > $(LOGFILE)

run_slam_bg:
	docker run --rm -d --name slam_container --network $(NETWORK_NAME) $(SLAM_NAME) 

run: run_slam

run_input: run_slam_bg
	docker run --rm -d --name $(SLAM_INPUT_NAME)_container --network $(NETWORK_NAME) $(SLAM_INPUT_NAME)

