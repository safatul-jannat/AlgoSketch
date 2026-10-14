# AlgoSketch Compiler Completion Report

## 1. Repository Analysis

The repository at `D:\COMPILER PROJECT\AlgoSketch` contained a **mostly-complete but broken** compiler. All intended source modules were present and sized consistently, but the project could not compile, and when forced through it crashed and/or corrupted memory.

| Component | State on arrival |
|---|---|
| `src/lexer.l`, `src/parser.y` | Present, but parser.y did not build (see below) |
| `src/ast.h/.c`, `src/symbol_table.h/.c` | Present, correct |
| `src/semantic.c` | Present, but type inference was mathematically broken |
| `src/tac.c` | Present, but full of string-ownership bugs (heap corruption) |
| `src/codegen.c` | Present, but redeclared temporaries and could not emit correct `and`/`or` |
| `src/main.c` | Present, several defects (unused `--emit-c`, forward-slash run path) |
| `build.bat`, `Makefile` | Present, Makefile not Windows-clean |
| `tools/` (win_bison 3.8.2, win_flex 2.6.4) | Present and working |
| `tests/` | Contained only an empty `test01_display.algo` |
| `generated/` | Empty |

**Concrete problems found:**

1. **Corrupted parser grammar (blocking).** `parser.y` was rewritten by an earlier regex script that turned every `$$` into `$`, so bison rejected it (`'$' undeclared`, `stray '$'` warnings).
2. **Lexer token collision (blocking).** win_flex emits `#define BEGIN (yy_start) = ...` before the user prologue, which clashed with the `BEGIN` keyword token.
3. **Bison header placement.** `%defines "parser.h"` wrote the header into the CWD instead of `src/`, so the MINGW system `parser.h` was included instead of the generated one.
4. **AST operator storage.** Operators were stored in `node->value` (an `AstNode*`), not in a `char *op` field, so every binary/unary expression used mismatched types.
5. **String ownership in TAC (runtime heap corruption).** Every temporary (`t1`, ...) and label (`L1`, ...) string was referenced by *two* instructions (producer `result`/`label` + consumer `arg1`/`label`), so `free_tac_inst` double-freed them → `STATUS_HEAP_CORRUPTION` / stack-buffer-overrun crashes on almost every run.
6. **Type inference returned the wrong value.** `type_of_operand()` (and the `==`/`!=` path and `infer_call()`) used the `infer_expr()` *change flag* (0/1) as if it were the operand *type*, so literals and sub-expressions stayed `UNKNOWN` and valid programs were rejected.
7. **Duplicate declarations were silently accepted.** `var x = 10; var x = 20;` compiled.
8. **`--run` could not execute.** The exe path was emitted with forward slashes (`generated/x.exe`), which `cmd.exe` cannot run.
9. **Codegen redeclared temporaries.** `a and b`/`a or b` emitted two `bool tN = ...;` declarations for the same temp → C compile error.
10. **String literals included their quotes.** The lexer captured `"..."` verbatim, so text printed as `"hello"`.
11. **Makefile wasn't Windows-clean.** `CC ?=` never overrides GNU make's built-in `cc`, and `clean` used the Unix `rm`.

## 2. Changes Made

**`src/parser.y`**
- Restored every corrupted `$$` (bison action symbols).
- Switched `%defines "parser.h"` to bare `%defines` so `parser.h` is generated next to the `-o` output in `src/`.
- All operator assignments now write `$$->op = (char *)"..."` (14 sites) instead of `$$->value`.

**`src/lexer.l`**
- `#undef BEGIN` before `#include "parser.h"` to defeat the flex `BEGIN` macro.
- String literals are now unquoted and unescaped (`\"`, `\\`, `\n`, `\t`) via `unquote_string()`, so `display "hello"` prints `hello`.
- String regex widened to `"([^"\\\n]|\\.)*"` so escaped quotes work.

**`src/ast.h`**
- Added `char *op;` field for binary/unary operator strings.

**`src/ast.c`**
- `ast_print` prints `op` for binary/unary expressions.

**`src/semantic.c`**
- `type_of_operand()` now returns the operand's actual `inferred_type` (running `infer_expr` for propagation rather than returning its flag).
- `==`/`!=` and `infer_call()` read `inferred_type` instead of the change flag.
- Added duplicate-declaration rejection: duplicate functions (phase 1), duplicate parameters within a function, duplicate program variables / function-name clashes.
- Fixed-point `MAX_ITER 20` inference retained; post-inference annotation copies resolved function return/param types onto AST nodes (kept — TAC/codegen depend on it).

**`src/tac.c`**
- Adopted a strict single-owner string model: every instruction owns every field it stores; `strdup()` at every consumer site; caller always frees the temp/label returned by `gen_expr`/`new_label`.
- `TAC_BINARY` uses a dedicated `TacInst.binop` field (left in `arg1`, right in `arg2`).
- and/or short-circuit control flow: labels/temps are `strdup`-ed per instruction and the originals freed once.
- Removed dead code: `tac_var_type`, unused locals, symbol-table includes (`semantic.h`) — TAC is fully AST-driven.
- `free_tac_inst`/`free_tac_list`/`free_tac_fn` now free each allocation exactly once.

**`src/codegen.c`**
- `TAC_CONSTANT` reuses the temp's declared type instead of re-declaring it (fixes `and`/`or` redeclaration).
- Text comparison (`==`/`!=`) emits `strcmp(a, b) == 0`.
- Removed unused `local_type`.

**`src/main.c`**
- Removed the `--emit-c` option and unused variable.
- Output and executable paths use backslashes (`generated\X.exe`) so `cmd.exe` can run them.

**`build.bat`**
- Runs `win_bison`/`win_flex` from inside `src\` (`pushd`) so generated files (including `parser.h`) land in `src\`.

**`Makefile`**
- `CC = gcc` (a plain assignment overrides make's built-in `cc`; still overridable on the command line).
- Clean target uses `del /Q` on Windows and `rm -f` on Linux via an `OS` check.
- Usage: `make BISON=tools/win_bison.exe FLEX=tools/win_flex.exe`.

## 3. Compiler Pipeline Status

```
.algo source
   ├─ src/lexer.l  ──(win_flex)──► tokens
   ├─ src/parser.y ──(win_bison)──► AST (src/ast.h/.c)
   ├─ src/semantic.c ──► symbol tables, fixed-point type inference,
   │        validation, recursion check (no recursion/arrays/closures supported)
   ├─ src/tac.c    ──► three-address code (short-circuit and/or, labels, calls)
   ├─ src/codegen.c ──► C source (includes stdio/stdbool/string.h,
   │        prototypes, function defs, main; strcmp for text equality;
   │        true/false prints)
   └─ gcc (C:\mingw64\mingw64\bin) ──► generated\X.exe
```

- The compiler binary `algosketch.exe` builds from a clean tree with `build.bat` and with `make`.
- Compiles with `gcc -Wall` and no warnings.
- Type mapping: `number → double`, `text → const char *`, `boolean → bool`.
- `display` output: number `%g`, text `%s`, boolean `true`/`false`.

## 4. Testing Results

`algosketch <file> [--run]`; expected outputs below are the generated program's stdout.

**Positive tests — all pass (9/9):**

| Test | Program | Output |
|---|---|---|
| test01_basic | `var x = 10; display x;` | `10` |
| test02_expression | `var x = 2 + 3 * 4; display x;` | `14` |
| test03_if | `if/otherwise` with comparisons | `15` |
| test04_while | counter loop `while x < 5` | `5` |
| test05_function | `Add(10,20)`, `Multiply(3,4)` | `30`, `12` |
| test06_text | string variables and string literals | `Hello World`, `Direct string`, `42` |
| test07_boolean | `true and false`, `5 > 3`, `a or b` | `false, true, false, true, true` |
| test08_canonical | full combined example | `The sum is: 30 Sum is large 3 Not done yet` |
| test09_text_compare | text `==`/`!=` | `equal`, `different` |

**Rejection tests — all pass (6/6) (nonzero exit, error message, no .exe produced):**

| Test | Source | Error |
|---|---|---|
| err01_undeclared | `display x;` | `use of undefined variable 'x'` + `cannot display value of undetermined type` |
| err02_duplicate | `var x = 10; var x = 20;` | `duplicate declaration of 'x'` |
| err03_type_mismatch | `var x = 10; x = "hello";` | `cannot assign text to number variable 'x'` |
| err04_invalid_condition | `if 10 then ...` | `if condition must be boolean, got number` |
| err05_bad_args | `Add(10)` (Add expects 2) | `function 'Add' expects 2 argument(s), got 1` |
| err06_recursion | `function A() ... return A();` | `recursion is not supported: function call cycle detected` |

Flags verified: `--ast` (pretty-printed AST), `--tac` (three-address listing), `--run`, `-o`, `--cc`, `--cflags`. Since the double-free fixes, no crash occurs on any success or error path.

## 5. Remaining Issues

- **Type constraints are explicit, not matrix-unified.** `==` between different types is rejected only when both sides are known; unknown-vs-known kinds (e.g., a variable first used in a comparison with a number) resolve to `number` because comparison operands are constrained to `TYPE_NUMBER`. This matches the spec's rules (no mixed-type expressions).
- **No runtime bounds/`NULL` checking** on allocator failures (production hardening only; all current paths are covered).
- **Language gaps are intentional per spec**: no recursion/arrays/pointers/classes/void/input/`for`/`break`/`continue`/overloading — all rejected or unparseable by design.
- **`tools/` bundling.** The bison `data\` skeletons must live next to `win_bison.exe`; they are already shipped that way. If `tools/` is ever re-downloaded, keep `win_flex.exe` + `win_bison.exe` + `data\` together.
- **A single valid empty program** (`<empty source>` with only whitespace) produces an empty `int main` — acceptable.

## 6. How To Run

Prerequisites: Windows, the bundled `tools\win_bison.exe`/`tools\win_flex.exe`, and any MinGW-w64 GCC on `PATH` (the machine has `C:\mingw64\mingw64\bin\gcc.exe`).

**Build the compiler:**

```
build.bat                (Windows; uses tools\win_bison.exe / win_flex.exe)
# or
make BISON=tools/win_bison.exe FLEX=tools/win_flex.exe   (if make is installed)
```

**Compile and run an AlgoSketch program:**

```
algosketch.exe tests\test04_while.algo --ast   # dump AST
algosketch.exe tests\test04_while.algo --tac   # dump three-address code
algosketch.exe tests\test04_while.algo --run   # compile C, link with gcc, run exe
algosketch.exe myprog.algo -o myprog --run     # custom output base name
```

Artifacts: `generated\<base>.c` (the C output) and `generated\<base>.exe`.

**Try the full suite:**

```
algosketch.exe tests\test08_canonical.algo --run
algosketch.exe tests\err06_recursion.algo    # expect: recursion is not supported
```

Exit code `0` = success; non-zero = lexical/parse/semantic error (messages go to stderr with source line numbers).