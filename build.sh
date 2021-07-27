#!/bin/bash


SOURCES=(
    src/main.c
    src/imghash.c
    src/dev.c
)

gcc -g -Wall -I "include" ${SOURCES[*]} -lm `pkg-config vips --cflags --libs`
