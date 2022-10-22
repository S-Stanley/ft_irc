#include "server.hpp"
#include "check.hpp"

int main(int argc, char **argv)
{
    if (check_args(argc, argv) == -1)
    {
        return (0);
    }

    Server  server;

    server.set_config(atoi(argv[1]), argv[2]);
    server.setup();
    server.run();

    return (0);
}