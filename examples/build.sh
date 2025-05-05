#!/bin/sh

set -xe

cc -Wall -Wextra -ggdb -fsanitize=address -I ../src/ -o fib fib.c
cc -Wall -Wextra -ggdb -fsanitize=address -I ../src/ -o factorial factorial.c
cc -Wall -Wextra -ggdb -fsanitize=address -I ../src/ -o PandC PandC.c
