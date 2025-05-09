.PHONY: all
all: build test

CXX=clang++

.PHONY: build
build: BatterySpoof/build/arduino.renesas_uno.minima/BatterySpoof.ino.elf

.PHONY: init
init:
	arduino-cli core install arduino:renesas_uno

extra_compilation_flags =
ifdef OSCILLATE_VOLTAGE
	extra_compilation_flags = "-DOSCILLATE_VOLTAGE"
endif

fqbn = arduino:renesas_uno:minima

BatterySpoof/build/arduino.renesas_uno.minima/BatterySpoof.ino.elf: BatterySpoof/BatterySpoof.ino BatterySpoof/spoof.hpp BatterySpoof/spoof.cpp
	arduino-cli compile --fqbn $(fqbn) BatterySpoof --export-binaries --warnings all \
		--build-property compiler.cpp.extra_flags=$(extra_compilation_flags)

.PHONY: install
install: build
	arduino-cli upload -p $$(arduino-cli board list | grep arduino:renesas_uno:minima | cut -f 1 -d ' ') \
		--fqbn $(fqbn) BatterySpoof --build-path ./BatterySpoof/build/$(subst :,.,$(fqbn))

.PHONY: clean
clean: clean-test
	rm -rf BatterySpoof/build

.PHONY: clean-test
clean-test:
	rm -rf BatterySpoof/test/build

.PHONY: format
format:
	find BatterySpoof/ -iname '*.ino' -o -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i

.PHONY: check
check: format build test

.PHONY: test
test: clean-test build-test run-test

.PHONY: build-test
build-test: BatterySpoof/test/build/test

BatterySpoof/test/build/test: BatterySpoof/test/test.cpp BatterySpoof/spoof.hpp BatterySpoof/spoof.cpp
	mkdir -p BatterySpoof/test/build
	$(CXX) BatterySpoof/test/test.cpp BatterySpoof/spoof.cpp -o BatterySpoof/test/build/test \
		$(extra_compilation_flags)

.PHONY: run-test
run-test: build-test
	./BatterySpoof/test/build/test

.PHONY: list
list:
	arduino-cli board list
