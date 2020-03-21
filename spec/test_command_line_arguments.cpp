#include <cest>


describe("Cest command line options", []() {
    it("will use default behaviour if empty", []() {
        int argc = 1;
        const char *argv[] = { "/bin/cest" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.quiet).toBe(false);
    });

    it("will supress output if --quiet is present", []() {
        int argc = 2;
        const char *argv[] = { "/bin/cest", "--quiet" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.quiet).toBe(true);
    });

    it("will supress output if -q is present", []() {
        int argc = 2;
        const char *argv[] = { "/bin/cest", "-q" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.quiet).toBe(true);
    });

    it("will show help if --help is present", []() {
        int argc = 2;
        const char *argv[] = { "/bin/cest", "--help" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.help).toBe(true);
    });

    it("will show help if -h is present", []() {
        int argc = 2;
        const char *argv[] = { "/bin/cest", "-h" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.help).toBe(true);
    });
});
