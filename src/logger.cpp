#include "logger.hpp"

sf::Clock Logger::clock;
queue<Log> Logger::log_queue;
unordered_set<int> Logger::ids;

void Logger::log_delayed(int id, string message) {

    bool already_queued = ids.count(id);
    if (!already_queued) {
        ids.insert(id);

        Log log(id, message);
        log_queue.push(log);
    }

    if (clock.getElapsedTime().asMilliseconds() >= 1000) {
        while (!log_queue.empty()) {
            cout << log_queue.front().message << endl;
            log_queue.pop();
        }
        ids.clear();

        clock.restart();
    }
}