LIBRARY = ds1307
VERSION = 0.0.5

ifeq ($(OS),Darwin)
	PREFIX = /usr/local/$(LIBRARY)/$(VERSION)
	AVRM = /usr/local/avrm/0.0.5
else
	PREFIX = /usr/avr/$(LIBRARY)/$(VERSION)
	AVRM = /usr/avr/avrm/0.0.5
endif
DEPENDENCIES = $(PREFIX) $(AVRM)

include $(AVRM)/share/Makefile
