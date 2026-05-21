This file highlights how I used AI while developing this project: areas that were AI-assisted,
example prompts, and moments where I guided the model toward a specific direction.

- AI served greatly as a glorified google search for me thorugh out this project, identifying libraries that are fit for purpose and how they are best utilized, language nuances or specific syntax I might need.

- Once refactoring was completed to make the project modular, I asked Copilot to add and edit
  Doxygen comments. The prompt below produced comments in both `.cpp` and `.hpp` files; I
  preferred them only in the header files, so I steered the model until the results matched
  my expectations.

```text
I have implemented an order book that connects to a
websocket client and updates the order book when an
update arrives. I need you to add Doxygen-style comments
for my classes and functions using this format:
/**
 * Class/function description and purpose
 * @param ...
 * @return ...
 */
```

- After completing the implementation I asked the model to review the code for potential
  performance regressions — e.g., operations that could slow down updates and cause the
  order book to lag. The model suggested using `try_emplace` to avoid a double lookup in
  `update_order()`, which reduces work on hot paths:

```c++
// improvement
auto [it, inserted] = order_side.try_emplace(price, quantity);
if (inserted) {
    side_heap.push(price);
} else {
    it->second = quantity;
}
```

rather than:

```c++
auto it = order_side.find(price);
if (it == order_side.end()) {
    side_heap.push(price);
    order_side.try_emplace(price, quantity);
} else {
    it->second = quantity;
}
```

Example prompt used:

```text
Can you review my order book implementation and identify any areas
where I might be compromising latency?
```

Additional, sensible prompts

```text
Scan the codebase and suggest small, low-risk micro-optimizations that reduce
latency or allocations (e.g., container reserves, avoiding string copies, precomputed
multipliers). For each suggestion, explain the change, why it helps, and show a
minimal code patch.
```

- Generate unit tests for critical modules:

```text
Generate GoogleTest unit tests covering the `OrderBook` update and query paths,
edge cases for `parse_string_to_int64`, and concurrency tests for `MessageQueue`.
Place tests under `tests/` and make them runnable with `make unit_tests`.
```

- Suggest an orderbook data-structure alternative for high churn:

```text
Based on the current `OrderBook` implementation (unordered_map + heap), recommend
one or two alternative data structures (e.g., indexed heap, ordered map with
aggregation). For each option, explain trade-offs, performance characteristics,
and a migration plan (small step-by-step changes and tests required).
```