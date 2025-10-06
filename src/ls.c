/*
* Programming Assignment 02: lsv1.1.0 (Windows-friendly)
* Feature: Long Listing (-l)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

void do_ls(const char *dir, int long_flag);
void show_file_info(const char *dir, const char *filename);

int main(int argc, char *argv[])
{
    int long_flag = 0;
    int opt;

    // parse options
    while ((opt = getopt(argc, argv, "l")) != -1)
    {
        switch (opt)
        {
        case 'l':
            long_flag = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-l] [dir...]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind == argc)
    {
        do_ls(".", long_flag);
    }
    else
    {
        for (int i = optind; i < argc; i++)
        {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i], long_flag);
            puts("");
        }
    }
    return 0;
}

// permissions string
void mode_to_str(mode_t mode, char *buf)
{
    buf[0] = S_ISDIR(mode) ? 'd' : '-';
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

void do_ls(const char *dir, int long_flag)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }
    errno = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;

        if (long_flag)
        {
            show_file_info(dir, entry->d_name);
        }
        else
        {
            printf("%s\n", entry->d_name);
        }
    }

    if (errno != 0)
    {
        perror("readdir failed");
    }

    closedir(dp);
}

void show_file_info(const char *dir, const char *filename)
{
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir, filename);

    struct stat st;
    if (stat(path, &st) == -1)
    {
        perror("stat");
        return;
    }

    char perms[11];
    mode_to_str(st.st_mode, perms);

    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&st.st_mtime));

    // Windows: no pwd.h or grp.h → print dummy user/group
    printf("%s %2ld %-8s %-8s %8lld %s %s\n",
           perms,
           (long)st.st_nlink,
           "user",   // dummy owner
           "group",  // dummy group
           (long long)st.st_size,
           timebuf,
           filename);
}
