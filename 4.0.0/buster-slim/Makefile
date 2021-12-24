.PHONY: all push

all: Dockefile
	docker build --file $< --tag yonta44/smlsharp:4.0.0-buster-slim .

push:
	docker push yonta44/smlsharp:4.0.0-buster-slim
