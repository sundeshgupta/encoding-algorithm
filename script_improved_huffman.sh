#!/bin/bash
for filename in ./images/*; do
    g++ improved_huffman_encode_image.cpp
    ./a.out <<< "$filename" >> data_improved_huffman.csv
    g++ improved_huffman_decode_image.cpp
    ./a.out >> data_improved_huffman.csv
done