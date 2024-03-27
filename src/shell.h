#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <stdbool.h>

#include "inputline.h"
#include "scanner.h"

bool parseInputLine(InputLine **inputLine, List *lp);

#endif
