//
// Created by remza on 28.12.2025.
//

#pragma once


class Logger{

public:
    static std::ostream& debug() {
        return log(DEBUG);
    }
    static std::ostream& info() {
        return log(INFO);
    }
    static std::ostream& warning() {
        return log(WARNING);
    }
    static std::ostream& error() {
        return log(ERROR);
    }


private:
    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static std::ostream& log(const Level level) {
        std::ostream& output = std::cout;

        switch (level) {
            case DEBUG:
                output << "[DEBUG] ";
                break;
            case INFO:
                output << "[INFO] ";
                break;
            case WARNING:
                output << "[WARNING] ";
                break;
            case ERROR:
                output << "[ERROR] ";
                break;
        }
        return output;
    }
};