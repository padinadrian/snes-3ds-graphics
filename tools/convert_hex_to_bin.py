#! /usr/bin/env python

import sys

if __name__ == "__main__":
    input_file = sys.argv[1]
    with open(input_file, 'rb') as f:
        hex_data = f.readlines()

    bin_data = []
    for line in hex_data:
        for i in range(0, len(line) // 2):
            index = i * 2
            bin_data.append(int(line[index:index+2], 16))

    filename_parts = input_file.split('.')
    filename_parts[-1] = "bin"
    output_file = '.'.join(filename_parts)

    with open(output_file, 'wb') as f:
        f.write(bytearray(bin_data))
