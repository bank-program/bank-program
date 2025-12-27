
#ifndef CONSOLE_H
#define CONSOLE_H


#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"


#define FG_RED      "\033[38;5;196m"
#define FG_GREEN    "\033[38;5;46m"
#define FG_YELLOW   "\033[38;5;226m"
#define FG_CYAN     "\033[38;5;51m"
#define FG_BLUE     "\033[38;5;27m"
#define FG_MAGENTA  "\033[38;5;201m"
#define FG_WHITE    "\033[38;5;15m"
#define FG_GREY     "\033[38;5;245m"
#define FG_GOLD     "\033[38;5;178m"


#define BG_BLUE     "\033[48;5;27m"
#define BG_PURPLE   "\033[48;5;91m"
#define BG_GREEN    "\033[48;5;34m"
#define BG_RED      "\033[48;5;196m"
#define BG_GOLD     "\033[48;5;178m"


void msg_info(const char *text);
void msg_ok(const char *text);
void msg_warn(const char *text);
void msg_err(const char *text);

void header(const char *title);
void subheader(const char *title);
void prompt(const char *text);

#ifdef _WIN32
void enableANSI(void);
#endif

#endif
