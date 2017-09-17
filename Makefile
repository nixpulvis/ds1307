LIBRARY = ds1307
VERSION = 0.0.3
PREFIX = /usr/local/$(LIBRARY)/$(VERSION)

AVRM = /usr/local/avrm/0.0.5
DEPENDENCIES = $(PREFIX) $(AVRM)

include $(AVRM)/Makefile
