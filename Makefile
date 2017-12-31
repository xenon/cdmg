all:
#.PHONY: all
	$(MAKE) -C src

test:
#.PHONY: test
	$(MAKE) -C test

.PHONY: clean
clean:
	$(MAKE) clean -C src
