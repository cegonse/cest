import sys

with open(sys.argv[1], 'r') as file:
    lines = file.readlines()

result = []
for line in lines:
    if 'sanitizer/lsan_interface.h' in line:
        result.append('#ifndef _WIN32\n')
        result.append(line)
        result.append('#endif\n')
    else:
        result.append(line)

with open(sys.argv[1], 'w') as file:
    file.writelines(result)
