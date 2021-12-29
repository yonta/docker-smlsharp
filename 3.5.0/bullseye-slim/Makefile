.PHONY: all push

all: Dockerfile
	docker build --file $< --tag yonta44/smlsharp:3.5.0-bullseye-slim .
	docker tag yonta44/smlsharp:3.5.0-bullseye-slim yonta44/smlsharp:3.5.0

push:
	docker push yonta44/smlsharp:3.5.0-bullseye-slim
	docker push yonta44/smlsharp:3.5.0
