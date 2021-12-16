#!/usr/bin/env bash

gcc run.c -o run
gcc run2.c -o run2
gcc -pthread -O3 fast.c -o fast