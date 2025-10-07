# BSDSF22M035-OS-A02
# 🧮 Operating Systems – Programming Assignment 02

**Student Name:** Aiman Asif  
**Roll No:** BSDSF22M035  
**Department:** BS Data Science  
**Instructor:** Dr. Khurram Shahzad  
**Project:** Implementation of Linux `ls` Command (Custom Version)  
**Environment:** Git Bash + MinGW (Windows)

---

## 📘 Project Overview
This project implements a simplified version of the Linux `ls` command in C.  
It reproduces essential file listing features such as:
- Long listing (`-l`)
- Hidden files (`-a`)
- Horizontal display (`-x`)
- Recursive listing (`-R`)
- Colorized output by file type 🎨

Each version was built incrementally, managed using **Git branching**, **tagging**, and **merging** to simulate real-world software development workflows.

---

## ⚙️ Features Summary
| Version | Feature | Description |
|:--------:|:---------|:-------------|
| v1.0.0 | Basic Listing | Simple directory listing |
| v1.1.0 | Long Listing (-l) | Shows permissions, size, and timestamps |
| v1.2.0 | Multi-Column | Adaptive terminal-width output |
| v1.3.0 | Horizontal Display (-x) | Left-to-right display layout |
| v1.4.0 | Sorting | Alphabetical sorting using `qsort()` |
| v1.5.0 | Colorized Output | Colored output based on file type |
| v1.6.0 | Recursive Listing (-R) | Lists directories and subdirectories recursively |

---

## 🧩 Technologies Used
- **C Language** – System-level coding  
- **Git & GitHub** – Version control  
- **MinGW + Git Bash** – Compilation and testing  
- **Markdown** – Documentation  

---

## 🚀 How to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/<your-username>/BSDSF22M035-OS-A02.git
   ```
2. Navigate into the folder:
   ```bash
   cd BSDSF22M035-OS-A02
   ```
3. Compile the code:
   ```bash
   make
   ```
4. Run the program:
   ```bash
   ./bin/ls
   ```

---

## 📄 Project Report
For detailed explanation of versions, Git process, and testing summary —  
👉 **[Click here to view the full report](REPORT.md)**

---

## 🏁 Final Remarks
This project provided valuable experience in:
- Directory traversal and recursion in C  
- File metadata handling (`stat`, `lstat`)  
- Command-line option parsing  
- Git versioning and documentation  

✅ Completed successfully up to **Version 1.6.0**  
✅ All releases tested and tagged on GitHub  

**End of README**
