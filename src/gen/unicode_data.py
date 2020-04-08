import argparse
import sys
import typing
from collections import namedtuple

header_format = """#ifndef H_LINGO_PAGE_INTERNAL_UNICODE_DATA_{version}
#define H_LINGO_PAGE_INTERNAL_UNICODE_DATA_{version}

namespace lingo
{{
	namespace page
	{{
        namespace unicode_{version}
        {{
            namespace internal
            {{
                template <typename Point>
                struct unicode_data_{version}
                {{
                    using point_type = Point;

                }};
            }}
        }}
    }}
}}

#endif"""

Entry = namedtuple("Entry", [
    "code_value",
    "character_name",
    "general_catagory",
    "canonical_combining_classes",
    "bidirectional_category",
    "character_decomposition_mapping",
    "decimal_digit_value",
    "digit_value",
    "numeric_value",
    "mirrored",
    "unicode_1_0_name",
    "iso10646_comment_field",
    "uppercase_mapping",
    "lowercase_mapping",
    "titlecase_mapping"])

def parse_unicode_data(input: typing.TextIO) -> typing.List[Entry]:
    entries = []
    for line in input:
        if len(line) == 0 or line[0] == '#' or line == "\n":
            continue
        fields = line.split(";")
        if len(fields) != 15:
            raise SyntaxError("Invalid field count: {}, expected: 15.\nSource: {}".format(len(fields), repr(line)))

        fields = [field if len(field) > 0 else None for field in fields]
        
        entry = Entry._make(fields)
        #entry.code_value = int(fields[0], base=16)
        #entry.character_name = fields[1]
        #entry.general_catagory = fields[2]
        #entry.canonical_combining_classes = fields[3]
        entries.append(entry)

    return entries

def generate_header(entries: typing.List[Entry], version: str, output: typing.TextIO) -> None:
    safe_version = version.replace(".", "_")
    output.write(header_format.format(version=safe_version))

def main(argv) -> None:
    parser = argparse.ArgumentParser(description="Generate headers from unicode data files")
    parser.add_argument("input")
    parser.add_argument("output")
    parser.add_argument("version")

    args = parser.parse_args(argv)
    with open(args.input, "r") as input:
        entries = parse_unicode_data(input)

    with open(args.output, "w") as output:
        generate_header(entries, args.version, output)
    
if __name__ == "__main__":
    main(sys.argv[1:])