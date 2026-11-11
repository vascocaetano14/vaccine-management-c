# 💉 Vaccine Management System in C

> **TL;DR:** A command-line program, written in **C**, that manages a vaccination centre: it registers batches of vaccines (with expiry dates and number of doses), gives doses to patients (always picking the batch that expires first), keeps the history of who got what and when, and simulates the passing of time so expired batches stop being used. The focus is on **data structures, dynamic memory and careful input validation**. University project for *Introduction to Algorithms and Data Structures* at Instituto Superior Técnico.

---

## What is this, in plain words?

A vaccination centre receives vaccines in **batches** (in Portuguese, *lotes*). Each batch has a code, an expiry date and a number of doses. Every time someone is vaccinated, the system has to:

- choose **which batch** to use (the one that expires soonest but is still valid, so nothing is wasted);
- **record** the vaccination (patient, vaccine, batch, date);
- refuse it if there is no stock, or if that person already got that vaccine today.

You talk to the program by typing short commands, one per line. It answers on the screen, in **English** by default or in **Portuguese** if started with `pt`.

## Commands

| Command | What it does | Example |
|---|---|---|
| `c` | Add a new batch of a vaccine | `c A0C0 31-7-2025 210 malaria` |
| `l` | List batches (all, or only some vaccines), sorted by expiry date | `l malaria tetano` |
| `a` | Vaccinate a patient (names with spaces go in quotes) | `a "Joao Miguel" tetano` |
| `r` | Withdraw a batch (no more doses can be given from it) | `r A0C0` |
| `d` | Delete vaccination records of a patient (optionally only for one date / batch) | `d xico 1-1-2025` |
| `u` | List all vaccinations, or those of one patient | `u xico` |
| `t` | Move the simulated date forward | `t 2-2-2025` |
| `q` | Quit | `q` |

Every command validates its input and prints a clear error (`invalid batch`, `invalid date`, `no stock`, `already vaccinated`, `no such user`, ...).

## What was implemented

- **Data structures** for vaccines, batches and vaccinations, with **dynamic memory** (`malloc`/`free`) so the number of patients and records is not fixed.
- **Sorting** of batches by expiry date and then alphabetically by code.
- **Date handling:** validation (including month lengths and leap years) and comparison of dates.
- **Robust parsing** of each command line: quoted names with spaces, hexadecimal batch codes, UTF-8 names.
- **Bilingual messages** (English / Portuguese) selected at start-up.
- Graceful handling of **out-of-memory** situations (frees everything and exits with a message).

## How to run it

You need `gcc` and `make` (Linux, macOS, or WSL / Git Bash on Windows).

```bash
make                         # compile
./proj                       # run and type commands (English)
./proj pt                    # same, with messages in Portuguese
make run                     # run the example in examples/example.in
```

## Files

```
main.c            # The whole program
proj.h            # Constants, error messages, data types and function declarations
Makefile
examples/example.in   # A short example session
```

## Authors

Vasco Caetano, Instituto Superior Técnico.

## Context

- **Course:** Introduction to Algorithms and Data Structures (Introdução aos Algoritmos e Estruturas de Dados), Instituto Superior Técnico, University of Lisbon
- **Language:** C
- The project statement belongs to the course teaching staff and is not included here.
