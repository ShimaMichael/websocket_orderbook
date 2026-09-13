
**Project Overview**
- **Purpose:** A high-performance, low-latency order book streamer and processor written in modern C++ (C++17). It receives market updates over a WebSocket, enqueues raw messages, and applies them to an in-memory order book for downstream use or benchmarking.
- **Primary goals:** correctness, predictable latency, testability, and a small, Makefile-driven build.

**Documentation**
- **Documentation** have been generated with doxygen which can be viewed in Latex and in web
To view documentation
run doxygen in CLI then:
Navigated to [docs/html/index.html](docs/html/index.html) to open in browser
Navigated to [docs/latex](docs/latex) to view tex file for 

**Considerations**
- I initially used an ordered map to build scaffold for the project and processing logic around storing orders
- Once the project was completed, I aimed to improve the implementaion and boost latency
- I swapped out my ordered map for a unordered map + heap to store info but that was not without a trade off of dealing with completed or cancelled order which might be in the middle of the heap causing the heap to possibly get bloated
- Another improvement I made was allowing for reading of websocket data to be completed in a seperate thread and stored in a shared message queue which was locked with a mutex during read or write which allowed for simultaneous read and process of market data ensuring orderbook does not fall behind during processing.


**Features**
- **WebSocket network I/O:** TLS-capable client in [src/network/websocket.cpp](src/network/websocket.cpp) for subscribing to market feeds.
- **Message queue:** Thread-safe `MessageQueue` implementation in [src/messagequeue](src/messagequeue) to decouple network reads from order processing.
- **Order book:** In-memory `OrderBook` in [src/orderbook](src/orderbook) with bid/ask levels, update logic, and best-level queries.
- **Utilities:** Helpers for JSON parsing, decimal->fixed conversions, and simple runner loops in [src/Utilities](src/Utilities).
- **Unit tests:** GoogleTest suite under `tests/` validating parsing, orderbook updates, and helpers.

**Requirements & Dependencies**
- **Compiler:** g++ or clang with C++17 support.
- **Libraries:** Boost (system, json), OpenSSL, pthreads.
- **GoogleTest:** Installed separately (Homebrew prefix commonly `/usr/local/opt/googletest`) for the `unit_tests` target.

**Build Instructions**
- **Build the main binary:**

```bash
make
./bin/streamer
```

- **Build & run unit tests:**

```bash
make unit_tests
./bin/unit_tests
```

- **Clean build artifacts:**

```bash
make clean
```

If you want parallel builds, use `make -j$(nproc)` (or `make -j` on macOS).

**Project Layout**
- **Root:** `main.cpp`, `makefile`, and top-level `README.md`.
- **Binary output:** `bin/streamer` and `bin/unit_tests`.
- **Sources:**
	- [src/network/websocket.cpp](src/network/websocket.cpp) / [src/network/websocket.hpp](src/network/websocket.hpp)
	- [src/messagequeue/messsagequeue.cpp](src/messagequeue/messsagequeue.cpp) / [src/messagequeue/messsagequeue.hpp](src/messagequeue/messsagequeue.hpp)
	- [src/orderbook/orderbook.cpp](src/orderbook/orderbook.cpp) / [src/orderbook/orderbook.hpp](src/orderbook/orderbook.hpp)
	- [src/Utilities/helper.cpp](src/Utilities/helper.cpp) / [src/Utilities/helper.hpp](src/Utilities/helper.hpp)
- **Tests:** `tests/` (GoogleTest-based runners).

**Runtime**
- The produced binary is `bin/streamer`. Launch it directly. It expects network endpoints/configuration inside the code although this has been set if any other values are not presentated for the sake of this assessment.

**Testing**
- Unit tests are implemented with GoogleTest. After `make unit_tests`, run `./bin/unit_tests` to execute the suite. The tests cover parsing helpers and core orderbook behavior.

**Performance & Best Practices (Possible future improvements)**
- **Reserve containers:** Pre-allocate containers used on hot paths (maps, vectors) to reduce reallocations.
- **Avoid copies:** Prefer `string_view` or operate on `std::string` references to avoid temporary string copies when parsing JSON or numeric strings.
- **Heap cleanup policy:** The current heap-based best-level implementation can accumulate stale entries; consider an occasional heap rebuild when `heap.size()` grows significantly beyond the active map size.
- **Precompute multipliers:** Convert decimal strings to fixed-point by precomputing multipliers for scale factors to avoid repeated pow/exp operations.
- **Consider data-structure alternatives:** For extreme churn, an indexed heap, or an ordered map with level aggregation may offer more predictable performance.

**Development Workflow**
- Edit sources under `src/` and add tests to `tests/`.
- Run `make` frequently and `./bin/unit_tests` after changes.

**Troubleshooting**
- If the build cannot find GoogleTest on macOS, install it via Homebrew:

```bash
brew install googletest
```

- If you encounter linker errors referencing `MessageQueue`, ensure the source file `src/messagequeue/messsagequeue.cpp` is compiled and linked by `make` (the provided Makefile already includes it).
