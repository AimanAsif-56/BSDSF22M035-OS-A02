#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <limits.h>

#define SPACING 2

/* ---------- Color escape codes (ANSI) ---------- */
#define RESET   "\033[0m"
#define BLUE    "\033[1;34m"
#define GREEN   "\033[1;32m"
#define GRAY    "\033[0;37m"

/* ---------- Permission string helper ---------- */
void perm_string(mode_t mode, char *out) {
    out[0] = S_ISDIR(mode) ? 'd' :
             S_ISCHR(mode) ? 'c' :
             S_ISBLK(mode) ? 'b' :
             S_ISFIFO(mode)? 'p' : '-';
    out[1] = (mode & S_IRUSR) ? 'r' : '-';
    out[2] = (mode & S_IWUSR) ? 'w' : '-';
    out[3] = (mode & S_IXUSR) ? 'x' : '-';
    out[4] = (mode & S_IRGRP) ? 'r' : '-';
    out[5] = (mode & S_IWGRP) ? 'w' : '-';
    out[6] = (mode & S_IXGRP) ? 'x' : '-';
    out[7] = (mode & S_IROTH) ? 'r' : '-';
    out[8] = (mode & S_IWOTH) ? 'w' : '-';
    out[9] = (mode & S_IXOTH) ? 'x' : '-';
    out[10] = '\0';
}

/* ---------- Detect color for a file ---------- */
const char *file_color(const char *dirpath, const char *name) {
    char full[PATH_MAX];
    snprintf(full, sizeof(full), "%s/%s", dirpath, name);
    struct stat sb;
    if (stat(full, &sb) == -1) return RESET;

    if (S_ISDIR(sb.st_mode)) return BLUE;               // directory
    if (sb.st_mode & S_IXUSR) return GREEN;             // executable
    if (name[0] == '.') return GRAY;                    // hidden file
    return RESET;                                       // normal file
}

/* ---------- Collect directory entries ---------- */
char **collect_entries(const char *path, int include_hidden, int *out_n) {
    DIR *dir = opendir(path);
    if (!dir) { perror("opendir"); *out_n = 0; return NULL; }

    struct dirent *entry;
    int cap = 64, n = 0;
    char **names = malloc(sizeof(char*) * cap);
    if (!names) { perror("malloc"); closedir(dir); *out_n = 0; return NULL; }

    while ((entry = readdir(dir)) != NULL) {
        if (!include_hidden && entry->d_name[0] == '.') continue;
        if (n >= cap) {
            cap *= 2;
            char **tmp = realloc(names, sizeof(char*) * cap);
            if (!tmp) { perror("realloc"); free(names); closedir(dir); *out_n = 0; return NULL; }
            names = tmp;
        }
        names[n++] = _strdup(entry->d_name);
    }
    closedir(dir);
    *out_n = n;
    return names;
}

/* ---------- qsort comparator ---------- */
int cmp_names(const void *a, const void *b) {
    const char *const *pa = a;
    const char *const *pb = b;
    return _stricmp(*pa, *pb);
}

/* ---------- Free helper ---------- */
void free_entries(char **names, int n) {
    for (int i = 0; i < n; ++i) free(names[i]);
    free(names);
}

/* ---------- Long listing ---------- */
void print_long_for_one(const char *dirpath, const char *name) {
    char full[PATH_MAX];
    snprintf(full, sizeof(full), "%s/%s", dirpath, name);
    struct stat sb;
    if (stat(full, &sb) == -1) { perror("stat"); return; }

    char perm[11];
    perm_string(sb.st_mode, perm);
    const char *user = "user";
    const char *group = "group";
    char timebuf[64];
    struct tm *tm = localtime(&sb.st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm);

    const char *color = file_color(dirpath, name);
    printf("%s %3ld %-8s %-8s %8ld %s %s%s%s\n",
           perm, (long)sb.st_nlink,
           user, group,
           (long)sb.st_size,
           timebuf,
           color, name, RESET);
}

/* ---------- Column output ---------- */
void print_names_in_columns(const char *path, char **names, int n) {
    int maxlen = 0;
    for (int i = 0; i < n; ++i)
        if ((int)strlen(names[i]) > maxlen) maxlen = strlen(names[i]);

    int termw = 80;
    int colw = maxlen + SPACING;
    int cols = termw / colw;
    if (cols < 1) cols = 1;
    int rows = (n + cols - 1) / cols;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int idx = r + c * rows;
            if (idx < n) {
                const char *color = file_color(path, names[idx]);
                printf("%s%-*s%s", color, colw, names[idx], RESET);
            }
        }
        putchar('\n');
    }
}

/* ---------- Horizontal (-x) ---------- */
void print_names_horizontal(const char *path, char **names, int n) {
    int termw = 80, pos = 0;
    for (int i = 0; i < n; ++i) {
        const char *color = file_color(path, names[i]);
        int wlen = strlen(names[i]) + SPACING;
        if (pos + wlen > termw) { putchar('\n'); pos = 0; }
        printf("%s%-*s%s", color, wlen, names[i], RESET);
        pos += wlen;
    }
    if (pos) putchar('\n');
}

/* --------------------------- main() --------------------------- */
int main(int argc, char *argv[]) {
    int long_flag = 0, all_flag = 0, horiz_flag = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                if (argv[i][j] == 'l') long_flag = 1;
                else if (argv[i][j] == 'a') all_flag = 1;
                else if (argv[i][j] == 'x') horiz_flag = 1;
            }
        }
    }

    const char *path = ".";
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') { path = argv[i]; break; }
    }

    int n = 0;
    char **names = collect_entries(path, all_flag, &n);
    if (!names || n == 0) return 0;

    qsort(names, n, sizeof(char*), cmp_names);

    if (long_flag)
        for (int i = 0; i < n; ++i) print_long_for_one(path, names[i]);
    else if (horiz_flag)
        print_names_horizontal(path, names, n);
    else
        print_names_in_columns(path, names, n);

    free_entries(names, n);
    return 0;
}
