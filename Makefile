SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=build/%.o)

sttp: $(OBJS)
	gcc $^ -o $@

build/%.o: src/%.c
	@mkdir -p build
	gcc -Wall -Wextra -g -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

PREFIX ?= ~/.local
BINDIR := $(PREFIX)/bin

.PHONY: clean install uninstall
	
clean:
	rm -rf build sttp

install: sttp
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 sttp $(DESTDIR)$(BINDIR)/sttp

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/sttp
