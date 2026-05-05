# 📚 Library Management System

A console-based **C++17** library management system with role-based access control, JSON-backed persistence, and live file-watching. Designed around clean service/repository separation and an event-driven architecture.

---

## Table of Contents

- [Features](#features)
- [Architecture Overview](#architecture-overview)
- [Project Structure](#project-structure)
- [Roles & Permissions](#roles--permissions)
- [Data Models](#data-models)
- [Prerequisites](#prerequisites)
- [Building & Running](#building--running)
- [Default Credentials](#default-credentials)
- [Data Storage](#data-storage)
- [Key Design Patterns](#key-design-patterns)

---

## Features

- **Role-based access** — three distinct roles (Admin, Librarian, Student) each with their own menu and permissions
- **Book management** — add, modify, delete, search, list all, and list available books
- **Member management** — add, modify, and delete library members (Admin only)
- **Borrow/return system** — issue books to members, track due dates, mark returns, detect overdue records
- **Owing members report** — list all members with unreturned/overdue books
- **JSON persistence** — all data is stored in human-readable `.json` files under `./data/`
- **Live file-watching** — `FileWatcher` polls the data directory; if a file changes on disk while the app is running, the corresponding repository reloads and merges automatically
- **Optimistic concurrency** — each JSON file carries a `version` field; write conflicts are detected and merged without data loss
- **EventBus** — a lightweight publish/subscribe bus decouples services from each other (e.g. `USER_CREATED`, `BOOK_UPDATED`, `FILE_CHANGED`)

---

## Architecture Overview

```
┌──────────────────────────────────────────────────────┐
│                        main.cpp                      │
│  1. Load JSON data   2. Start FileWatcher            │
│  3. Login loop       4. Role-based menu loop         │
└───────────────┬──────────────────────────────────────┘
                │
                ▼
┌──────────────────────────┐
│       AppContainer       │  ← Dependency injection root
│  Repos + Services wired  │
└────────────┬─────────────┘
             │
     ┌───────┴────────┐
     ▼                ▼
 Services          Repositories
 ─────────         ────────────
 AuthService    ←  JsonRepo<User>
 UserService    ←  JsonRepo<User>
 BookService    ←  JsonRepo<Book>
 BorrowRecord   ←  JsonRepo<BorrowRecord>
 Service

             │
             ▼
       Infrastructure
       ─────────────
       JSONLoader (singleton, in-memory cache)
       FileWatcher (singleton, inotify/poll)
       EventBus   (singleton, pub/sub)
```

---

## Project Structure

```
library_sys/
├── main.cpp                        # Entry point
├── Constants.h                     # File path constants (ROOT_PATH, etc.)
├── CMakeLists.txt                  # CMake build configuration
├── build.sh                        # Convenience build & run script
│
├── models/                         # Plain data classes + JSON serialisation
│   ├── User.{h,cpp}
│   ├── Book.{h,cpp}
│   ├── Member.{h,cpp}
│   └── BorrowRecord.{h,cpp}
│
├── repos/
│   ├── JsonRepo.h                  # Generic template repository
│   └── UsersRepo.{h,cpp}
│
├── application/                    # Business logic (services)
│   ├── AuthService.{h,cpp}         # Login, logout, role checks
│   ├── BookService.{h,cpp}         # CRUD + search for books
│   ├── UserService.{h,cpp}         # CRUD for members
│   ├── BorrowRecordService.{h,cpp} # Issue/return/overdue logic
│   └── Helper.{h,cpp}             # Shared utilities
│
├── core/
│   ├── AppContainer.{h,cpp}        # Wires repos → services
│   └── EventBus.{h,cpp}           # Publish/subscribe event system
│
├── infrastructure/
│   ├── JSONLoader.{h,cpp}          # Singleton JSON file cache
│   └── FileWatcher.{h,cpp}         # Watches ./data for file changes
│
├── ui/                             # Console UI handlers
│   ├── Menu.{h,cpp}               # Dispatches to role menu
│   ├── AdminMenu.{h,cpp}
│   ├── LibrarianMenu.{h,cpp}
│   ├── StudentMenu.{h,cpp}
│   ├── AuthUIHandler.{h,cpp}
│   ├── SharedUI.{h,cpp}           # Reusable search/modify widgets
│   ├── Prompt.{h,cpp}             # Type-safe input helpers
│   └── loginPage.{h,cpp}
│
└── data/                           # JSON data files (auto-created/updated)
    ├── users.json
    ├── books.json
    └── borrowRecord.json
```

---

## Roles & Permissions

| Feature                     | Admin | Librarian | Student |
|-----------------------------|:-----:|:---------:|:-------:|
| Add / Modify / Delete Book  | ✅    | ✅ (add/modify) | ❌   |
| Search Book                 | ✅    | ✅        | ✅      |
| List All Books              | ✅    | ✅        | ✅      |
| List Available Books        | ✅    | ❌        | ❌      |
| Issue Book to Member        | ✅    | ✅        | ❌      |
| Return a Book               | ✅    | ✅        | ❌      |
| View Borrow Records         | ✅    | ✅        | own only|
| View Owing Members          | ✅    | ✅        | ❌      |
| Add / Modify / Delete Member| ✅    | ❌        | ❌      |
| Show All Users              | ✅    | ❌        | ❌      |

---

## Data Models

### User
| Field      | Type   | Notes                              |
|------------|--------|------------------------------------|
| `id`       | string | UUID                               |
| `fullname` | string |                                    |
| `username` | string | Used for login                     |
| `password` | string | Plain-text (no hashing currently)  |
| `role`     | enum   | `1`=Admin, `2`=Librarian, `3`=Student |

### Book
| Field          | Type   | Notes                          |
|----------------|--------|--------------------------------|
| `id`           | string | UUID                           |
| `isbn`         | string | Unique book identifier         |
| `title`        | string |                                |
| `author`       | string |                                |
| `noOfCopies`   | int    | Total copies in library        |
| `available`    | int    | Currently available for borrow |
| `maxBorrowDays`| int    | Lending period in days         |

### BorrowRecord
| Field          | Type              | Notes                            |
|----------------|-------------------|----------------------------------|
| `id`           | string            | UUID                             |
| `borrowerId`   | string            | Foreign key → User.id            |
| `isbn`         | string            | Foreign key → Book.isbn          |
| `borrowedDate` | ISO-8601 datetime |                                  |
| `dueDate`      | ISO-8601 datetime | Computed from `maxBorrowDays`    |
| `returnedDate` | optional datetime | Null until book is returned      |

---

## Prerequisites

| Requirement | Version  | Notes                               |
|-------------|----------|-------------------------------------|
| C++ compiler| GCC / Clang supporting C++17 | |
| CMake       | ≥ 3.16   |                                     |
| [nlohmann/json](https://github.com/nlohmann/json) | any | Must be installed system-wide (e.g. via your package manager) |

Install nlohmann/json on Ubuntu/Debian:
```bash
sudo apt install nlohmann-json3-dev
```

On macOS with Homebrew:
```bash
brew install nlohmann-json
```

---

## Building & Running

### Quick start (using the provided script)

```bash
chmod +x build.sh
./build.sh
```

This script:
1. Removes any existing `build/` directory
2. Configures via CMake (`cmake -B build`)
3. Compiles using all available cores (`cmake --build build -j`)
4. Runs the resulting `./app` binary

### Manual build

```bash
cmake -B build
cmake --build build -j
./app
```

The compiled binary is placed in the project root as `app`.

---

## Default Credentials

The `data/users.json` file ships with three accounts for testing:

| Username | Password   | Role      |
|----------|------------|-----------|
| `admin`  | `12345678` | Admin     |
| `tmafa`  | `12345678` | Librarian |
| `edabire`| `12345678` | Student   |

> ⚠️ Passwords are stored in plain text. Do not use real passwords in this system.

---

## Data Storage

All data is persisted to JSON files in `./data/`. Each file follows this structure:

```json
{
  "version": 3,
  "data": [
    { ... record 1 ... },
    { ... record 2 ... }
  ]
}
```

The `version` field is incremented on every write and used for optimistic concurrency control — if two processes modify the same file, the `JsonRepo` detects the version mismatch on the next write and merges the changes before persisting.

The `FileWatcher` polls the directory at the end of each menu loop iteration. If a file's last-modified timestamp changes, it fires a `DATA_RELOADED` event, and the appropriate `JsonRepo` reloads and merges from disk automatically — no restart required.

---

## Key Design Patterns

| Pattern | Where Used | Purpose |
|---------|-----------|---------|
| **Singleton** | `JSONLoader`, `FileWatcher`, `EventBus` | Single shared instance across the app |
| **Repository** | `JsonRepo<T>` (generic template) | Decouples data access from business logic |
| **Service Layer** | `AuthService`, `BookService`, etc. | Encapsulates business rules |
| **Dependency Injection** | `AppContainer` | Constructs and wires all repos and services |
| **Observer / EventBus** | `EventBus` + `EventType` enum | Loose coupling between components |
| **Optimistic Concurrency** | `JsonRepo::add/update/remove` | Detect and merge concurrent file writes |

---

## License

No license file is present in the repository. Contact the repository owner for usage terms.
