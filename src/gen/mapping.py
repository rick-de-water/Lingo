import argparse
import os
import re
import sys
import typing


header_format = """#ifndef H_LINGO_PAGE_INTERNAL_{0}
#define H_LINGO_PAGE_INTERNAL_{0}

namespace lingo
{{
	namespace page
	{{
		namespace internal
		{{
            template <typename Point>
            struct {1}
            {{
                using point_type = Point;

                static constexpr point_type minor_table_empty[256] = {{ {2} }};
                {3}
                static constexpr point_type point_start_index = {4};
                static constexpr point_type point_end_index = {5};

				static constexpr point_type minor_table_start_index = {6};
				static constexpr point_type minor_table_end_index = {7};
				static constexpr point_type minor_table_count = {8};

                static constexpr const point_type* major_table[{8}] {{ {9} }};
            }};
        }}
    }}
}}

#endif"""

class Mapping():
    def __init__(self, source: int = -1, destination: int = -1):
        self.source = source
        self.destination = destination

    def __str__(self):
        return "0x{:X} -> 0x{:X}".format(self.source, self.destination)

def parse_mappings(input: typing.TextIO) -> typing.List[Mapping]:
    mappings = []
    for line in input:
        result = re.match(r"(0x[\da-fA-F]+)[\t;]+(0x[\da-fA-F]+)", line)
        if (result):
            mappings.append(Mapping(int(result.group(1), base=16), int(result.group(2), base=16)))
    return mappings

def invert_mappings(mappings: typing.List[Mapping]) -> typing.List[Mapping]:
    inverted_mappings = []
    for mapping in mappings:
        inverted_mappings.append(Mapping(mapping.destination, mapping.source))
    return inverted_mappings

def generate_header(mappings: typing.List[Mapping], output: typing.TextIO, name: str) -> None:
    minor_table_empty = [0xFFFF] * 256
    minor_tables = []
    
    point_start_index = 0xFFFFFFFF
    point_end_index = 0

    minor_table_start_index = 0xFFFFFFFF
    minor_table_end_index = 0

    for mapping in mappings:
        if mapping.source < point_start_index:
            point_start_index = mapping.source
        if mapping.source > point_end_index:
            point_end_index = mapping.source

        minor_table_index = mapping.source // 256
        if minor_table_index < minor_table_start_index:
            minor_table_start_index = minor_table_index
        if minor_table_index > minor_table_end_index:
            minor_table_end_index = minor_table_index

        while (len(minor_tables) <= minor_table_index):
            minor_tables.append(minor_table_empty)
        if minor_tables[minor_table_index] is minor_table_empty:
            minor_tables[minor_table_index] = minor_table_empty.copy()
        minor_tables[minor_table_index][mapping.source - minor_table_index * 256] = mapping.destination
        
    minor_table_string = ""
    major_table_list = []
    for i in range(0, len(minor_tables)):
        if minor_tables[i] is minor_table_empty:
            major_table_list.append("minor_table_empty")
        else:
            minor_table_string += "static constexpr point_type minor_table_{}[256] = {{ {} }};\n\t\t\t\t".format(i, ", ".join(("0x{:X}".format(x) for x in minor_tables[i])))
            major_table_list.append("minor_table_{}".format(i))

    output.write(header_format.format(
        name.upper(),
        name,
        ", ".join(("0x{:X}".format(x) for x in minor_table_empty)),
        minor_table_string,
        point_start_index,
        point_end_index,
        minor_table_start_index,
        minor_table_end_index,
        minor_table_end_index - minor_table_start_index + 1,
        ", ".join(major_table_list)
    ))

def main(argv) -> None:
    parser = argparse.ArgumentParser(description="Generate headers from unicode mapping files")
    parser.add_argument("input")
    parser.add_argument("output_directory")
    parser.add_argument("name")
    parser.add_argument("reverse_name")

    args = parser.parse_args(argv)

    with open(args.input, "r") as input:
        mappings = parse_mappings(input)
        inverted_mappings = invert_mappings(mappings)
    
    with open(os.path.join(args.output_directory, args.name) + ".hpp", "w") as output:
        generate_header(mappings, output, args.name)
    
    with open(os.path.join(args.output_directory, args.reverse_name) + ".hpp", "w") as output:
        generate_header(inverted_mappings, output, args.reverse_name)

if __name__ == "__main__":
    main(sys.argv[1:])