# AlgoSketch

**AlgoSketch** is a small educational compiler for a simple procedural language.
It lexes and parses `.algo` source, performs semantic analysis and type
inference, lowers the program to three-address code (TAC), and emits C source
that is then compiled to a native executable with GCC.

The compiler is implemented in C using **Flex** (lexer) and **Bison** (parser).

## Features

- **Types:** `number` (C `double`), `text` (C `const char *`), `boolean` (C `bool`)
- **Type inference** for variables from their initializer / first assignment
- **Functions** with parameters and a `return` value
- **Control flow:** `if / then / otherwise / end`, `while ... end`
- **Operators:** `+ - * /`, `< > <= >= == !=`, `and or not`
- **Output** via `display` (`%g` for numbers, `%s` for text, `true`/`false` for booleans)
- **Text comparison** compiled to `strcmp`, string literals unquoted/unescaped
- Clear semantic errors with source line numbers, and safety checks for
  duplicate declarations, type mismatches, bad argument counts, and recursion

The language is intentionally kept minimal: recursion, arrays, pointers,
classes, user input, `for`, `break`, `continue`, and overloading are not
supported.

## Example

```algo
function Add(a,b)
begin
    return a+b;
end

var x = 10;
var y = 20;
var sum = Add(x,y);

if sum > 25 then
    display "Sum is large";
otherwise
    display "Sum is small";
end

display "The sum is:";
display sum;
```

## Building

Prerequisites:

- Windows (the bundled toolchain targets Windows) or Linux
- GCC (MinGW-w64 on Windows) on `PATH`
- Bundled `tools\win_flex.exe` and `tools\win_bison.exe` (Windows), or system
  `flex` / `bison` (Linux)

Build the compiler on Windows:

```
build.bat
```

Or with Make:

```
make BISON=tools/win_bison.exe FLEX=tools/win_flex.exe   # Windows
make                                                      # Linux
```

This produces `algosketch.exe` (or `algosketch`).

## Usage

```
algosketch <source.algo> [options]
```

| Option | Description |
|---|---|
| `--run` | Compile the generated C and run the executable (default) |
| `--ast` | Print the parsed AST (do not run) |
| `--tac` | Print the generated three-address code (do not run) |
| `-o <base>` | Output base name (default: source file name) |
| `--cc <command>` | C compiler command (default: `gcc`) |
| `--cflags <flags>` | Extra flags passed to the C compiler |

Examples:

```
algosketch.exe tests\test04_while.algo --ast
algosketch.exe tests\test04_while.algo --tac
algosketch.exe tests\test08_canonical.algo --run
algosketch.exe myprog.algo -o myprog --run
```

Generated artifacts are written to `generated\`: `generated\<base>.c` and
`generated\<base>.exe`.

Exit code `0` means success; a non-zero exit indicates a lexical, parse, or
semantic error (reported on stderr).

## Project Layout

```
src/
  lexer.l        Flex lexer specification
  parser.y       Bison grammar specification
  ast.h/.c       AST node definitions and pretty-printing
  semantic.c     Symbol tables, type inference, validation
  tac.c          Three-address code generation
  codegen.c      C code generator
  symbol_table.c Scoped symbol table
  error.c        Error reporting
  main.c         CLI driver and compilation pipeline
tests/           Positive (.algo programs) and error-case tests
tools/           Bundled win_flex / win_bison for Windows
generated/       Generated C sources and executables (build output)
build.bat        Windows build script
Makefile         Cross-platform make build
REPORT.md        Detailed implementation and completion report
```

## Pipeline

```
.algo source
  -> lexer.l      (win_flex)      -> tokens
  -> parser.y     (win_bison)     -> AST
  -> semantic.c                   -> symbol tables, type inference, validation
  -> tac.c                        -> three-address code
  -> codegen.c                    -> C source
  -> gcc                          -> native executable
```

## Tests

Two kinds of tests are provided in `tests/`:

- **Positive tests** (`test01`–`test13`): valid programs covering expressions,
  `if`, `while`, functions, text, booleans, scoping, and type inference.
- **Error tests** (`err01`–`err09`): programs that must be rejected with a clear
  message, e.g. undeclared variables, duplicate declarations, type mismatches,
  invalid conditions, wrong argument counts, and recursion.

```
algosketch.exe tests\test08_canonical.algo --run
algosketch.exe tests\err06_recursion.algo
```
