#include <cest>

using namespace std;


struct CoinAmount {
    int two_euro;
    int one_euro;
};

enum ParseResult {
    OK,
    NEGATIVE_AMOUNT,
    INVALID_CURRENCY
};

struct Amount {
    int integer_part;
    ParseResult parse_result;
};


namespace Coiner
{
    bool any(int amount)
    {
        return amount > 0;
    }

    bool not_any(int amount)
    {
        return !any(amount);
    }

    string renderCoins(int amount)
    {
        if (amount > 1) {
            return "coins";
        }

        return "coin";
    }

    string present(int two_euro_coin_amount=0, int one_euro_coin_amount=0)
    {
        stringstream stream;

        if (not_any(two_euro_coin_amount) && not_any(one_euro_coin_amount)) return "no coins";

        if (two_euro_coin_amount > 0) {
            stream << two_euro_coin_amount << " " << renderCoins(two_euro_coin_amount) << " of 2E";
        }

        if (two_euro_coin_amount > 0 && one_euro_coin_amount > 0) {
            stream << " and ";
        }

        if (one_euro_coin_amount > 0) {
            stream << one_euro_coin_amount << " coin of 1E";
        }

        return stream.str();
    }

    string presentFailedParseResult(ParseResult result)
    {
        if (result == ParseResult::NEGATIVE_AMOUNT) {
            return "negative amounts cannot be counted";
        }

        return "only amounts in Euro be counted";
    }

    int calculate_two_euro_coin_amount(int euro_amount)
    {
        return euro_amount / 2;
    }

    int calculate_one_euro_coin_amount(int euro_amount, int remainder)
    {
        return euro_amount - remainder;
    }

    CoinAmount calculate(int euro_amount)
    {
        CoinAmount amount;

        amount.two_euro = calculate_two_euro_coin_amount(euro_amount);
        amount.one_euro = calculate_one_euro_coin_amount(euro_amount, amount.two_euro * 2);

        return amount;
    }

    bool isValidCurrency(string money)
    {
        return money.find('E') != string::npos;
    }

    string extractIntegerAmount(string money)
    {
        return money.substr(0, money.find('E'));
    }

    Amount parse(string money)
    {
        Amount amount;
        string integer_part_descriptor;

        if (!isValidCurrency(money)) {
            amount.parse_result = ParseResult::INVALID_CURRENCY;
            return amount;
        }

        integer_part_descriptor = extractIntegerAmount(money);
        amount.integer_part = stoi(integer_part_descriptor);

        if (amount.integer_part < 0) {
            amount.parse_result = ParseResult::NEGATIVE_AMOUNT;
            return amount;
        }

        amount.parse_result = ParseResult::OK;

        return amount;
    }

    string count(string money)
    {
        Amount total_amount;
        CoinAmount coin_amount;

        total_amount = parse(money);

        if (total_amount.parse_result != ParseResult::OK) {
            return presentFailedParseResult(total_amount.parse_result);
        }

        coin_amount = calculate(total_amount.integer_part);

        return present(coin_amount.two_euro, coin_amount.one_euro);
    }
}


describe("currency kata", []() {
    it("converts 0E to coins", []() {
        string money = "0E";

        string coins = Coiner::count(money);

        expect(coins).toBe("no coins");
    });

    it("converts 1E to coins", []() {
        string money = "1E";

        string coins = Coiner::count(money);

        expect(coins).toBe("1 coin of 1E");
    });

    it("converts 2E to coins", []() {
        string money = "2E";

        string coins = Coiner::count(money);

        expect(coins).toBe("1 coin of 2E");
    });

    it("converts 3E to coins", []() {
        string money = "3E";

        string coins = Coiner::count(money);

        expect(coins).toBe("1 coin of 2E and 1 coin of 1E");
    });

    it("converts 4E to coins", []() {
        string money = "4E";

        string coins = Coiner::count(money);

        expect(coins).toBe("2 coins of 2E");
    });

    it("converts 213E to coins", []() {
        string money = "213E";

        string coins = Coiner::count(money);

        expect(coins).toBe("106 coins of 2E and 1 coin of 1E");
    });

    it("rejects negative amounts", []() {
        string money = "-4E";

        string coins = Coiner::count(money);

        expect(coins).toBe("negative amounts cannot be counted");
    });

    it("rejects amounts not in Euro", []() {
        string money = "4$";

        string coins = Coiner::count(money);

        expect(coins).toBe("only amounts in Euro be counted");
    });
});
