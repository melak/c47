#! /bin/bash 

find . -name '*.c' -o -name '*.h' | xargs perl -p -i subs1.txt
