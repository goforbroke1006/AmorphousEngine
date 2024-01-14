//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_LOGGER_H
#define AMORPHOUS_ENGINE_LOGGER_H

#include <iostream>
#include <string>

namespace AmE {
    class Logger {
    public:

        inline
        static void Trace(const std::string &message) {
            std::cout << "TRACE: " << message << std::endl;
        }

        inline
        static void Debug(const std::string &message) {
            std::cout << "DEBUG: " << message << std::endl;
        }

        inline
        static void Error(const std::string &message) {
            std::cerr << "ERROR: " << message << std::endl;
        }
    };
}

#endif //AMORPHOUS_ENGINE_LOGGER_H
