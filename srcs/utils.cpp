#include "utils.hpp"

std::string *split(std::string to_split, std::string split_at)
{
    size_t pos = 0;
    std::string *token = new std::string[100];
    unsigned int i = 0;

    while (i < to_split.size())
    {
        pos = to_split.find(split_at);
        token[i] = to_split.substr(0, pos);
        to_split.erase(0, pos + split_at.length());
        i++;
    }
    return (token);
}