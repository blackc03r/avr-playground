ifdef NANO
	MCU = atmega168
	USBDEV = /dev/ttyUSB0
	BAUDS = 57600
endif

MCU ?= atmega328p
F_CPU ?=16000000UL
CC = avr-gcc
CFLAGS = -Os -Wall -DF_CPU=$(F_CPU) -mmcu=$(MCU) -c
LDFLAGS = -mmcu=$(MCU)
OBJS ?= main.o

USBDEV ?= /dev/ttyACM0
BAUDS ?= 115200

# Patterns

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Rules

.PHONY: send clean

all: $(PROGNAME).hex
	@echo Done!

send: $(PROGNAME).hex
	avrdude -F -V -c arduino -p $(MCU) -P $(USBDEV) -b $(BAUDS) -U flash:w:$(PROGNAME).hex

clean:
	rm -f $(OBJS) $(PROGNAME).hex $(PROGNAME).bin

$(PROGNAME).bin: $(OBJS)
	$(CC) $(LDFLAGS) $+ -o $@

$(PROGNAME).hex: $(PROGNAME).bin
	avr-objcopy -O ihex -R .eeprom $< $@

