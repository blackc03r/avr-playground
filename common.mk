ifdef NANO
	MCU = atmega168
	USBDEV = /dev/ttyUSB0
	BAUDS = 57600
endif

ifdef TINY
	MCU = attiny85
# attiny85 goes up to 20mhz, but the chips coming out of the factory are apparently clocked at 1mhz
# by default.
	F_CPU = 1000000UL
	AVRDUDEMCU = t85
	AVRDUDEARGS = -c usbtiny -P usb 
endif

MCU ?= atmega328p
F_CPU ?=16000000UL
CC = avr-gcc
CFLAGS = -Os -Wall -DF_CPU=$(F_CPU) -mmcu=$(MCU) -c
LDFLAGS = -mmcu=$(MCU)
OBJS ?= main.o

USBDEV ?= /dev/ttyACM0
BAUDS ?= 115200
AVRDUDEMCU ?= $(MCU)
AVRDUDEARGS ?= -F -V -c arduino -P $(USBDEV) -b $(BAUDS)

# Patterns

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Rules

.PHONY: send clean

all: $(PROGNAME).hex
	@echo Done!

send: $(PROGNAME).hex
	avrdude $(AVRDUDEARGS) -p $(AVRDUDEMCU) -U flash:w:$(PROGNAME).hex

clean:
	rm -f $(OBJS) $(PROGNAME).hex $(PROGNAME).bin

$(PROGNAME).bin: $(OBJS)
	$(CC) $(LDFLAGS) $+ -o $@

$(PROGNAME).hex: $(PROGNAME).bin
	avr-objcopy -O ihex -R .eeprom $< $@

