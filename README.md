# OS Lab - Known Issues & Challenges

This file lists common problems encountered while working on OS Lab C files (`worker.c`, `ch_4.c`, etc.) and tips to avoid them.

## File & Compilation Issues

- **File not found errors**
  - Example: `cc1: fatal error: challenges/worker.c: No such file or directory`
  - Cause: Folder name mismatch, typos, or wrong working directory.

- **Undefined reference to main**
  - Occurs when compiling files without a `main()` function directly.
  - Fix: Compile as object files (`gcc -c ch_4.c`) and link with a main program.

- **Linking modules incorrectly**
  - Files like `ch_4.c` must be linked with main program object files to run.

- **Path issues**
  - Ensure correct relative paths in compilation commands and Makefiles.
  - Watch out for trailing slashes and case sensitivity in folder and file names.

Generally speaking, everything was steady going except challenge 4. Having to compile the worker.c file and ./ch4.. was challenging.
