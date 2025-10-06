/*
 * Programming Assignment 02: lsv1.2.0
 * Feature: Column display (down-then-across) + keep -l support
 *
 * Windows-friendly version (no S_ISLNK, S_ISSOCK)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#ifndef _WIN32
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#endif

/* function declarations */
void do_ls(const char *dir, int long_flag);
void show_file_info(const char *dir, const char *filename);
void mode_to_str(mode_t mode, char *buf);
int get_term_width(void);

/* ------------------------------------------------------------------ */
int main(int argc, char *argv[])
{
    int long_flag = 0;
    int opt;

    /* parse options: only -l for now */
    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
            case 'l': long_flag = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-l] [dir...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind == argc) {
        do_ls(".", long_flag);
    } else {
        for (int i = optind; i < argc; ++i) {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i], long_flag);
            if (i + 1 < argc) puts("");
        }
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/* get terminal width: try ioctl on POSIX, fallback to 80 on Windows */
int get_term_width(void)
{
#ifdef _WIN32
    return 80;  /* fallback */
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) return 80;
    if (w.ws_col == 0) return 80;
    return (int)w.ws_col;
#endif
}

/* ------------------------------------------------------------------ */
/* convert mode to string like "drwxr-xr-x" */
void mode_to_str(mode_t mode, char *buf)
{
    buf[0] = S_ISDIR(mode) ? 'd' :
             S_ISCHR(mode) ? 'c' :
             S_ISBLK(mode) ? 'b' :
             S_ISFIFO(mode)? 'p' : '-';

    buf[1] = (mode & S_IRUSR) ? 'r' : '-';
    buf[2] = (mode & S_IWUSR) ? 'w' : '-';
    buf[3] = (mode & S_IXUSR) ? 'x' : '-';
    buf[4] = (mode & S_IRGRP) ? 'r' : '-';
    buf[5] = (mode & S_IWGRP) ? 'w' : '-';
    buf[6] = (mode & S_IXGRP) ? 'x' : '-';
    buf[7] = (mode & S_IROTH) ? 'r' : '-';
    buf[8] = (mode & S_IWOTH) ? 'w' : '-';
    buf[9] = (mode & S_IXOTH) ? 'x' : '-';
    buf[10] = '\0';
}

/* ------------------------------------------------------------------ */
/* show file info for -l mode (dummy user/group for Windows) */
void show_file_info(const char *dir, const char *filename)
{
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir, filename);

    struct stat st;
    if (stat(path, &st) == -1) {
        perror("stat");
        return;
    }

    char perms[11];
    mode_to_str(st.st_mode, perms);

    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&st.st_mtime));

    printf("%s %2ld %-8s %-8s %8lld %s %s\n",
           perms,
           (long)st.st_nlink,
           "user",   // dummy owner
           "group",  // dummy group
           (long long)st.st_size,
           timebuf,
           filename);
}

/* ------------------------------------------------------------------ */
/* do_ls: if long_flag, print -l style; otherwise print in columns */
void do_ls(const char *dir, int long_flag)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL) {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    errno = 0;

    if (long_flag) {
        /* long listing */
        while ((entry = readdir(dp)) != NULL) {
            if (entry->d_name[0] == '.') continue;
            show_file_info(dir, entry->d_name);
        }
        if (errno != 0) perror("readdir failed");
        closedir(dp);
        return;
    }

    /* ---------- column mode ---------- */
    size_t cap = 64;
    size_t n = 0;
    char **names = malloc(cap * sizeof(char*));
    if (!names) { closedir(dp); perror("malloc"); return; }

    size_t maxlen = 0;
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        if (n + 1 > cap) {
            cap *= 2;
            char **tmp = realloc(names, cap * sizeof(char*));
            if (!tmp) { perror("realloc"); break; }
            names = tmp;
        }
        names[n] = strdup(entry->d_name);
        if (!names[n]) { perror("strdup"); break; }
        size_t L = strlen(names[n]);
        if (L > maxlen) maxlen = L;
        n++;
    }

    if (errno != 0) perror("readdir failed");
    closedir(dp);

    if (n == 0) {
        free(names);
        return;
    }

    int term_w = get_term_width();
    size_t col_width = maxlen + 2;
    if (col_width == 0) col_width = 1;
    int cols = term_w / (int)col_width;
    if (cols < 1) cols = 1;
    size_t rows = (n + cols - 1) / cols;

    for (size_t r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            size_t idx = (size_t)c * rows + r;
            if (idx >= n) continue;
            printf("%-*s", (int)col_width, names[idx]);
        }
        putchar('\n');
    }

    for (size_t i = 0; i < n; ++i) free(names[i]);
    free(names);
}


