#!/bin/sh

set -xe

cc -Wall -Wextra -ggdb -I ./src -o fistr main.c
