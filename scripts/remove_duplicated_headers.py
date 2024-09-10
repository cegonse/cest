import sys

with open(sys.argv[1], 'r') as file:
    lines = file.readlines()

includes = [l for l in lines if '#include' in l]
lines_without_includes = [l for l in lines if '#include' not in l]
unique_includes = list(set(includes))

source = '#pragma once\n' + ''.join(unique_includes + lines_without_includes)

with open(sys.argv[1], 'w') as file:
    file.write(source)
