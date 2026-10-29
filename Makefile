SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=build/%.o)

sttp: $(OBJS)
	gcc $^ -o $@

build/%.o: src/%.c
	@mkdir -p build
	gcc -Wall -Wextra -g -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

.PHONY: clean
	
clean:
	rm -rf build sttp
