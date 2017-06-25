CC = avr-gcc
CFLAGS = -Os -DF_CPU=16000000UL -mmcu=atmega328p -c
LDFLAGS = -mmcu=atmega328p

.PHONY: send

all: blink.hex
	@echo Done!

send: blink.hex
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$<

blink.hex: blink
	avr-objcopy -O ihex -R .eeprom $< $@

blink: main.o
	$(CC) $(LDFLAGS) $< -o $@
main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<
