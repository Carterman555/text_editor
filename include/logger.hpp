#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <SFML/System.hpp>

using namespace std;

struct Log {

    Log(int id, string message) {
        this->id = id;
        this->message = message;
    }

    int id;
    string message;
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
    static void log_delayed(int id, string_view message);

private:
    static sf::Clock clock;

    static queue<Log> log_queue;
    static unordered_set<int> ids;
};