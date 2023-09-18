
#pragma once

#include <iostream>
#include <functional>
#include <sstream>

template <typename... Types>
size_t hasher(Types... data) {
    std::stringstream stream;
    (stream << ... << data);
    return std::hash<std::string>{}(stream.str());
}