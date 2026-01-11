#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *read_file(const char *path, size_t *size) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) return NULL;
    struct stat st;
    if (fstat(fd, &st) != 0) { close(fd); return NULL; }
    *size = st.st_size;
    unsigned char *data = malloc(*size);
    if (!data) { close(fd); return NULL; }
    if (read(fd, data, *size) != *size) { free(data); close(fd); return NULL; }
    close(fd);
    return data;
}

static char *format_hex(const unsigned char *data, size_t size) {
    size_t lines = (size + 15) / 16;
    size_t bufsize = lines * (10 + 16*3 + 1 + 16 + 1);
    char *out = malloc(bufsize + 1);
    if (!out) return NULL;
    char *p = out;
    for (size_t i = 0; i < size; i += 16) {
        p += sprintf(p, "%08lx  ", i);
        for (size_t j = 0; j < 16; j++) {
            if (i + j < size) p += sprintf(p, "%02x ", data[i + j]);
            else p += sprintf(p, "   ");
        }
        *p++ = ' ';
        for (size_t j = 0; j < 16 && i + j < size; j++) {
            unsigned char c = data[i + j];
            *p++ = isprint(c) ? c : '.';
        }
        *p++ = '\n';
    }
    *p = 0;
    return out;
}

static void render_ncurses(const unsigned char *data, size_t size) {
    initscr();
    start_color();
    use_default_colors();
    init_pair(1, COLOR_CYAN, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_WHITE, -1);
    size_t row = 0;
    for (size_t i = 0; i < size; i += 16) {
        if (row >= LINES - 1) break;
        attron(COLOR_PAIR(3));
        mvprintw(row, 0, "%08lx  ", i);
        attron(COLOR_PAIR(1));
        for (size_t j = 0; j < 16; j++) {
            if (i + j < size) printw("%02x ", data[i + j]);
            else printw("   ");
        }
        printw(" ");
        attron(COLOR_PAIR(2));
        for (size_t j = 0; j < 16 && i + j < size; j++) {
            unsigned char c = data[i + j];
            addch(isprint(c) ? c : '.');
        }
        row++;
    }
    refresh();
    getch();
    endwin();
}

int main(int argc, char **argv) {
    if (argc < 2) { printf("Usage: unhex FILE\n"); return 1; }
    const char *filename = argv[1];
    size_t size;
    unsigned char *data = read_file(filename, &size);
    if (!data) { printf("Error reading file '%s'\n", filename); return 1; }
    char *formatted = format_hex(data, size);
    if (formatted) { printf("%s", formatted); free(formatted); }
    render_ncurses(data, size);
    free(data);
    return 0;
}
