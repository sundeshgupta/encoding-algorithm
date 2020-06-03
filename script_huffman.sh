#!/bin/bash
for filename in ./images/*; do
    g++ huffman_encode_image.cpp
    ./a.out rgb <<< "$filename" >> data_huffman_rgb.csv
    g++ huffman_decode_image.cpp
    ./a.out >> data_huffman_rgb.csv
done