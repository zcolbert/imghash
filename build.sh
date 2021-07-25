#!/bin/bash


SOURCES=(
    main.c
    imghash.c
    dev.c
)

gcc -g -Wall ${SOURCES[*]} -lm `pkg-config vips --cflags --libs`
