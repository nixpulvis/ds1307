LIBRARY = ds1307
VERSION = 0.0.3
PREFIX = /usr/local/Cellar/$(LIBRARY)/$(VERSION)

AVRM = /usr/local/Cellar/avrm/0.0.5
DEPENDENCIES = $(PREFIX) $(AVRM)

include $(AVRM)/Makefile
