Cest C++ Testing Framework
==========================

![CI](https://github.com/cegonse/cest/workflows/CI/badge.svg?branch=master)

[https://cestframework.com/](https://cestframework.com/)

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

Build the project and Cest will run your new test:

```bash
make

Building tests and sources

 PASS  spec/test_sum.cpp:11 it adds 1 + 2 to equal 3
```

**You have created your first test with Cest!**

Integrating Cest in an existing build system
--------------------------------------------

The Cest core is a single C++ header. To integrate it into your build system, simply add the `vendor/` directory to your include path and include `cest` in your tests.

The framework header already includes a `main()` function, so tests must be linked *without* any additional entry point.

Cest runner will execute all tests inside the suite and return **0 on success** and **1 on failure**.

To see an integration example, check the [cpm package manager](https://github.com/jorsanpe/cpm).

Contributing
------------

Feel free to create any issues or pull requests with new functionality you deem useful! I will review issues created by others first.

License
-------

Cest is MIT licensed.
