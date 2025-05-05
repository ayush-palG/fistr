#!/bin/sh

set -xe

cc -Wall -Wextra -ggdb -fsanitize=address -o factorial factorial.c
cc -Wall -Wextra -ggdb -fsanitize=address -o nCk nCk.c
cc -Wall -Wextra -ggdb -fsanitize=address -I ../src/ -o fib fib.c
