# Deterministic Text Adventure Engine (C++)

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![Deterministic Engine](https://img.shields.io/badge/Engine-Deterministic-green)
![Replay Validation](https://img.shields.io/badge/Replay-Validated-brightgreen)
![State Machine](https://img.shields.io/badge/Architecture-State%20Machine-orange)
![OOP Design](https://img.shields.io/badge/Design-OOP-purple)

> A deterministic C++ runtime engine designed around strict state control,
replay validation, and polymorphic execution strategies.

<p align="center">
  <img src="docs/Animation.gif" width="800"/>
</p>

---

## Overview

This project implements a console-based deterministic simulation engine focused on architectural clarity and strict state control.

Its primary goal is not gameplay complexity, but deterministic correctness and reproducible execution across runtime modes.

Every recorded session produces an identical state evolution and final outcome.

---

## Deterministic Simulation Model

The engine operates in a controlled tick-based simulation loop where:

- All state transitions are deterministic  
- State mutations are explicit and validated  
- Only minimal non-derivable data is recorded  
- Replay execution reconstructs the exact state evolution  

This guarantees reproducibility and enables regression-style automated validation.

There are no hidden side effects, undefined behavior dependencies, or implicit randomness affecting state progression.

---

## Execution Modes

```
adv-world.exe
adv-world.exe -save
adv-world.exe -load [-silent]
```


### Normal Mode
Runs the interactive simulation as a standard console execution.

### Save Mode
- Records deterministic session input and significant events  
- Writes:
  - `adv-world.steps` (minimal deterministic input stream)
  - `adv-world.result` (expected final outcome summary)

### Load Mode
- Reads `adv-world.steps`
- Reconstructs identical state transitions
- In silent mode, compares actual results against `adv-world.result`

### Silent Mode
- Executes without rendering  
- Validates actual results against expected output  
- Functions as a lightweight deterministic validation mechanism  

---

## Architecture

The engine is structured around polymorphic execution layers:

- `Game` — core deterministic simulation engine  
- `SaveGame` — extends `Game` to record deterministic sessions  
- `LoadGame` — extends `Game` to replay and validate sessions  

Architectural principles emphasized:

- Separation of concerns  
- Inheritance and polymorphism  
- Deterministic state control  
- Modular execution strategies  
- Clear runtime boundary definitions  

The simulation logic remains a single source of truth across all execution modes.

---

## Runtime Architecture Diagram

<p align="center">
  <img src="docs/deterministic-engine-architecture.png" width="680" style="max-width:100%;"/>
</p>

### Save / Replay Flow Clarification

- **Save mode** records the minimal deterministic input stream into `*.steps`
- It also writes an expected summary into `*.result`
- **Replay mode** reads `*.steps`, reconstructs the state evolution
- In silent mode, it compares the computed outcome against `*.result`

Replay execution never derives behavior from `.result` — it only validates against it.

---

## Dynamic Room Loading

Rooms are parsed from structured `.screen` files including:

- Board layout definition  
- Legend positioning constraints  
- Metadata directives (`#ROOM`, `#KEY`, `#DOOR`, `#RIDDLE`, etc.)  

The parser enforces strict format validation and produces descriptive error reporting for malformed definitions.

Malformed input cannot corrupt runtime state.

---

## Engineering Challenges Addressed

This project intentionally focused on architectural complexity rather than gameplay complexity.
The following challenges were central to its design:

### Deterministic Replay Guarantee

To ensure identical replay behavior:

- Strict control over state mutation  
- Clear separation between input logging and derived events  
- Recording only essential non-derivable data  
- Prevention of hidden non-deterministic behavior  

### Execution Strategy Isolation

Multiple runtime behaviors (normal, save, load, silent) are supported while preserving a single simulation engine core.

This required explicit separation between:

- Simulation logic  
- Rendering  
- Input handling  
- Validation  

---

## Technical Concepts Demonstrated

- Object-Oriented Design  
- Inheritance & Polymorphism  
- Deterministic simulation modeling  
- Replay-based validation systems  
- Structured metadata parsing  
- State-driven architecture  
- Explicit error handling  
- Runtime strategy separation  

---

## Project Structure

```
src/      → Core engine implementation
docs/     → Architecture diagrams & file format documentation
README.md → Project overview
```

## Build

- Requires: Windows x64, Visual Studio 2022  
- Open the solution in Visual Studio  
- Build in x64 (Debug or Release)  
- Run with optional command-line parameters  

All files are read from and written to the current working directory.

---

## Summary

This project demonstrates how deterministic runtime architecture can be applied to a state-driven engine with replay validation and execution-mode polymorphism.

It serves as a focused exercise in system design, reproducibility, and clean runtime separation in modern C++.
