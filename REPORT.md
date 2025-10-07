🧮 Operating Systems – Programming Assignment 02

Student Name: Aiman Asif
Roll No: BSDSF22M035
Department: BS Data Science
Instructor: Dr. Khurram Shahzad
Project: Implementation of Linux ls Command (Custom Version)
Environment: Git Bash + MinGW (Windows)

📘 Overview

This project implements a simplified version of the Linux ls command in C.
Each feature was developed incrementally and managed through Git branching, merging, and version tagging (v1.0.0 → v1.6.0).
The final version supports multiple command-line options such as -l, -a, -x, and -R, along with sorting and colorized output.

⚙️ Project Structure
BSDSF22M035-OS-A02/
│
├── bin/           # Compiled executable files (ls.exe)
├── obj/           # Object files
├── src/           # Source code (ls.c)
├── man/           # Manual or documentation
├── REPORT.md      # Project report
├── README.md      # Project summary
└── Makefile       # Build file

🧩 Feature Development Summary
🟢 Version 1.0.0 – Basic Listing

Implemented: Basic file listing using directory traversal.
Details:

Displays file and folder names in simple columns.

Set up project structure and initial Makefile.

Command Tested:

./bin/ls


Git Tag: v1.0.0

🟢 Version 1.1.0 – Long Listing (-l)

Implemented: Support for -l option to display:

File permissions

Number of links

Dummy user/group

File size and modification time

Key Functions Used: stat(), getpwuid(), getgrgid(), ctime()

Commands Tested:

./bin/ls -l


Git Tag: v1.1.0

🧠 Report Questions

Q1: What is the crucial difference between stat() and lstat()?
A: stat() follows symbolic links to the target file, while lstat() retrieves information about the link itself. In ls, lstat() is preferred to correctly display link metadata.

Q2: How can bitwise operators extract information from st_mode?
A: Use bitwise AND with macros like S_IFDIR or S_IRUSR.
Example:

if (st.st_mode & S_IFDIR) printf("Directory");

🟢 Version 1.2.0 – Multi-Column Display

Implemented: Default multi-column output (down-then-across).

Dynamically adjusts column width using longest filename.

Makes output compact and readable.

Commands Tested:

./bin/ls
./bin/ls -l


Git Tag: v1.2.0

🧠 Report Questions

Q1: Why can’t a single loop print “down then across” columns?
A: Because items must be printed row-by-row, referencing entries across multiple columns. You need nested loops and index calculation for proper layout.

Q2: What is the purpose of ioctl() here?
A: ioctl() retrieves terminal width for dynamic formatting.
Without it, output would use a fixed width (e.g., 80 chars), which breaks alignment on different screen sizes.

🟢 Version 1.3.0 – Horizontal Display (-x)

Implemented: Added -x flag for horizontal row-major output.

Displays files left-to-right across the screen, wrapping when full.

Simplifies visual scanning of filenames.

Commands Tested:

./bin/ls -x


Git Tag: v1.3.0

🧠 Report Questions

Q1: Which layout is more complex—vertical or horizontal?
A: Vertical (“down then across”) is more complex because it requires calculating rows and columns. Horizontal printing just tracks width and wraps lines.

Q2: How does the program choose between -l, -x, or default?
A: Using flags set by getopt(). Based on the flag value, the correct display function is called.

🟢 Version 1.4.0 – Alphabetical Sorting

Implemented: Alphabetical sorting with qsort() and strcmp().

Sorting applied before displaying in all modes.

Case-insensitive ordering for professional output.

Commands Tested:

./bin/ls
./bin/ls -a
./bin/ls -l


Git Tag: v1.4.0

🧠 Report Questions

Q1: Why must all entries be read into memory before sorting?
A: Because sorting requires random access to the full list. Streaming or incremental sorting is inefficient for directory listings.

Q2: Why does the qsort() comparison function use const void *?
A: Because qsort() is generic; it doesn’t know the data type.
You cast to (const char **) inside the function to compare strings.

🟢 Version 1.5.0 – Colorized Output

Implemented: File type–based color display using ANSI codes.

🔵 Blue → Directories

🟢 Green → Executables

🔴 Red → Archives (.tar/.zip)

⚪ White → Regular files

💜 Pink → Symbolic links

Commands Tested:

./bin/ls
./bin/ls -l


Git Tag: v1.5.0

🧠 Report Questions

Q1: How do ANSI escape codes color text?
A: Colors are printed using escape sequences like:

printf("\033[0;32mHello\033[0m");


[0;32m sets green; \033[0m resets color.

Q2: How to detect executables?
A: Check execute bits in st_mode:

if (st.st_mode & S_IXUSR) // owner executable

🟢 Version 1.6.0 – Recursive Listing (-R)

Implemented: Recursive directory traversal using -R.

Prints each directory, then descends into subdirectories.

Skips . and .. to avoid infinite recursion.

Works with all flags (-l, -a, etc.).

Commands Tested:

./bin/ls -R
./bin/ls -lR


Git Tag: v1.6.0

🧠 Report Questions

Q1: What is a “base case” in recursion?
A: It’s the condition that stops recursion. Here, recursion stops when no subdirectories remain or entries . and .. are reached.

Q2: Why construct the full path (e.g., dir/subdir)?
A: Because lstat() needs the full path to locate files correctly.
Without it, recursive calls would fail to find subdirectory entries.

🧪 Testing Summary
Option	Description	Compatible With
-l	Long listing format	-a, -R
-a	Include hidden files	-l, -R
-x	Multi-column horizontal layout	Default
-R	Recursive listing	-l, -a
Colors	Type-based coloring	All modes

✅ All features tested successfully on Git Bash (Windows 10).

🧾 Git and Version Control Summary

Each version was developed in separate branches and merged into main.
Annotated tags (v1.0.0 → v1.6.0) were pushed and released on GitHub.

Example Workflow:

git checkout -b feature-v1.3.0
# edit code
git add src/ls.c
git commit -m "feat(ls): add -a option for hidden files"
git checkout main
git merge feature-v1.3.0
git tag -a v1.3.0 -m "Version 1.3.0: Show hidden files (-a)"
git push origin main
git push origin v1.3.0

🧮 Tools and Commands Used
Tool	Purpose
Git Bash + MinGW	Coding and compilation
GCC (GNU Compiler)	C program compilation
Makefile	Automated build system
Git & GitHub	Version control and releases
Markdown (REPORT.md)	Documentation formatting
🏁 Final Remarks

This project provided hands-on experience with:

System-level C programming (opendir, readdir, stat)

File permissions and metadata handling

Recursion and argument parsing

Git branching, tagging, and merging

Professional documentation and versioned releases

✅ All features completed successfully up to v1.6.0
✅ All versions tagged and released on GitHub with binaries
