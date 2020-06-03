#!/bin/bash
for filename in ./images/*; do
    g++ improved_huffman_encode_image.cpp
    ./a.out rgb <<< "$filename" >> data_improved_huffman_rgb.csv
    g++ improved_huffman_decode_image.cpp
    ./a.out >> data_improved_huffman_rgb.csv
done
