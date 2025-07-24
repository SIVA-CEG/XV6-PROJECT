# 🧠 Priority-Based Process Management in xv6

A modified version of the [MIT xv6-public](https://github.com/mit-pdos/xv6-public) teaching operating system, extended with **priority-based scheduling** and several new **process management system calls** for better control, visibility, and educational value.

---

## 🚀 Project Objective

The default xv6 kernel provides minimal system call support for process management. This project enhances xv6 by introducing priority-based process control, allowing introspection of process metadata, dynamic adjustment of scheduling parameters, and additional utilities for process monitoring and debugging.

---

## 🛠️ System Calls Implemented

| System Call | Description |
|-------------|-------------|
| `int getpriority(int pid)` | Returns the current priority of a given process. |
| `int setpriority(int pid, int new_priority)` | Updates the priority of a specified process. |
| `int getparents(void)` | Returns the parent PID of the calling process. |
| `int getchildren(int pid)` | Prints the list of child PIDs of the given process. |
| `int setbursttime(int pid, int burst)` | Sets the expected CPU burst time for the given process. |
| `int getnumsyscalls(int pid)` | Returns the total number of system calls made by the given process. |

---

## 🧪 User-Level Testing Programs

Custom user programs created to validate each system call:

- `_ps` – Enhanced `ps` command that lists all processes with their PID, state, and priority.
- `_nice` – Adjusts the priority of a running process (like Unix `nice`).
- `test_getpriority`
- `test_setpriority`
- `test_getparents`
- `test_getchildren`
- `test_setbursttime`
- `test_getnumsyscalls`

---

## ⚙️ How to Build & Run

### 1. Clone the Repository
```bash
git clone https://github.com/SIVA-CEG/XV6-PROJECT.git
cd XV6-PROJECT/xv6-public
