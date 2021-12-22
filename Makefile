.PHONY: all slim-latest slim-3.7.1 run-slim clean

all: slim-latest

slim-latest:
	docker build --file Dockerfile.slim --tag yonta44/smlsharp:4.0.0-buster-slim .
	docker tag yonta44/smlsharp:4.0.0-buster-slim yonta44/smlsharp:4.0.0
	docker tag yonta44/smlsharp:4.0.0-buster-slim yonta44/smlsharp:latest

slim-3.7.1:
	docker build --file Dockerfile.slim --build-arg smlsharp_version='3.7.1' --build-arg sources_url='https://www.pllab.riec.tohoku.ac.jp/smlsharp/download/debian/dists/buster/smlsharp.list' --tag yonta44/smlsharp:3.7.1-buster-slim .
	docker tag yonta44/smlsharp:3.7.1-buster-slim yonta44/smlsharp:3.7.1

run-slim: slim-latest
	docker run --interactive --tty --rm yonta44/smlsharp:latest

clean:
