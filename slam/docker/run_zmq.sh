#!/bin/bash
docker build -t zmq_source -f Dockerfile.source ..
docker build -t zmq_sink -f Dockerfile.sink ..
docker build -t slam -f Dockerfile.slam ..
docker run --rm -d --name zmq_source_container --network zmq zmq_source
# docker run --rm -d --name zmq_sink_container --network zmq zmq_sink
docker run --rm -d --name slam_container --network zmq slam 

