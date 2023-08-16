setup_network:
	docker network create slamnet

setup: setup_network

build_mock:
	docker build -t slam_mock -f docker/Dockerfile.mock .

build_slam:
	docker build -t slam -f docker/Dockerfile.slam .

build: build_mock build_slam

run_mock:
	docker run --rm -d --name slam_mock_container --network slamnet slam_mock

run_slam:
	docker run --rm --name slam_container --network slamnet slam > logs.txt

run_slam_bg:
	docker run --rm -d --name slam_container --network slamnet slam 

run: run_mock run_slam

