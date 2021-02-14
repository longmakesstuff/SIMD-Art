//
// Created by long on 14.02.21.
//

#ifndef GRAVITATION_TIMEIT_H
#define GRAVITATION_TIMEIT_H


#include <string>
#include <sstream>
#include <chrono>
#include <iostream>

class TimeIt {
public:
    explicit TimeIt(const std::string &text);

    void end();

    static std::string format_number(uint64_t number);

private:
    static uint32_t recursion_level;
    static bool closed_last_line;
    bool closed_last_line_bkp;
    std::chrono::steady_clock::time_point start_time;
    static void indent();
};



#endif //GRAVITATION_TIMEIT_H
