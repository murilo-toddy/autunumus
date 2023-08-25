NETWORK_NAME:=slamnet
SLAM_CONTAINER_NAME:=slam
SLAM_INPUT_CONTAINER_NAME:=slam_input

setup_network:
	@if ! docker network inspect $(NETWORK_NAME); then \
		docker network create $(NETWORK_NAME); \
	fi

setup: setup_network

build_input:
	docker build -t $(SLAM_INPUT_CONTAINER_NAME) -f docker/Dockerfile.mock .

build_slam:
	docker build -t $(SLAM_CONTAINER_NAME) -f docker/Dockerfile.slam .

build: setup_network build_mock build_slam

run_mock:
	docker run --rm -d --name slam_mock_container --network $(NETWORK_NAME) slam_mock

run_slam:
	docker run --rm --name slam_container --network $(NETWORK_NAME) $(SLAM_CONTAINER_NAME) > logs.txt

run_slam_bg:
	docker run --rm -d --name slam_container --network $(NETWORK_NAME) $(SLAM_CONTAINER_NAME) 

run: run_mock run_slam

