#include "server.hpp"
#include "check.hpp"

int main(int argc, char **argv)
{
    if (check_args(argc, argv) == -1)
        return (1);

    Server  server;

    server.set_config(ft_atoi(argv[1]), argv[2]);
    if (!server.setup())
        return (1);
    server.run();

    return (0);
}