#include <cest>

using namespace std;
using namespace cest;

describe("parametrized tests", []() {
    it("can make use of integer parameters", []() {
        withParameter<int>().
            withValue(10).
            withValue(20).
            withValue(30).
            thenDo([](int x) {
                expect(x > 0).toBeTruthy();
            });
    });

    it("can make use of string parameters", []() {
        withParameter<std::string>().
            withValue("hello cat").
            withValue("hello dog").
            withValue("hello cest").
            thenDo([](std::string x) {
                expect(x).toContain("hello");
            });
    });

    it("can be used with classes", []() {
        typedef std::pair<int, std::string> HttpStatusCode;

        auto statusCodeCategory = [](HttpStatusCode status_code) {
            if (status_code.first >= 400 && status_code.first <= 499) {
                return std::string("Client Error");
            }

            return std::string("Unknown");
        };

        withParameter<HttpStatusCode>().
            withValue(HttpStatusCode(400, "Bad Request")).
            withValue(HttpStatusCode(403, "Forbidden")).
            withValue(HttpStatusCode(404, "Not Found")).
            withValue(HttpStatusCode(409, "Conflict")).
            thenDo([=](HttpStatusCode x) {
                expect(statusCodeCategory(x)).toBe("Client Error");
            });
    });
});
