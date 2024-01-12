.PHONY: all install uninstall clean

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
MANDIR ?= $(PREFIX)/share/man/man1
INSTALL ?= install

SOURCES = mcp.c

all: mcp

mcp: $(SOURCES)
	$(CC) -o $@ $<

install: mcp
	$(INSTALL) -v -C -m 755 mcp $(BINDIR)/mcp
	$(INSTALL) -v -C -m 644 mcp.1 $(MANDIR)/mcp.1

uninstall:
	rm -f $(BINDIR)/mcp
	rm -f $(MANDIR)/mcp.1

clean:
	rm -f mcp

test: mcp testfile
	rm -f testfile.out
	./mcp testfile testfile.out
	du -k testfile testfile.out
	cksum testfile testfile.out
