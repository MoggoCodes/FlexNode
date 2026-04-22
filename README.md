# FlexNode

FlexNode is a small C++ value container that can hold exactly one value at a time from a fixed set of types:

- `INT`
- `DOUBLE`
- `CHAR`
- `STRING`
- `BOOL`
- `UNINITIALIZED` (empty state)

The project includes:

- A static library target: `flexnode_lib`
- An interactive terminal UI executable: `flexnode`
- A test executable target scaffold: `flexnode_tests`

## How FlexNode Works

`FlexNode` starts uninitialized and can be initialized once with one typed `add*` method:

- `addInt`
- `addDouble`
- `addChar`
- `addString`
- `addBool`

After initialization:

- Calling another `add*` returns `false` (node already contains data).
- You can update the stored value only with the matching `change*` method.
  - Example: if the node stores `DOUBLE`, only `changeDouble(...)` will succeed.
- `clear()` resets the node back to `UNINITIALIZED`.

Metadata access:

- `isEmpty()`
- `getType()`

Additional behaviors:

- Copy constructor and assignment operator perform deep copy semantics.
- `operator<<` prints the current value.

## Output Formatting

Printed values follow these rules:

- `DOUBLE` always shows at least one decimal place (`1` prints as `1.0`)
- `CHAR` prints with single quotes (`'x'`)
- `STRING` prints with double quotes (`"hello"`)

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

## Run the TUI

```bash
./build/flexnode
```

On each loop, the TUI always prints the current node state first, then shows a dynamic menu.

### Menu When Node Is Empty

- `1. Initialize node`
- `0. Exit`

### Menu When Node Is Initialized

- `1. Modify node` (same type only)
- `2. Clear node`
- `0. Exit`

## TUI Input Notes

- `INT`: expects a valid integer
- `DOUBLE`: expects a valid floating-point number
- `CHAR`: must be exactly one character
- `STRING`: full line input accepted
- `BOOL`: accepts `true/false`, `t/f`, `yes/no`, `y/n`, `1/0`

Invalid input prompts again for that field.

## API Surface (Quick Reference)

Core declarations are in `src/FlexNode.h`, implementation in `src/FlexNode.cpp`.

Typical usage:

```cpp
FlexNode node;
node.addDouble(3.5);     // true
node.changeDouble(7.0);  // true
node.clear();            // true
```
