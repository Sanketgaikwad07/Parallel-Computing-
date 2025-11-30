# Parallel-Computing-


This repository contains concepts, notes, and implementations related to Parallel Computing, including matrix multiplication, block partitioning, parallel algorithms, and performance analysis.
It is specially structured for M.Sc. Scientific Computing  students.

 What’s Inside?
1️ Parallel Computing Concepts

Introduction to Parallel Computing

Advantages of Parallelism

Data Parallelism vs Task Parallelism

Shared Memory & Distributed Memory Models

Message Passing (MPI Basics)

Cache Coherence & Synchronization


 2️ Matrix Multiplication (Parallel)

Includes important algorithms for parallel matrix multiplication:

 Standard Matrix Multiplication


Basic mathematical approach

 2D Block Partitioning

Divides matrix into blocks for parallel execution

Reduces communication overhead

Suitable for distributed memory systems

 Cannon’s Algorithm

Most important algorithm in parallel matrix multiplication

Works using:

Initial alignment of blocks

Cyclic shifts (row & column)

Parallel multiply–accumulate

Ideal for mesh-connected processor

Odd–Even Transposition Sort

Parallel Prefix Sum (Scan)

Parallel Reduction (Sum, Min, Max)

Map–Reduce based tasks

 3 Code Implementations

Programs written in:

C++

MPI (Message Passing Interface)



Viva

End-semester exams

📂 Project Structure
Parallel_computing/
│── matrix_multiplication/
│   ├── standard/
│   ├── block_partition/
│   ├── cannon_algorithm/
│── notes/
│── parallel_algorithms/
│── assignments/
│── README.md

⚙️ How to Run (Example)
MPI Program:
mpicc program.c -o program
mpirun -np 4 ./program

OpenMP Program:
gcc -fopenmp file.c -o output
./output
