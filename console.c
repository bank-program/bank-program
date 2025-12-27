
#include <stdio.h>
#include "console.h"

void msg_info(const char *text) { printf("%s%s%s\n", FG_CYAN, text, RESET); }
void msg_ok(const char *text)   { printf("%s%s%s\n", FG_GREEN, text, RESET); }
void msg_warn(const char *text) { printf("%s%s%s\n", FG_YELLOW, text, RESET); }
void msg_err(const char *text)  { printf("%s%s%s\n", FG_RED, text, RESET); }

void header(const char *title)    { printf("%s%s %s %s\n", BG_BLUE, BOLD, title, RESET); }
void subheader(const char *title) { printf("%s%s-- %s --%s\n", FG_MAGENTA, BOLD, title, RESET); }
void prompt(const char *text)     { printf("%s%s%s", FG_GOLD, text, RESET); }

#ifdef _WIN32
#include <windows.h>
void enableANSI(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    // ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004
    SetConsoleMode(hOut, dwMode | 0x0004);
}
#endif
