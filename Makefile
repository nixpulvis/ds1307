PREFIX = /usr/local/Cellar
LIBRARY = ds1307
VERSION = 0.0.3
TARGET = $(PREFIX)/$(LIBRARY)/$(VERSION)
DEPENDENCIES = $(TARGET) /usr/local/Cellar/avrm/0.0.3
include /usr/local/Cellar/avrm/0.0.3/Makefile
