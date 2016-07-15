SHELL = /bin/sh

TARGET = xdgurl
srcdir = ./src

DESTDIR =
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datadir = $(prefix)/share

INSTALL = install
INSTALL_PROGRAM = $(INSTALL) -D -m 755
INSTALL_DATA = $(INSTALL) -D -m 644
RM = rm -f

.PHONY: all rebuild build clean install uninstall

all: rebuild ;

rebuild: clean build ;

build: $(TARGET) ;

clean:
	$(RM) ./$(TARGET)

install: build
	$(INSTALL_PROGRAM) ./$(TARGET) $(DESTDIR)$(bindir)/$(TARGET)
	$(INSTALL_DATA) $(srcdir)/$(TARGET).desktop $(DESTDIR)$(datadir)/applications/$(TARGET).desktop

uninstall:
	$(RM) $(DESTDIR)$(bindir)/$(TARGET)
	$(RM) $(DESTDIR)$(datadir)/applications/$(TARGET).desktop

$(TARGET):
	# Just copy for now
	install -m 755 $(srcdir)/$(TARGET).py ./$(TARGET)
