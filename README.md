# Ghost Hunt Simulation (C, multithreaded)

A ghost-hunting simulation written in C, inspired by the game *Phasmophobia*. One ghost and four hunters move around a haunted house **at the same time**, each running on its own thread.

This was a university team project. I built it to practise **threads, shared memory and synchronization** in C.

## How it works

- The house is a set of connected rooms (kitchen, basement, garage and so on), stored as **linked lists**.
- **The ghost** wanders between rooms and leaves evidence behind (EMF, temperature, fingerprints or sound). Different ghost types leave different evidence.
- **Four hunters** each carry one tool, so each can only detect one kind of evidence. They move around, collect evidence and share what they find.
- Hunters get **scared** when they're in the same room as the ghost. If fear gets too high, they leave.
- **The hunters win** if they collect enough evidence to identify the ghost. **The ghost wins** if it scares everyone away.

## The interesting part: threads and locks

- The program starts **5 threads** with `pthread_create`: 1 ghost + 4 hunters, all running at once.
- Many threads can enter the same room at the same time, so each room has its own **lock** (a semaphore used as a mutex: `sem_wait` / `sem_post`).
- A thread has to take the room's lock before it reads or changes anything in that room, like adding or picking up evidence. That stops two threads from changing the same data at once (a **race condition**).
- At the end, `pthread_join` waits for every thread to finish before printing the results.

## Run it

You need Linux (or WSL) and `gcc`.

```bash
cd PhasmophobiaSimulation
make
./finalProject
```

It asks for 4 hunter names, then prints the ghost's and hunters' moves and who won.

To clean up the build files:

```bash
make clean
```

## Files

| File | What it does |
|---|---|
| `main.c` | Sets everything up, starts the 5 threads, prints the results |
| `defs.h` | All the data types, constants and function declarations |
| `building.c` | Builds the house and connects the rooms |
| `room.c` | Room linked list and room helpers |
| `ghost.c` | Ghost behaviour: moving, leaving evidence, getting bored |
| `hunter.c` | Hunter behaviour: moving, collecting evidence, fear |
| `evidence.c` | Evidence linked list, plus sharing evidence between hunters |

## What I'd improve now

- Fix the remaining compiler warnings (`gcc -Wall`) and free every bit of memory (check with `valgrind`).
- Use `pthread_mutex_t` for the room locks. A mutex is a clearer fit than a semaphore when you only need "one at a time".
- Add a random seed option so a run can be repeated exactly. That makes timing bugs much easier to debug.
