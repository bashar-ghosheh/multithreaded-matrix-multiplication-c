# Multithreaded Matrix Multiplication

> **Platform: Windows only.** This project uses the Windows API through `windows.h` (`CreateThread`, `WaitForMultipleObjects`, and `QueryPerformanceCounter`). Building on Linux or macOS requires porting the threading and timing code.

A C benchmark of four shared-memory matrix multiplication approaches: a sequential baseline, row-wise threading, column-wise threading, and 64 × 64 block tiling.

**Institution:** Birzeit University  
**Course:** ENCS3390 Operating Systems  
**Authors:** Bashar Ghosheh and Tareq Ahmad

## Results

Reported measurements were collected on an Intel Core i7-1165G7 (4 cores, 8 logical processors). The project summary reports these comparative results:

| Test | Result |
| --- | --- |
| 1000 × 1000, block-tiled, 8 threads | 4.11× speedup over the sequential baseline |
| 500 × 500 and 1000 × 1000 | Block-tiled was the fastest strategy in the reported runs |
| Thread-count comparison | 8 threads generally performed best; 16 threads added context-switching overhead |

The report summary does not list raw elapsed times for every configuration. The current `main.c` defaults to one 1000 × 1000 run with 16 threads; the table reflects the broader comparisons reported for the project.

## Algorithms

- **Sequential baseline:** triple nested loops, used as the reference result.
- **Row-wise:** each worker thread computes a range of output rows.
- **Column-wise:** each worker thread computes a range of output columns.
- **Block-tiled:** workers compute 64 × 64 output tiles to improve cache locality.

Each parallel result is checked against the sequential reference with a floating-point tolerance of `1e-6`.

## Build and run

Use one of the Windows compiler options below. Run the commands from the repository root.

### MSVC

Open an **x64 Native Tools Command Prompt for Visual Studio**, then run:

```bat
cl /O2 /W4 main.c /Fe:matrix-benchmark.exe
matrix-benchmark.exe
```

### MinGW-w64

From PowerShell with MinGW-w64 on `PATH`:

```powershell
gcc -O2 -Wall -Wextra main.c -o matrix-benchmark.exe -lm
.\matrix-benchmark.exe
```

The program prints the sequential and parallel elapsed times and reports whether each output matches the baseline.

## Project files

- `main.c` — matrix generation, baseline, threaded strategies, timing, and result verification.
- [`docs/project1.pdf`](docs/project1.pdf) — project report.
