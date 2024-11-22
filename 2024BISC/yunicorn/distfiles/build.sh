#!/bin/bash

gcc -o main main.c -lunicorn -O0 -fno-toplevel-reorder
strip main
