import re
import sys

lines_out = []
lines_out.append(
    "#include \"runtime/tests/macros.h\"\n"
    "#include \"runtime/interpreter.h\"\n"
    "#include \"mem/mem.h\"\n"
    "#include <gtest/gtest.h>\n\n")

assert(len(sys.argv) == 3)

with open(sys.argv[1], 'r') as file:
    for line in file:
        if (line.startswith("B")):  # B type test
            continue
        elif (line.startswith("TEST_IMM_OP")):  # I type test
            regexp = re.compile(r'(?<=\().*(?=\))')
            matched = regexp.search(line)
            splited_line = matched.group().split(", ")
            splited_line[0] = splited_line[0].upper()
            splited_line[1] = splited_line[1][1:]
            splited_line[2] = splited_line[2][1:]

            line_mangled = "TEST_IMM_OP(" + ', '.join(splited_line) + ")\n"
            lines_out.append(line_mangled)
            continue
        elif (line.startswith("J")):  # J type test
            continue
        elif (line.startswith("TEST_RR_OP")):  # R type test
            regexp = re.compile(r'(?<=\().*(?=\))')
            matched = regexp.search(line)
            splited_line = matched.group().split(", ")
            splited_line[0] = splited_line[0].upper()
            splited_line[1] = splited_line[1][1:]
            splited_line[2] = splited_line[2][1:]
            splited_line[3] = splited_line[3][1:]

            line_mangled = "TEST_RR_OP(" + ', '.join(splited_line) + ")\n"
            lines_out.append(line_mangled)
        elif (line.startswith("S")):  # S type test
            continue
        elif (line.startswith("U")):  # U type test
            continue

with open(sys.argv[2], 'w') as file:
    file.writelines(lines_out)
