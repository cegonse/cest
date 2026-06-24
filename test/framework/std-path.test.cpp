#include <cest>
#include <filesystem>

namespace fs = std::filesystem;

describe("std::filesystem::path assertions", []() {
    it("asserts path equality", []() {
        fs::path p = "/home/user/file.txt";

        expect(p).toBe(fs::path("/home/user/file.txt"));
        expect(p).toEqual(fs::path("/home/user/file.txt"));

        expect(p).Not->toBe(fs::path("/other/path"));
    });

    it("asserts path extension", []() {
        fs::path p = "/home/user/file.txt";

        expect(p).toHaveExtension(".txt");

        expect(p).Not->toHaveExtension(".cpp");
        expect(p).Not->toHaveExtension("");
    });

    it("asserts path filename", []() {
        fs::path p = "/home/user/file.txt";

        expect(p).toHaveFilename("file.txt");

        expect(p).Not->toHaveFilename("other.txt");
    });

    it("asserts absolute and relative paths", []() {
        fs::path absolute = "/home/user";
        fs::path relative = "src/main.cpp";

        expect(absolute).toBeAbsolute();
        expect(relative).toBeRelative();

        expect(absolute).Not->toBeRelative();
        expect(relative).Not->toBeAbsolute();
    });

    it("works with paths without extension", []() {
        fs::path p = "/usr/bin/gcc";

        expect(p).toHaveExtension("");
        expect(p).toHaveFilename("gcc");
        expect(p).toBeAbsolute();
    });
});
