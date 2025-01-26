#!/bin/sh

set -xe
clang -Wall -Wextra -pedantic *.c -o ucc
rm -rf out/*.dSYM