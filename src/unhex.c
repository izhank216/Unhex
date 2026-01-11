#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <Python.h>

static void *map_file(const char *path, size_t *size) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) return NULL;
    struct stat st;
    fstat(fd, &st);
    *size = st.st_size;
    void *data = mmap(0, *size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return data;
}

static void unmap_file(void *data, size_t size) {
    if (data) munmap(data, size);
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
            if (i + j < size)
                printw("%02x ", data[i + j]);
            else
                printw("   ");
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
    if (argc < 2) {
        printf("Usage: unhex FILE\n");
        return 1;
    }

    const char *filename = argv[1];

    Py_Initialize();

    PyObject *hex_module = PyImport_ImportModule("Hex");
    PyObject *view_module = PyImport_ImportModule("View");

    if (!hex_module || !view_module) {
        PyErr_Print();
        Py_Finalize();
        return 1;
    }

    PyObject *data = PyObject_CallMethod(hex_module, "read", "(s)", filename);
    if (!data) {
        printf("Error: Could not read file '%s'\n", filename);
        PyErr_Print();
        Py_Finalize();
        return 1;
    }

    PyObject *formatted = PyObject_CallMethod(view_module, "format", "(O)", data);
    if (!formatted) {
        printf("Error: Could not format data for '%s'\n", filename);
        PyErr_Print();
        Py_DECREF(data);
        Py_Finalize();
        return 1;
    }

    const char *out = PyUnicode_AsUTF8(formatted);
    if (out) printf("%s\n", out);

    render_ncurses((unsigned char *)PyBytes_AS_STRING(data), PyBytes_GET_SIZE(data));

    Py_DECREF(data);
    Py_DECREF(formatted);
    Py_DECREF(hex_module);
    Py_DECREF(view_module);
    Py_Finalize();

    return 0;
}
