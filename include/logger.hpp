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
    static void log_delayed(int id, string_view message);
    static void print_trace();
private:
    static sf::Clock clock;

    static queue<Log> log_queue;
    static unordered_set<int> ids;
};