import argparse
import glob
import io
import os
import re
import sys
import typing
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
    parser.add_argument("-m", "--module", action="store_true")

    args = parser.parse_args(argv)
    generate_module = args.module

    # Find all header files
    headers = []
    for include_directory in args.include_directories:
        for filename in glob.glob(os.path.join(include_directory, "lingo", "**", "*.hpp"), recursive=True):
            if filename.endswith("lingo.hpp"):
                continue
            headers.append(Header(filename, include_directory))

    print([h.relpath for h in headers])

    # Write the content of all headers to a single header
    #with open(args.output, "w") as output:
    with io.StringIO() as output:
        system_includes = []
        
        for header in headers:
            write_header(header, output, headers, system_includes)

        with open(args.output, "w") as file:

            system_includes = list(set(system_includes))
            system_includes.sort()

            if generate_module:
                file.write("module;\n")
            file.writelines(system_includes)
            if generate_module:
                file.write("export module lingo;\nexport{\n")

            output.seek(0)
            file.write(output.read())
            
            if generate_module:
                file.write("}\n")

def write_header(header, output, headers, system_includes):
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
                    print(include[0].relpath)
                    write_header(include[0], output, headers, system_includes)
                    output.write("#line {} \"{}\"\n".format(line_index + 1, header.relpath))
                else:
                    system_includes.append(line)
            else:
                output.write(line)
            line_index += 1
    output.write("\n")

if __name__ == "__main__":
    main(sys.argv[1:])