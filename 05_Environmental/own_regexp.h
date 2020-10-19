#pragma once

#include <ncurses.h>

int matchRegexp(const unsigned char* pattern, const unsigned char* subject, WINDOW* output);

