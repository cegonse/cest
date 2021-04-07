/*
 * Sample solution of: http://katayuno-app.herokuapp.com/katas/67
*/

#include <cest>
#include <map>


class Message {
    public:
        Message(std::string channel, std::string user, std::string text) {
            this->channel = channel;
            this->user = user;
            this->text = text;
        }

        Message() {}

        bool hasMention() {
            for (std::string word : tokenizedMessage()) {
                if (isMention(word)) return true;
            }

            return false;
        }

        std::string mention() {
            for (std::string word : tokenizedMessage()) {
                if (isMention(word)) return word.substr(1, word.length());
            }

            return "";
        }

        std::string user;
        std::string text;
        std::string channel;

        std::vector<std::string> tokenizedMessage() {
            std::regex regex("\\s+");
            std::sregex_token_iterator first{text.begin(), text.end(), regex, -1}, last;

            return {first, last};
        }

    private:
        bool isMention(std::string word) {
            return word[0] == '@';
        }
};

class EventBus {
    public:
        void subscribe(std::function<void(const Message&)> callback) {
            callbacks.push_back(callback);
        }

        void publish(const Message& message) {
            for (auto& callback : callbacks) {
                callback(message);
            }
        }

    private:
        std::vector<std::function<void(const Message&)> > callbacks;
};

class Bot {
    public:
        Bot(std::string username, EventBus *chat_events) {
            this->username = username;
            this->chat_events = chat_events;
        }

        std::string username;

        virtual void onNotification(Message message) = 0;

    protected:
        EventBus *chat_events;
};

class SampleBot : public Bot {
    public:
        SampleBot(std::string username, EventBus *chat_events) :
            Bot(username, chat_events),
            hasBeenNotified(false) {}

        void onNotification(Message message) {
            Message response(username, username, "Hola holita!");
            hasBeenNotified = true;

            chat_events->publish(response);
        }

        bool hasBeenNotified;
};

class VeoVeoBot : public Bot {
    public:
        VeoVeoBot(std::string username, EventBus *chat_events) :
            Bot(username, chat_events),
            game_started(false),
            word(""),
            channel("") {}

        void onNotification(Message message) {
            Message response(message.user, username, "");

            if (game_started) {
                if (message.user == channel) {
                    return;
                }

                if (hasCorrectResponse(message)) {
                    game_started = false;
                    generateWinningResponse(message, &response);
                } else {
                    generateFailureResponse(message, &response);
                }
            } else {
                word = message.tokenizedMessage()[2];
                channel = message.user;
                game_started = true;
                generateStartResponse(&response);
            }

            chat_events->publish(response);
        }

    private:
        bool hasCorrectResponse(Message message) {
            auto candidates = message.tokenizedMessage();

            for (size_t i=1; i<candidates.size(); ++i) {
                if (candidates[i] == word) {
                    return true;
                }
            }

            return false;
        }

        void generateStartResponse(Message *response) {
            response_text.str(std::string());
            response_text <<
                "VeoVeo una cosita que empieza por la letrita \"" <<
                static_cast<char>(std::toupper(word[0])) <<
                "\"";

            response->text = response_text.str();
        }

        void generateFailureResponse(const Message& message, Message *response) {
            response_text.str(std::string());
            response_text << "@" << message.user << " has fallado";

            response->channel = channel;
            response->text = response_text.str();
        }

        void generateWinningResponse(const Message& message, Message *response) {
            response_text.str(std::string());
            response_text << "¡Felicidades @" << message.user << " has ganado!";

            response->channel = channel;
            response->text = response_text.str();
        }

        bool game_started;
        std::string word;
        std::string channel;
        std::stringstream response_text;
};

class Channel {
    public:
        Channel(std::string name, EventBus *chat_events) {
            this->chat_events = chat_events;
            this->name = name;

            auto onChatEvent = [&](const Message& message) {
                if (message.channel == this->name) {
                    this->send(message);
                }
            };

            this->chat_events->subscribe(onChatEvent);
        }

        void send(Message message) {
            messages.push_back(message);

            if (message.hasMention()) {
                notifyBots(message);
            }
        }

        const Message& lastMessage() {
            return messages.back();
        }

        const std::vector<Message>& allMessages() {
            return messages;
        }

        void addBot(Bot *bot) {
            bots.push_back(bot);
        }

        std::string name;

    private:
        void notifyBots(Message message) {
            for (Bot *bot : bots) {
                if (bot->username == message.mention()) {
                    bot->onNotification(message);
                }
            }
        }

        std::vector<Message> messages;
        std::vector<Bot *> bots;
        EventBus *chat_events;
};

class Chat {
    public:
        Chat(EventBus *chat_events) {
            this->chat_events = chat_events;
        }

        ~Chat() {
            for (auto& channel : channels) {
                delete channel.second;
            }
        }

        void addUser(std::string username) {
            std::pair<std::string, Channel *> key_value_pair(username, new Channel(username, chat_events));
            channels.insert(key_value_pair);
        }

        void addBot(Bot *bot) {
            std::pair<std::string, Channel *> key_value_pair(bot->username, new Channel(bot->username, chat_events));

            channels.insert(key_value_pair);

            for (auto& channel : channels) {
                channel.second->addBot(bot);
            }
        }

        Channel *getChannel(std::string username) {
            return channels[username];
        }

    private:
        std::map<std::string, Channel *> channels;
        EventBus *chat_events;
};


describe("VeoVeo Chatbot", []() {
    it("adds a channel in the chat for each user", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);

        chat.addUser("maria");
        chat.addUser("pepe");

        expect(chat.getChannel("maria")).toBeNotNull();
        expect(chat.getChannel("pepe")).toBeNotNull();
    });

    it("is possible to send messages to channels by all members", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);
        Message message("pepe", "maria", "Hola!");
        Message other("pepe", "pepe", "Hola Maria!");

        chat.addUser("maria");
        chat.addUser("pepe");

        chat.getChannel("pepe")->send(message);
        chat.getChannel("pepe")->send(other);

        expect(chat.getChannel("pepe")->allMessages()[0].text).toBe("Hola!");
        expect(chat.getChannel("pepe")->allMessages()[0].user).toBe("maria");

        expect(chat.getChannel("pepe")->allMessages()[1].text).toBe("Hola Maria!");
        expect(chat.getChannel("pepe")->allMessages()[1].user).toBe("pepe");
    });

    it("considers messages starting with an @ to be mentions", [&]() {
        Message message("pepe", "ana", "@pepe te bajas a almorzar?");

        expect(message.hasMention()).toBeTruthy();
        expect(message.mention()).toBe("pepe");
    });

    it("notifies bots when a message is directed to them", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);
        Message message("bot", "user", "@bot hola bot!");
        SampleBot bot("bot", &chat_events);

        chat.addBot(&bot);
        chat.getChannel("bot")->send(message);

        expect(bot.hasBeenNotified).toBeTruthy();
        expect(chat.getChannel("bot")->lastMessage().text).toBe("Hola holita!");
        expect(chat.getChannel("bot")->lastMessage().user).toBe("bot");
    });

    it("forwards chat events to all registered peers", []() {
        EventBus chat_events;
        bool first_event_handler_called;
        bool second_event_handler_called;
        Message message("channel", "user", "text");
        Message first_received_message;
        Message second_received_message;

        chat_events.subscribe([&](const Message& message) {
            first_event_handler_called = true;
            first_received_message = message;
        });

        chat_events.subscribe([&](const Message& message) {
            second_event_handler_called = true;
            second_received_message = message;
        });

        chat_events.publish(message);

        expect(first_event_handler_called).toBeTruthy();
        expect(second_event_handler_called).toBeTruthy();
        expect(first_received_message.text).toBe(message.text);
        expect(first_received_message.user).toBe(message.user);
        expect(second_received_message.text).toBe(message.text);
        expect(second_received_message.user).toBe(message.user);
    });

    it("can be started with a word by sending a message to the VeoVeo bot's channel, which will answer on the user's channel", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);
        Message message("chatbot", "pepe", "@chatbot VeoVeo camisa");
        VeoVeoBot veo_veo_bot("chatbot", &chat_events);

        chat.addBot(&veo_veo_bot);
        chat.addUser("pepe");

        chat.getChannel("chatbot")->send(message);

        expect(chat.getChannel("pepe")->allMessages().size()).toBe(1);
        expect(chat.getChannel("pepe")->lastMessage().user).toBe("chatbot");
        expect(chat.getChannel("pepe")->lastMessage().text).toBe("VeoVeo una cosita que empieza por la letrita \"C\"");
    });

    it("will report failure when an user answers with an incorrect word", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);
        VeoVeoBot veo_veo_bot("chatbot", &chat_events);

        chat.addUser("pepe");
        chat.addUser("maria");
        chat.addBot(&veo_veo_bot);

        chat.getChannel("chatbot")->send(Message(
            "chatbot", "pepe", "@chatbot VeoVeo camisa"
        ));
        chat.getChannel("pepe")->send(Message(
            "pepe", "maria", "@chatbot cartel"
        ));

        expect(chat.getChannel("pepe")->allMessages().size()).toBe(3);
        expect(chat.getChannel("pepe")->allMessages()[0].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[0].text).toBe("VeoVeo una cosita que empieza por la letrita \"C\"");
        expect(chat.getChannel("pepe")->allMessages()[1].user).toBe("maria");
        expect(chat.getChannel("pepe")->allMessages()[1].text).toBe("@chatbot cartel");
        expect(chat.getChannel("pepe")->allMessages()[2].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[2].text).toBe("@maria has fallado");
    });

    it("will report success if the correct word is responded", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);
        VeoVeoBot veo_veo_bot("chatbot", &chat_events);

        chat.addUser("pepe");
        chat.addUser("maria");
        chat.addBot(&veo_veo_bot);

        chat.getChannel("chatbot")->send(Message(
            "chatbot", "pepe", "@chatbot VeoVeo camisa"
        ));
        chat.getChannel("pepe")->send(Message(
            "pepe", "maria", "@chatbot camisa"
        ));

        expect(chat.getChannel("pepe")->allMessages().size()).toBe(3);
        expect(chat.getChannel("pepe")->allMessages()[0].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[0].text).toBe("VeoVeo una cosita que empieza por la letrita \"C\"");
        expect(chat.getChannel("pepe")->allMessages()[1].user).toBe("maria");
        expect(chat.getChannel("pepe")->allMessages()[1].text).toBe("@chatbot camisa");
        expect(chat.getChannel("pepe")->allMessages()[2].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[2].text).toBe("¡Felicidades @maria has ganado!");
    });

    it("will ignore the channel's owner responses", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);
        VeoVeoBot veo_veo_bot("chatbot", &chat_events);

        chat.addUser("pepe");
        chat.addUser("maria");
        chat.addBot(&veo_veo_bot);

        chat.getChannel("chatbot")->send(Message(
            "chatbot", "pepe", "@chatbot VeoVeo camisa"
        ));
        chat.getChannel("pepe")->send(Message(
            "pepe", "pepe", "@chatbot camisa"
        ));

        expect(chat.getChannel("pepe")->allMessages().size()).toBe(2);
        expect(chat.getChannel("pepe")->allMessages()[0].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[0].text).toBe("VeoVeo una cosita que empieza por la letrita \"C\"");
        expect(chat.getChannel("pepe")->allMessages()[1].user).toBe("pepe");
        expect(chat.getChannel("pepe")->allMessages()[1].text).toBe("@chatbot camisa");
    });

    it("will can process several candidate words at once", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);
        VeoVeoBot veo_veo_bot("chatbot", &chat_events);

        chat.addUser("pepe");
        chat.addUser("maria");
        chat.addBot(&veo_veo_bot);

        chat.getChannel("chatbot")->send(Message(
            "chatbot", "pepe", "@chatbot VeoVeo camisa"
        ));
        chat.getChannel("pepe")->send(Message(
            "pepe", "maria", "@chatbot cafe casa coche camisa"
        ));

        expect(chat.getChannel("pepe")->allMessages().size()).toBe(3);
        expect(chat.getChannel("pepe")->allMessages()[0].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[0].text).toBe("VeoVeo una cosita que empieza por la letrita \"C\"");
        expect(chat.getChannel("pepe")->allMessages()[1].user).toBe("maria");
        expect(chat.getChannel("pepe")->allMessages()[1].text).toBe("@chatbot cafe casa coche camisa");
        expect(chat.getChannel("pepe")->allMessages()[2].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[2].text).toBe("¡Felicidades @maria has ganado!");
    });

    it("can only be started again after a game is running", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);
        VeoVeoBot veo_veo_bot("chatbot", &chat_events);

        chat.addUser("pepe");
        chat.addUser("maria");
        chat.addBot(&veo_veo_bot);

        chat.getChannel("chatbot")->send(Message(
            "chatbot", "pepe", "@chatbot VeoVeo camisa"
        ));
        chat.getChannel("chatbot")->send(Message(
            "chatbot", "pepe", "@chatbot VeoVeo coche"
        ));

        expect(chat.getChannel("pepe")->allMessages().size()).toBe(1);
        expect(chat.getChannel("pepe")->allMessages()[0].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[0].text).toBe("VeoVeo una cosita que empieza por la letrita \"C\"");
    });

    it("can be restarted in the same channel after the game finishes", [&]() {
        EventBus chat_events;
        Chat chat(&chat_events);
        VeoVeoBot veo_veo_bot("chatbot", &chat_events);

        chat.addUser("pepe");
        chat.addUser("maria");
        chat.addUser("ana");
        chat.addBot(&veo_veo_bot);

        chat.getChannel("chatbot")->send(Message(
            "chatbot", "pepe", "@chatbot VeoVeo camisa"
        ));
        chat.getChannel("pepe")->send(Message(
            "pepe", "maria", "@chatbot camisa"
        ));
        chat.getChannel("chatbot")->send(Message(
            "chatbot", "pepe", "@chatbot VeoVeo zapato"
        ));
        chat.getChannel("pepe")->send(Message(
            "pepe", "ana", "@chatbot zapato"
        ));

        expect(chat.getChannel("pepe")->allMessages().size()).toBe(6);
        expect(chat.getChannel("pepe")->allMessages()[0].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[0].text).toBe("VeoVeo una cosita que empieza por la letrita \"C\"");
        expect(chat.getChannel("pepe")->allMessages()[1].user).toBe("maria");
        expect(chat.getChannel("pepe")->allMessages()[1].text).toBe("@chatbot camisa");
        expect(chat.getChannel("pepe")->allMessages()[2].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[2].text).toBe("¡Felicidades @maria has ganado!");
        expect(chat.getChannel("pepe")->allMessages()[3].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[3].text).toBe("VeoVeo una cosita que empieza por la letrita \"Z\"");
        expect(chat.getChannel("pepe")->allMessages()[4].user).toBe("ana");
        expect(chat.getChannel("pepe")->allMessages()[4].text).toBe("@chatbot zapato");
        expect(chat.getChannel("pepe")->allMessages()[5].user).toBe("chatbot");
        expect(chat.getChannel("pepe")->allMessages()[5].text).toBe("¡Felicidades @ana has ganado!");
    });
});
