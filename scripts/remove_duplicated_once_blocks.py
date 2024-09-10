import sys

with open(sys.argv[1], 'r') as file:
    lines = file.readlines()

occurences = []
prev = -1
for i in range(len(lines)):
    if '// CEST-ONCE-START' in lines[i]:
        prev = i
    
    if '// CEST-ONCE-END' in lines[i]:
        occurences.append((prev, i))


deleted_items = 0
if len(occurences) > 1:
    for i in range(1, len(occurences)):
        first = occurences[i][0]
        last = occurences[i][1]

        for j in range(first, last + 1):
            line_idx = j - deleted_items
            del lines[line_idx]
            deleted_items = deleted_items + 1

print("Deleting duplicated blocks:")
print(occurences)

source = ''.join(lines)

with open(sys.argv[1], 'w') as file:
    file.write(source)
