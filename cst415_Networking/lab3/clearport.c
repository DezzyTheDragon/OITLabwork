#include <stdio.h>
#include <stdlib.h>

#include "getport.h"
#include "utils.h"

int main(int argc, char **argv)
{
    if (argc < 2) Fatal_Error("clearport <name>\n");
    int port;
    int result;

    setup_ns("unix.cset.oit.edu", 50000);

    port = lookup_port(argv[1]);
    if (port < 0)
    {
        printf("Service not found\n");
    }
    else
    {
        result = release_port(argv[1], port);

        printf("Release: %d\n", result);
    }

    return 0;
}
