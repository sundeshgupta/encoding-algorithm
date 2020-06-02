#!/bin/bash
for filename in ./images/*; do
    g++ huffman_encode_image.cpp
    ./a.out <<< "$filename" >> data_huffman.csv
    g++ huffman_decode_image.cpp
    ./a.out >> data_huffman.csv
done