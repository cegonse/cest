#include <cest>
#include <algorithm>
#include <vector>

using namespace std;


static bool isUppercase(string name)
{
    return all_of(name.begin(), name.end(), [](auto letter) {
        return isupper(letter);
    });
}


string Greet(string name)
{
    if (name == "") {
        return "Hello my friend!";
    }

    if (isUppercase(name)) {
        return "HELLO " + name + "!!!!";
    }

    return "Hello " + name + "!";
}


string greetManyPeople(vector<string> names)
{
    size_t i;
    string greeting = "Hello ";

    for (i=0; i<names.size(); ++i) {
        if (i == names.size() - 1) {
            greeting += names[i] + "!";
        } else if (i == names.size() - 2) {
            greeting += names[i] + " and ";
        } else {
            greeting += names[i] + ", ";
        }
    }

    return greeting;
}


string Greet(vector<string> names)
{
    if (names.size() == 0) {
        return Greet("");
    }

    if (names.size() == 1) {
        return Greet(names[0]);
    }

    vector<string> uppercase_names(names.size()), lowercase_names(names.size());

    auto it = copy_if(names.begin(), names.end(), uppercase_names.begin(), [](auto name) {
        return isUppercase(name);
    });

    uppercase_names.resize(distance(uppercase_names.begin(), it));

    it = copy_if(names.begin(), names.end(), lowercase_names.begin(), [](auto name) {
        return !isUppercase(name);
    });

    lowercase_names.resize(distance(lowercase_names.begin(), it));

    return greetManyPeople(lowercase_names);
}


describe("greeting kata", []() {
    it("should greet my friend when name is empty", []() {
        auto name = "";

        auto greeting = Greet(name);

        expect(greeting).toBe("Hello my friend!");
    });

    it("should greet", []() {
        auto name = "Jim";

        auto greeting = Greet(name);

        expect(greeting).toBe("Hello Jim!");
    });

    it("should greet shouting when name is uppercase", []() {
        auto name = "PEPE";

        auto greeting = Greet(name);

        expect(greeting).toBe("HELLO PEPE!!!!");
    });

    it("should greet my friend when greeting an empty list of names", []() {
        vector<string> names;

        auto greeting = Greet(names);

        expect(greeting).toBe("Hello my friend!");
    });

    it("should greet a list of names with one name", []() {
        vector<string> names({ "Juan" });

        auto greeting = Greet(names);

        expect(greeting).toBe("Hello Juan!");
    });

    it("should greet two people", []() {
        vector<string> names({ "Juan", "Maria" });

        auto greeting = Greet(names);

        expect(greeting).toBe("Hello Juan and Maria!");
    });

    it("should greet many people", []() {
        vector<string> names({ "Juan", "Antonio", "Maria" });

        auto greeting = Greet(names);

        expect(greeting).toBe("Hello Juan, Antonio and Maria!");
    });
});
