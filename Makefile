.PHONY: all slim-latest run-slim clean

all: all-slim

slim-latest:
	docker build --file Dockerfile.slim --tag smlsharp:latest .

run-slim: slim
	docker run --interactive --tty --rm smlsharp:buster-slim

clean:
