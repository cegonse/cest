def all_suites(files):
    cpp_files = filter(is_cpp_file, files)

    return [build_test_suite(path) for path in cpp_files]


def build_test_suite(path):
    return TestSuite(
        path=path,
        name=extract_suite_name(path)
    )


def extract_suite_name(path):
    return remove_extension(remove_subdirectories(path))


def remove_extension(path):
    filtered_path = path

    for extension in valid_extensions:
        filtered_path = filtered_path.replace(extension, '')
    
    return filtered_path


def remove_subdirectories(path):
    return path.split('/')[-1]


def is_cpp_file(path):
    return any([extension in path for extension in valid_extensions])


valid_extensions = ['.cpp', '.cxx', '.cc']


class TestSuite(object):
    def __init__(self, path, name):
        self.path = path
        self.name = name

    def __eq__(self, other):
        return self.__dict__ == other.__dict__

    def __repr__(self):
        return 'TestSuite (path={path}, name={name})'.format(
            path=self.path,
            name=self.name
        )
