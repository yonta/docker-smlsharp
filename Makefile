.PHONY: all all-slim run-slim clean

all: all-slim

slim:
	docker build -f Dockerfile.slim -t smlsharp:buster-slim .

run-slim: slim
	docker run -it smlsharp:buster-slim

alpine:
	docker build -f Dockerfile.alpine -t smlsharp:alpine-3.13.1 .

run-alpine: alpine
	docker run -it smlsharp:alpine-3.13.1 ash

clean:
