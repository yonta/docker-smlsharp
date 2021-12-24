.PHONY: all push

all:
	$(MAKE) -C buster-slim
	$(MAKE) -C bullseye-slim

push:
	$(MAKE) push -C buster-slim
	$(MAKE) push -C bullseye-slim
