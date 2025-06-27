# Heap Management System — March 2025

This project implements a dynamic heap memory manager using the **Buddy Allocation Algorithm** in C. It simulates a simple memory allocation system that efficiently handles memory requests by splitting and merging blocks, mimicking how low-level memory allocators work in operating systems.

## Features

- **Buddy System Allocation**: Allocates memory in powers of two using a split-and-merge approach.
- **Dynamic Allocation and Deallocation**: Supports real-time memory management with user input.
- **Free and Allocated Lists**: Displays current memory status by separating free and in-use blocks.
- **Minimal Overhead**: Lightweight metadata attached to each block to manage heap structure.
- **Interactive CLI**: Menu-driven interface for allocation, deallocation, and monitoring.

## Data Structures

- `Block` struct contains:
  - Block size
  - Allocation status (`is_free`)
  - Pointer to next block
- Free list is maintained as a linked list of blocks.

## File Structure

- `new3.c`: Source file containing the entire heap management implementation.

## Build Instructions

### Requirements
- GCC or any standard C compiler.

### Compilation
```bash
gcc -o heap_manager heap_management.c
