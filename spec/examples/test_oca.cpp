#include <cest>
#include <map>

using namespace cest;

class Player {
    public:
        Player(std::string playerName) : name(playerName) {}

        std::string getName() { return name; }
    
    private:
        std::string name;
};

class DuplicatedPlayerException : std::exception {};

class OcaGame {
    public:
        OcaGame() {}

        void add(Player player) {
            for (auto p : players) {
                if (p.getName() == player.getName()) {
                    throw DuplicatedPlayerException();
                }
            }

            players.push_back(player);
        }

        int playerCount() { return players.size(); }

        std::vector<Player> getPlayers() { return players; }

        void move(Player player, int firstDice, int secondDice) {
        }

        int position(Player player) { return 0; }
    
    private:
        std::vector<Player> players;
};

describe("the Oca game", []() {
    it("accepts adding one player to the game", []() {
        OcaGame oca;
        Player playerOne("juan");

        oca.add(playerOne);

        expect(oca.playerCount()).toBe(1);
        expect(oca.getPlayers()[0].getName()).toBe("juan");
    });

    it("accepts adding multiple players to the game", []() {
        OcaGame oca;
        Player playerOne("juan");
        Player playerTwo("ana");

        oca.add(playerOne);
        oca.add(playerTwo);

        expect(oca.playerCount()).toBe(2);
        expect(oca.getPlayers()[0].getName()).toBe("juan");
        expect(oca.getPlayers()[1].getName()).toBe("ana");
    });

    it("it doesn't allow duplicate players", []() {
        OcaGame oca;
        Player playerOne("juan");

        oca.add(playerOne);

        assertRaises<DuplicatedPlayerException>([&]() {
            oca.add(playerOne);
        });

        expect(oca.playerCount()).toBe(1);
        expect(oca.getPlayers()[0].getName()).toBe("juan");
    });
});
