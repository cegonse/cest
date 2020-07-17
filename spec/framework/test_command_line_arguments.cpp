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

    it("will randomize test execution if -r is present", []() {
        int argc = 2;
        const char *argv[] = { "/bin/cest", "-r" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.randomize).toBe(true);
    });

    it("will randomize test execution if --random is present", []() {
        int argc = 2;
        const char *argv[] = { "/bin/cest", "--randomize" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.randomize).toBe(true);
    });

    it("will use provided seed for randomized tests if -s is present", []() {
        int argc = 3;
        const char *argv[] = { "/bin/cest", "-s", "112233" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.random_seed_present).toBe(true);
        expect(options.random_seed).toBe(112233);
    });

    it("will use provided seed for randomized tests if --seed is present", []() {
        int argc = 3;
        const char *argv[] = { "/bin/cest", "--seed", "112233" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.random_seed_present).toBe(true);
        expect(options.random_seed).toBe(112233);
    });

    it("will fail to use provided seed for randomized tests if seed is not present", []() {
        int argc = 2;
        const char *argv[] = { "/bin/cest", "-s" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.random_seed_present).toBe(false);
    });

    it("will fail to use provided seed for randomized tests if seed is not an integer", []() {
        int argc = 3;
        const char *argv[] = { "/bin/cest", "-s", "blue" };
        cest::CommandLineOptions options;

        options = cest::parseArgs(argc, argv);

        expect(options.random_seed_present).toBe(false);
    });
});
