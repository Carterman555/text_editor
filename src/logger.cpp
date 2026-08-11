#include "logger.hpp"

sf::Clock Logger::clock;
std::queue<Log> Logger::log_queue;
std::unordered_set<int> Logger::ids;

void Logger::log_delayed(int id, std::string_view message) {

    bool already_queued = ids.count(id);
    if (!already_queued) {
        ids.insert(id);

        Log log(id, std::string(message));
        log_queue.push(log);
    }

    if (clock.getElapsedTime().asMilliseconds() >= 1000) {
        while (!log_queue.empty()) {
            std::cout << log_queue.front().message << std::endl;
            log_queue.pop();
        }
        ids.clear();

        clock.restart();
    }
}