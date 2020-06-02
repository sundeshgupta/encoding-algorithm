#!/bin/bash
for i in {10..1000..10}; do
    python3 checker.py <<< "$i"
    g++ huffman_encode_image.cpp
    ./a.out <<< "chk.jpg" >> data_chk.csv
    g++ arithmetic_encode_image.cpp
    ./a.out <<< "chk.jpg" >> data_chk.csv
done