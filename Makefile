VIRTUALENV = venv
VIRTUALENV_BIN = $(VIRTUALENV)/bin
PLATFORMIO = $(VIRTUALENV)/bin/platformio
ATTINY_FLAGS = -e trinket3
ARDUINO_FLAGS = -e uno
ATTINY_BRANCH = attiny
UNO_BRANCH = uno

$(VIRTUALENV):
	virtualenv $(VIRTUALENV)

$(PLATFORMIO): $(VIRTUALENV)
	$(VIRTUALENV_BIN)/pip install platformio

attiny_build: $(PLATFORMIO)
	git checkout $(ATTINY_BRANCH)
	$(PLATFORMIO) $(ATTINY_FLAGS)

arduino_build: $(PLATFORMIO)
	git checkout $(UNO_BRANCH)
	$(PLATFORMIO) $(ARDUINO_FLAGS)
