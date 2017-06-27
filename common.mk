CC = avr-gcc
CFLAGS = -Os -DF_CPU=16000000UL -mmcu=atmega328p -c
LDFLAGS = -mmcu=atmega328p

# Patterns

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

%.bin:
	$(CC) $(LDFLAGS) $+ -o $@

%.hex: %.bin
	avr-objcopy -O ihex -R .eeprom $< $@

# Rules

.PHONY: send clean

all: $(PROGNAME).hex
	@echo Done!

send: $(PROGNAME).hex
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$(PROGNAME).hex

clean:
	rm *.o *.hex *.bin

$(PROGNAME).hex: $(PROGNAME).bin
$(PROGNAME).bin: main.o
main.o: main.c
