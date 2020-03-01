Cest
====

![CI](https://github.com/cegonse/cest/workflows/CI/badge.svg?branch=master)

A test-driven development framework for C++ inspired by Jest and similar frameworks.

Getting started
---------------

Cest requires having `make`, `g++` and `python` installed. These should be available on most systems. It can be used either stand-alone or integrated with an external build system.

Clone or download the Cest Git repository:

```bash
git clone git@github.com:cegonse/cest.git
```

Create a new test suite in the `spec/` directory called, for example, `test_sum.cpp`:

```cpp
#include <cest>

int sum(int a, int b) {
    return a + b;
}

describe("testing additions", []() {
    it("adds 1 + 2 to equal 3", []() {
        expect(sum(1, 2)).toBe(3);
    });
});
```

Run your new test and Cest will print the results:

```bash
make test_sum

Building tests and sources
 PASS  spec/test_sum.cpp:11 it adds 1 + 2 to equal 3
```

**You have created your first test with Cest!**

Integrating Cest in an existing build system
--------------------------------------------

The Cest core is a single C++ header. To integrate it into your build system, simply add the `framework/` directory to your include path and include `cest` in your tests.

The framework header already includes a `main()` function, so tests must be linked *without* any additional entry point.

Cest runner will execute all tests inside the suite and return **0 on success** and **1 on failure**. Besides the console output showing the results, a [JSONL](http://jsonlines.org/) file is generated containing the test suite results in JSON format, where each execution of a test suite will append its results to the file.

There is a [JUnit](https://github.com/junit-team/junit5/blob/master/platform-tests/src/test/resources/jenkins-junit.xsd) converter included in the `framework/` folder (`junit.py`) which will convert any `test_summary.jsonl` file present in the working directory and generate a JUnit XML file called `log.xml`, which can be parsed by Jenkins or GitHub.

To see an integration example, check the [cpm package manager](https://github.com/jorsanpe/cpm).

Contributing
------------

Feel free to create any issues or pull requests with new functionality you deem useful! I will review issues created by others first.

License
-------

Cest is MIT licensed.
