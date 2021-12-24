.PHONY: all push

all: Dockerfile
	docker build --file $< --tag yonta44/smlsharp:3.5.0-buster-slim .

push:
	docker push yonta44/smlsharp:3.5.0-buster-slim
