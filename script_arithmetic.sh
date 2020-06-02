#!/bin/bash
for filename in ./images/*; do
    g++ arithmetic_encode_image.cpp
    ./a.out <<< "$filename" >> data_arithmetic.csv
    g++ arithmetic_decode_image.cpp
    ./a.out >> data_arithmetic.csv
done