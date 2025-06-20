#!/usr/bin/env python
import sys

if __name__ == "__main__":
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    with open(output_file, 'wb') as outfile:
        outfile.write(b"// Include this file to import the usage info as a string.\n\n")

        outfile.write(b"#ifndef  GENERATED_SOURCE_USAGE_HPP_\n")
        outfile.write(b"#define  GENERATED_SOURCE_USAGE_HPP_\n\n")

        outfile.write(b"static const char usage_str[] = \"\\\n")
        for line in open(input_file, 'rb'):
            outfile.write(line.rstrip())
            outfile.write(b"\\n\\\n")
        outfile.write(b"\";\n\n")
        outfile.write(b"#endif  // GENERATED_SOURCE_USAGE_HPP_\n")
