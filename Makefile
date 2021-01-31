.PHONY: all all-slim run-slim clean

all: all-slim

slim:
	docker build -f Dockerfile.slim -t smlsharp:buster-slim .

run-slim: slim
	docker run -it smlsharp:buster-slim

clean:
