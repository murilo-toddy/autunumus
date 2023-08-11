build:
	docker build -t zmq_source -f docker/Dockerfile.source .
	docker build -t zmq_sink -f docker/Dockerfile.sink .
	docker build -t slam -f docker/Dockerfile.slam .

run:
	docker run --rm -d --name zmq_source_container --network zmq zmq_source
	# docker run --rm -d --name zmq_sink_container --network zmq zmq_sink
	docker run --rm -d --name slam_container --network zmq slam 

