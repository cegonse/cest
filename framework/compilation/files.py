import os
import json

def all_files_in(directory):
    return os.listdir(directory)


def read_file(file):
    f = open(file, 'r')
    data = f.read()
    f.close()

    return data


def create_text_file(path, contents):
    file = open(path, "w")
    file.write(contents)
    file.close()


def delete_file(path):
    try:
        os.remove(path)
    except FileNotFoundError:
        pass


def parse_jsonl(text):
    result = []
    lines = text.splitlines()

    for line in lines:
        line_as_dict = json.loads(line)
        result.append(line_as_dict)

    return result
