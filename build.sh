#!/bin/bash


SOURCES=(
    src/main.c
    src/imghash.c
    src/dev.c
)

gcc -g -Wall -DDEBUG -I "include" ${SOURCES[*]} -lm `pkg-config vips --cflags --libs`
