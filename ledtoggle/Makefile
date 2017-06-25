CC = avr-gcc
CFLAGS = -Os -DF_CPU=16000000UL -mmcu=atmega328p -c
LDFLAGS = -mmcu=atmega328p

.PHONY: send

all: ledtoggle.hex
	@echo Done!

send: ledtoggle.hex
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$<

ledtoggle.hex: ledtoggle
	avr-objcopy -O ihex -R .eeprom $< $@

ledtoggle: main.o
	$(CC) $(LDFLAGS) $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<
