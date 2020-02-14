import os

def all_files_in(directory):
    return os.listdir(directory)


def read_file(file):
    f = open(file, 'r')
    data = f.read()
    f.close()

    return data
