all: build unit
.PHONY: all

build:
	echo TODO
.PHONY: build

unit:
	ctest -R test-Core
	ctest -R test-src
.PHONY: unit
