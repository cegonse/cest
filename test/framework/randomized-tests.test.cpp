#include <cest>
#include <algorithm>

describe("randomized tests", []() {
  it("randomizes tests in root and child suites, with provided sorting algorithm", []() {
    int seed = 0;
    int received_seed;
    auto fn = [&](int s, cest::TestSuite *suite)
    {
      received_seed = s;
      std::reverse(suite->test_cases.begin(), suite->test_cases.end());
    };
    cest::TestSuite root_suite, child_suite;
    cest::TestCase a, b, c, d;
    a.name = "a";
    b.name = "b";
    c.name = "c";
    d.name = "d";
    root_suite.test_cases.push_back(&a);
    root_suite.test_cases.push_back(&b);
    child_suite.test_cases.push_back(&c);
    child_suite.test_cases.push_back(&d);
    root_suite.test_suites["child"] = &child_suite;

    cest::randomizeTests(&root_suite, seed, fn);

    expect(received_seed).toEqual(seed);
    expect(root_suite.test_cases[0]->name).toEqual("b");
    expect(root_suite.test_cases[1]->name).toEqual("a");
    expect(child_suite.test_cases[0]->name).toEqual("d");
    expect(child_suite.test_cases[1]->name).toEqual("c");
  });
});
