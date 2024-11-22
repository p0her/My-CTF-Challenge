#!/bin/bash
make clean
make
strip -s main
cp main ../solver/main
cp main ../deploy/main
