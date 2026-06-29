#include <cest>
#include <filesystem>

namespace fs = std::filesystem;

#ifdef _WIN32
static const fs::path ABSOLUTE_PATH = "C:\\Users\\user";
static const fs::path ABSOLUTE_PATH_WITH_FILE = "C:\\Users\\user\\file.txt";
static const fs::path ABSOLUTE_PATH_NO_EXT = "C:\\Users\\user\\gcc";
#else
static const fs::path ABSOLUTE_PATH = "/home/user";
static const fs::path ABSOLUTE_PATH_WITH_FILE = "/home/user/file.txt";
static const fs::path ABSOLUTE_PATH_NO_EXT = "/usr/bin/gcc";
#endif

describe("std::filesystem::path assertions", []() {
    it("asserts path equality", []() {
        fs::path p = ABSOLUTE_PATH_WITH_FILE;

        expect(p).toBe(ABSOLUTE_PATH_WITH_FILE);
        expect(p).toEqual(ABSOLUTE_PATH_WITH_FILE);

        expect(p).Not->toBe(fs::path("/other/path"));
    });

    it("asserts path extension", []() {
        fs::path p = ABSOLUTE_PATH_WITH_FILE;

        expect(p).toHaveExtension(".txt");

        expect(p).Not->toHaveExtension(".cpp");
        expect(p).Not->toHaveExtension("");
    });

    it("asserts path filename", []() {
        fs::path p = ABSOLUTE_PATH_WITH_FILE;

        expect(p).toHaveFilename("file.txt");

        expect(p).Not->toHaveFilename("other.txt");
    });

    it("asserts absolute and relative paths", []() {
        fs::path relative = "src/main.cpp";

        expect(ABSOLUTE_PATH).toBeAbsolute();
        expect(relative).toBeRelative();

        expect(ABSOLUTE_PATH).Not->toBeRelative();
        expect(relative).Not->toBeAbsolute();
    });

    it("works with paths without extension", []() {
        expect(ABSOLUTE_PATH_NO_EXT).toHaveExtension("");
        expect(ABSOLUTE_PATH_NO_EXT).toHaveFilename("gcc");
        expect(ABSOLUTE_PATH_NO_EXT).toBeAbsolute();
    });
});
