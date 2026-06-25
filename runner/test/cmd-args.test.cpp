#include <cest>
#include "../directory.h"
#include "../cmd-args.h"

describe("CmdArgs", []() {
  describe("path()", []() {
    it("defaults to CWD when path is not present in args", []() {
      std::string cmd_path = "/bin/test";
      std::array<char *, 1> argv = { (char *)cmd_path.c_str() };

      auto cmd_args = CmdArgs(argv.size(), argv.data());

      expect(cmd_args.path()).toEqual(Directory::cwd());
    });

    it("assumes non-first argument not starting with -- to be the path", []() {
      std::string cmd_path = "/bin/test";
      std::string watch_mode = "--watch";
      std::string path = "/home/tests";
      std::array<char *, 3> argv = {
        (char *)cmd_path.c_str(),
        (char *)watch_mode.c_str(),
        (char *)path.c_str()
      };

      auto cmd_args = CmdArgs(argv.size(), argv.data());

      expect(cmd_args.path()).toEqual("/home/tests");
    });
  });

  describe("watch()", []() {
    it("defaults to false", []() {
      std::string cmd_path = "/bin/test";
      std::array<char *, 1> argv = { (char *)cmd_path.c_str() };

      auto cmd_args = CmdArgs(argv.size(), argv.data());

      expect(cmd_args.watch()).toBeFalsy();
    });

    it("sets to true when --watch is passed", []() {
      std::string cmd_path = "/bin/test";
      std::string watch = "--watch";
      std::array<char *, 2> argv = { (char *)cmd_path.c_str(), (char *)watch.c_str() };

      auto cmd_args = CmdArgs(argv.size(), argv.data());

      expect(cmd_args.watch()).toBeTruthy();
    });
  });

  describe("help()", []() {
    it("defaults to false", []() {
      std::string cmd_path = "/bin/test";
      std::array<char *, 1> argv = { (char *)cmd_path.c_str() };

      auto cmd_args = CmdArgs(argv.size(), argv.data());

      expect(cmd_args.help()).toBeFalsy();
    });

    it("sets to true when --help is passed", []() {
      std::string cmd_path = "/bin/test";
      std::string help = "--help";
      std::array<char *, 2> argv = { (char *)cmd_path.c_str(), (char *)help.c_str() };

      auto cmd_args = CmdArgs(argv.size(), argv.data());

      expect(cmd_args.help()).toBeTruthy();
    });
  });

  describe("grep()", []() {
    it("defaults to empty string", []() {
      std::string cmd_path = "/bin/test";
      std::array<char *, 1> argv = { (char *)cmd_path.c_str() };

      auto cmd_args = CmdArgs(argv.size(), argv.data());

      expect(cmd_args.grep()).toBe("");
    });

    it("captures value after --grep", []() {
      std::string cmd_path = "/bin/test";
      std::string flag = "--grep";
      std::string pattern = "assertions";
      std::array<char *, 3> argv = { (char *)cmd_path.c_str(), (char *)flag.c_str(), (char *)pattern.c_str() };

      auto cmd_args = CmdArgs(argv.size(), argv.data());

      expect(cmd_args.grep()).toBe("assertions");
    });

    it("does not treat grep value as path", []() {
      std::string cmd_path = "/bin/test";
      std::string flag = "--grep";
      std::string pattern = "foo";
      std::array<char *, 3> argv = { (char *)cmd_path.c_str(), (char *)flag.c_str(), (char *)pattern.c_str() };

      auto cmd_args = CmdArgs(argv.size(), argv.data());

      expect(cmd_args.grep()).toBe("foo");
      expect(cmd_args.path()).toEqual(Directory::cwd());
    });
  });
});
