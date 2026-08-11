#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <SFML/System.hpp>

struct Log {

    Log(int id, std::string message) {
        this->id = id;
        this->message = message;
    }

    int id;
    std::string message;
};

class Logger {
public:

    /**
     * @brief queue the message and only print messages with the same id every second
     *
     * This method only works properly if called from an update loop. It limits the message from
     * being displayed every loop to being displayed every second. The message will be a second
     * behind.
     */
    static void log_delayed(int id, std::string_view message);

private:
    static sf::Clock clock;

    static std::queue<Log> log_queue;
    static std::unordered_set<int> ids;
};