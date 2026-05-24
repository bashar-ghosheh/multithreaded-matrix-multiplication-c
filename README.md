# Multithreaded Matrix Multiplication

**Institution:** Birzeit University  
**Course:** ENCS3390 Operating Systems  
**Authors:** Bashar Ghosheh & Tareq Ahmad  

## 📌 Overview
This project implements and benchmarks shared-memory parallelization strategies for matrix multiplication using the native Windows Threading API (`windows.h`). The program evaluates execution time and computational speedup across various matrix sizes ($100 \times 100$, $500 \times 500$, and $1000 \times 1000$) and thread counts (2, 8, 16).

## 🚀 Parallelization Strategies
1. **Baseline (Sequential):** Standard $O(n^3)$ triple-nested loop implementation used as a reference for validation and speedup calculations.
2. **Row-Wise Parallelism:** Distributes matrix rows evenly across worker threads.
3. **Column-Wise Parallelism:** Distributes matrix columns evenly across worker threads.
4. **Block-Tiled Parallelism:** Divides the output matrix into fixed-size tiles ($64 \times 64$). Threads compute specific blocks, significantly optimizing CPU cache utilization and minimizing memory access overhead.

## 📊 Performance Highlights
Based on our testing (Intel Core i7-1165G7, 4 Cores / 8 Logical Processors):
* **Best Approach:** The **Block-Tiled** method consistently outperformed others for large matrices ($500^2$ and $1000^2$) due to enhanced cache locality.
* **Optimal Threads:** Maximum performance was generally achieved using **8 threads**, aligning with the hardware's logical processor count. Exceeding this (e.g., 16 threads) resulted in context-switching overhead and diminished returns.
* **Peak Speedup:** Achieved a **4.11x speedup** over the baseline when multiplying $1000 \times 1000$ matrices using the block-tiled approach on 8 threads.

## 🛠️ Compilation & Execution
This project requires a Windows environment and a C compiler (e.g., MSVC or MinGW).
