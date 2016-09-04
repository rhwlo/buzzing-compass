VIRTUALENV = venv
VIRTUALENV_BIN = $(VIRTUALENV)/bin
PLATFORMIO = $(VIRTUALENV)/bin/platformio
ATTINY_FLAGS = -e trinket3
ARDUINO_FLAGS = -e uno
ATTINY_BRANCH = attiny
UNO_BRANCH = uno
ATTINY_DEPS = lib/TinyWireM lib/Adafruit_Sensor

$(VIRTUALENV):
	virtualenv $(VIRTUALENV)

$(PLATFORMIO): $(VIRTUALENV)
	$(VIRTUALENV_BIN)/pip install platformio

attiny_build: $(PLATFORMIO) $(ATTINY_DEPS)
	git checkout $(ATTINY_BRANCH)
	$(PLATFORMIO) run $(ATTINY_FLAGS)

arduino_build: $(PLATFORMIO)
	git checkout $(UNO_BRANCH)
	$(PLATFORMIO) run $(ARDUINO_FLAGS)

attiny_upload: $(PLATFORMIO)
	git checkout $(ATTINY_BRANCH)
	avrdude -c usbtiny -p attiny85 -U flash:w:.pioenvs/trinket3/firmware.hex

lib/TinyWireM:
	git checkout $(ATTINY_BRANCH)
	git clone https://github.com/Adafruit/TinyWireM lib/TinyWireM

lib/Adafruit_Sensor:
	git checkout $(ATTINY_BRANCH)
	git clone https://github.com/Adafruit/Adafruit_Sensor lib/Adafruit_Sensor
