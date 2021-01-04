import argparse
import sys
import typing
import glob
import os
import re
from functools import reduce

class Header:
    def __init__(self, filename, include_directory):
        self.filename = filename
        self.include_directory = include_directory
        self.relpath = os.path.relpath(filename, include_directory).replace("\\", "/")
        self.mark = False

def main(argv) -> None:
    parser = argparse.ArgumentParser(description="Combine headers into a single file")
    parser.add_argument("output")
    parser.add_argument("include_directories", nargs="+")

    args = parser.parse_args(argv)

    # Find all header files
    headers = []
    for include_directory in args.include_directories:
        for filename in glob.glob(os.path.join(include_directory, "lingo", "**", "*.hpp"), recursive=True):
            headers.append(Header(filename, include_directory))

    # Write the content of all headers to a single header
    with open(args.output, "w") as output:
        for header in headers:
            write_header(header, output, headers)

def write_header(header, output, headers):
    if header.mark:
        return
    header.mark = True
    
    output.write("#line 1 \"{}\"\n".format(header.relpath))

    line_index = 1
    with open(header.filename, "r") as input:
        for line in input.readlines():
            match = re.match(r".*#include\s*[<\"](\S*)[>\"].*", line)
            if match:
                include = [h for h in headers if h.relpath == match.group(1)]
                if len(include) > 0:
                    write_header(include[0], output, headers)
                    output.write("#line {} \"{}\"\n".format(line_index + 1, header.relpath))
                else:
                    output.write(line)
            else:
                output.write(line)
            line_index += 1
    output.write("\n")

if __name__ == "__main__":
    main(sys.argv[1:])