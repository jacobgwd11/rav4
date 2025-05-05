.PHONY: build
build: BatterySpoof/build/arduino.renesas_uno.minima/BatterySpoof.ino.elf

.PHONY: init
init:
	arduino-cli core install arduino:renesas_uno

BatterySpoof/build/arduino.renesas_uno.minima/BatterySpoof.ino.elf: BatterySpoof/BatterySpoof.ino BatterySpoof/spoof.h BatterySpoof/spoof.cpp
	arduino-cli compile --fqbn arduino:renesas_uno:minima BatterySpoof --export-binaries --warnings all

.PHONY: clean
clean:
	rm -rf BatterySpoof/build
	rm -rf BatterySpoof/test/build

.PHONY: format
format:
	find BatterySpoof/ -iname '*.ino' -o -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i

.PHONY: check
check: format build test

.PHONY: test
test: build-test run-test

.PHONY: build-test
build-test: BatterySpoof/test/build/test

BatterySpoof/test/build/test: BatterySpoof/test/test.cpp BatterySpoof/spoof.h BatterySpoof/spoof.cpp	
	mkdir -p BatterySpoof/test/build
	clang++ BatterySpoof/test/test.cpp BatterySpoof/spoof.cpp -o BatterySpoof/test/build/test

.PHONY: run-test
run-test: build-test
	./BatterySpoof/test/build/test

.PHONY: list
list:
	arduino-cli board list
