#include <iostream>
#include <map>
#include <functional>
#include <utility>

typedef std::function<void ()> func;

void exit()
{
    return ;
}

void store_commands()
{
    std::map<std::string, void(*)()> commands;
    commands["/exit"] = exit;
    commands.find("/exit")->second();
    
}