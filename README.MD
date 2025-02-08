Cest C++ Testing Framework
==========================

[https://cestframework.com/](https://cestframework.com/)

A test-driven development framework for C++ and C inspired by Jest and similar frameworks.

Getting started
---------------

Cest requires a C++ compiler with C++14 support or greater. These should be available on most systems. It can be used either stand-alone or integrated with an external build system.

Download the latest cest release from GitHub: https://github.com/cegonse/cest/releases

Create a new test suite in the called, for example, `test_sum.cpp`:

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

Build test suite including the Cest framework header, and run the binary. Cest will run your new test:

```bash
g++ -I. --std=c++14 test_sum.cpp -o test_sum
./test_sum

PASS  spec/test_sum.cpp:11 it adds 1 + 2 to equal 3
```

**You have created your first test with Cest!**

Integrating Cest in an existing build system
--------------------------------------------

The Cest core is a single C++ header. To integrate it into your build system, simply add the `cest` header to your include path and include `cest` in your tests.

The framework header already includes a `main()` function, so tests must be linked *without* any additional entry point.

Cest runner will execute all tests inside the suite and return **0 on success** and **1 on failure**.

To see an integration example, check the [cpm package manager](https://github.com/jorsanpe/cpm).

License
-------

Cest is MIT licensed.

Contributing and roadmap
------------------------

Feel free to create any issues or pull requests with new functionality you deem useful! I will review issues created by others first.

Documentation is hosted in GitHub Pages in a different repository, found here: https://github.com/cegonse/cest-docs. Please create any issues related to the documentation in the main repository (cegonse/cest) instead of the docs one.

**Building**

Cest source code is in the `src/` directory, separated in multiple header files. Cest-runner code is in the `runner/` directory. The final build is a single header library plus `cest-runner`, which is built using [Quom](https://github.com/Viatorus/quom).

To generate the library amalgam, install Quom and build doing:
```bash
pip install quom
make
```

This will generate the library and runner builds in the `build/` directory.

**Roadmap**

- [x] Basic test support (it, xit, fit)
- [x] macOS and Linux support
- [x] Test parametrization support
- [x] Basic expect matchers
- [x] Nested test suite support
- [x] Simple integration of custom expect matchers
- [x] `cest-runner` for watch mode functionality of multiple suites
- [x] Test cases run in an subprocess to avoid crashing the whole suite
- [x] Leak sanitizer integration (memory leak detector)
- [ ] Complex expect matchers (comprehensive STD collection support)
- [ ] Windows support
