CC = avr-gcc
CFLAGS = -Os -DF_CPU=16000000UL -mmcu=atmega328p -c
LDFLAGS = -mmcu=atmega328p

.PHONY: send

all: serial.hex
	@echo Done!

send: serial.hex
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$<

serial.hex: serial
	avr-objcopy -O ihex -R .eeprom $< $@

serial: main.o
	$(CC) $(LDFLAGS) $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<
