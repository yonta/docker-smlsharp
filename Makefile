.PHONY: all all-slim run-slim clean

all: all-slim

slim:
	docker build --file Dockerfile.slim --tag smlsharp:buster-slim .

run-slim: slim
	docker run --interactive --tty --rm smlsharp:buster-slim

clean:
