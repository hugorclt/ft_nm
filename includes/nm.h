#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct s_file_handler {
    int fd;
}   t_file_handler;
