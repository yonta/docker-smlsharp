.PHONY: all slim-latest run-slim clean

all: all-slim

slim-latest:
	docker build --file Dockerfile.slim --tag yonta44/smlsharp:latest .

run-slim: slim-latest
	docker run --interactive --tty --rm yonta44/smlsharp:latest

clean:
