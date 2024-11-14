# Truss Structure Optimizer (Got 30 out of 30 points)

## Overview

The **Truss Structure Optimizer** is a system designed to construct rigid and lightweight truss structures optimized for polygonal shapes, which may be non-convex. By adding additional beams (without crossing) to divide the polygon into disjoint triangles, the optimizer ensures maximum rigidity while minimizing the total length of the beams. The project addresses two primary problems:

1. **Minimum Triangulation**: Finding a triangulation configuration with the smallest possible total length.
2. **Triangulation Count**: Calculating the total number of possible triangulations for a given polygon.

To efficiently solve these problems, the optimizer leverages multithreading, allowing concurrent processing of multiple problem packs from various companies.

## Features

- **Minimum Triangulation**: Compute the minimal total length of triangulation beams for polygonal structures.
- **Triangulation Counting**: Determine the total number of valid triangulations for complex polygons.
- **Multithreaded Processing**: Utilize multiple worker threads to handle computationally intensive tasks efficiently.
- **Order Preservation**: Ensure that solved problem packs are returned to companies in the order they were received.
- **Big Integer Support**: Handle large numbers of triangulations using custom 1024-bit wide integers (`CBigInt`).

## Key Classes

### `COptimizer`
- **Description**: Central class managing the optimization process, including thread management and communication with companies.
- **Responsibilities**:
  - Register and manage multiple `CCompany` instances.
  - Start and stop worker and communication threads.
  - Coordinate the distribution and collection of problem packs.

### `CCompany`
- **Description**: Represents a construction company utilizing the optimizer's services.
- **Responsibilities**:
  - Provide problem packs (`CProblemPack`) to be solved via `waitForPack`.
  - Receive solved problem packs through `solvedPack`.
  - Each company instance has two dedicated communication threads to handle sending and receiving problem packs.

### `CProblemPack`
- **Description**: Encapsulates a group of polygon problems to be solved.
- **Responsibilities**:
  - Contains two lists:
    - `m_ProblemsMin`: Polygons requiring minimum triangulation computation.
    - `m_ProblemsCnt`: Polygons requiring triangulation count computation.
  - Methods to add problems to each list (`addMin`, `addCnt`).

### `CPolygon`
- **Description**: Represents a single polygon problem.
- **Responsibilities**:
  - Stores polygon vertices (`m_Points`).
  - Holds results:
    - `m_TriangMin`: Length of minimal triangulation.
    - `m_TriangCnt`: Total number of possible triangulations.

### `CBigInt`
- **Description**: Implements big positive integers with up to 1024 bits.
- **Responsibilities**:
  - Support for setting values from `uint64_t` or decimal strings.
  - Conversion to decimal string representation.
  - Basic arithmetic operations: addition, multiplication, and comparison.

### `CProgtestSolver`
- **Description**: Provides a sequential solver for triangulation problems.
- **Responsibilities**:
  - Solve batches of triangulation problems.
  - Manage solver capacity and ensure single-use per instance.
  - Factory functions to create solvers for minimal triangulation (`createProgtestMinSolver`) and triangulation counting (`createProgtestCntSolver`).

## Implementation Guidelines

### Multithreading and Synchronization
- **Communication Threads**: 
  - Each `CCompany` instance has two dedicated communication threads:
    - **Receiver Thread**: Continuously calls `waitForPack` to receive new problem packs and dispatches them to worker threads.
    - **Sender Thread**: Sends solved problem packs back to the company via `solvedPack`, maintaining the original order.
- **Worker Threads**:
  - Execute computationally intensive tasks, processing problem packs to compute minimal triangulation lengths and triangulation counts.
  - The number of worker threads is configurable based on hardware capabilities.

### Thread Management
- **Initialization**:
  - `COptimizer::start` initializes worker threads and communication threads.
- **Termination**:
  - `COptimizer::stop` ensures all problem packs are processed and gracefully terminates all threads.
- **Order Preservation**:
  - Communication threads must maintain the order of problem packs when returning results to companies.

### Solver Integration
- **Using `CProgtestSolver`**:
  - Utilize the provided sequential solver for algorithmic computations.
  - Ensure full utilization of solver capacities to avoid exceeding limits.
- **Custom Solver (Optional)**:
  - Implement your own solver if `usingProgtestSolver()` returns `false`.
  - Ensure the custom solver adheres to the required time and space complexities.

## Testing

The optimizer is evaluated through various tests to ensure correctness, performance, and reliability:

1. **Algorithm Test**:
   - Validates the correctness of triangulation algorithms.
   - Ensures `checkAlgorithmMin` and `checkAlgorithmCnt` produce accurate results.

2. **Basic Test**:
   - Tests the creation and management of multiple companies and worker threads.
   - Ensures proper communication and problem solving without deadlocks.

3. **Advanced Basic Test (W=n, C=m)**:
   - Introduces more stringent conditions by halting problem delivery mid-test.
   - Verifies that the optimizer can handle interruptions and continue processing correctly.

4. **Speedup Test**:
   - Measures the performance improvement when increasing the number of worker threads.
   - Ensures that wall time decreases proportionally with more threads.

5. **Busy Waiting Tests**:
   - Checks for inefficient thread synchronization that could lead to excessive CPU usage.
   - Includes scenarios with delayed `waitForPack` and `solvedPack` calls.

6. **Load Balance Tests (#1, #2, #3)**:
   - Assesses the optimizer's ability to distribute workload evenly across worker threads.
   - Ensures that single large problem instances are efficiently processed using all available threads.

## Submission Instructions

- **Source Code**:
  - Implement the `COptimizer` class and any auxiliary classes in `solution.cpp`.
  - Do **not** include `main` or `#include` directives unless wrapped in conditional compile directives (`#ifdef`, `#ifndef`, `#endif`).

- **File Structure**:
  - Preferably organize code into multiple `.cpp` and `.h` files for maintainability.
  - If required to submit as a single file, consolidate source files while excluding `#include` directives.

- **Compiler Compatibility**:
  - Use C++11 thread APIs or `pthread` for multithreading.
  - Ensure compatibility with `g++` version 12.2, supporting C++11 to C++20 constructs.

- **Version Control**:
  - Utilize Git for versioning your code.
  - Regularly commit changes with clear messages.
  - Consider using the faculty GitLab server for archiving and collaboration.

- **Submission Format**:
  - Follow provided guidelines to submit your code, ensuring all necessary classes and methods are included and correctly implemented.

## Hints and Best Practices

- **Start with Threading**:
  - Implement communication and worker threads along with synchronization mechanisms early.
  - Ensure threads can handle problem packs concurrently without causing deadlocks.

- **Efficient Synchronization**:
  - Use mutexes and condition variables appropriately to manage shared resources.
  - Avoid busy waiting to prevent unnecessary CPU usage.

- **Solver Utilization**:
  - Fully utilize the capacities of `CProgtestSolver` instances by batching problem additions.
  - Avoid one-by-one problem solving to prevent exhausting solver capacities.

- **Resource Management**:
  - Avoid global variables; initialize all necessary data within class constructors or methods.
  - Ensure all dynamically allocated resources are properly freed to prevent memory leaks.

- **Performance Optimization**:
  - Leverage multithreading to maximize CPU core utilization.
  - Optimize algorithms to meet the required time and space complexities.

- **Testing and Validation**:
  - Regularly test your implementation against sample data.
  - Ensure that all problem packs are processed and returned correctly and in order.

## Additional Notes

- **Problem Constraints**:
  - Polygons are simple (no intersecting edges or holes).
  - Coordinate values are integers, facilitating precise geometric computations.

- **Algorithmic Considerations**:
  - Minimal triangulation can be approached using dynamic programming.
  - Triangulation counting leverages combinatorial mathematics, such as Catalan numbers for convex polygons.

- **Bonus Challenges**:
  - Implement a custom multithreaded solver to further optimize performance.
  - Ensure that your solver adheres to the required computational complexities and efficiently manages large problem instances.

---

Happy Optimizing! 🚀
