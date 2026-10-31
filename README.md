# Lexical Analyzer in C 🔍💻

A complete, dependency-free lexical analyzer (tokenizer) for C source
files, written in pure C --- no Flex, no Lex, no external libraries. 🚀

It tokenizes input, prints a formatted token table, summarizes token
counts per type, and exports everything to an Excel-compatible CSV sheet
(`Output.xls`). 📊📁

------------------------------------------------------------------------

## 📑 Table of Contents

1.  [✨ Features](#-features)
2.  [🗂️ Project Structure](#️-project-structure)
3.  [🔤 Token Types](#-token-types)
4.  [🛠️ Build & Run](#️-build--run)
5.  [📖 Usage](#-usage)
6.  [📋 Menu](#-menu)
7.  [🖥️ Sample Output](#️-sample-output)
8.  [🚦 Exit Codes](#-exit-codes)
9.  [⚙️ How It Works](#️-how-it-works)
10. [⚠️ Known Limitations](#️-known-limitations)
11. [👨‍💻 Author](#-author)
12. [📜 License](#-license)

------------------------------------------------------------------------

## ✨ Features

-   ✅ Pure C implementation --- no Flex, no Lex, no external libraries
-   ✅ Tokenizes keywords, identifiers, integers, floats, strings,
    chars, operators, punctuation, unknowns, and errors
-   ✅ Tracks line and column for every token 📍
-   ✅ Verbose mode (`-v`) adds a numeric `VALUE` column for `NUMBER` /
    `FLOAT` tokens
-   ✅ Interactive menu for analysis, summary, and sheet export 🎛️
-   ✅ Exports a properly quoted CSV / Excel sheet (`Output.xls`) that
    opens cleanly in Excel / LibreOffice / Google Sheets 📈
-   ✅ Handles `//` and `/* ... */` comments 💬
-   ✅ Reports lexical errors (unterminated strings/chars, bad escapes)
    and returns a non-zero exit code when errors are found ❌
-   ✅ Escaped quotes inside string literals are handled correctly in
    CSV output 🎯

------------------------------------------------------------------------

## 🗂️ Project Structure

The project consists of the following source files: `main.c` handles
argument parsing, the menu loop, and console output; `token.c` contains
the tokenizer core with `scan_alpha`, `scan_number`, `scan_string`, and
`scan_char` functions; `type.c` holds the keyword table and token type
name lookup; `creat_xls.c` provides the CSV writer functions `xl_header`
and `xl_content`; `main.h` defines the structs, enums, and function
prototypes; and `sample.c` is an optional example input file for
testing.

-   📄 **`main.c`** --- Entry point, argument parsing, menu, console
    table, summary
-   📄 **`token.c`** --- Scanner: character-by-character tokenization
    and helper scanners
-   📄 **`type.c`** --- Keyword list, `is_keyword()`,
    `token_type_name()`, lexer init
-   📄 **`creat_xls.c`** --- CSV output functions with proper
    quoting/escaping
-   📄 **`main.h`** --- `Token`, `Lexer`, `TokenType`, and all function
    prototypes

------------------------------------------------------------------------

## 🔤 Token Types

  🔹 Enum               Display Name    Examples
  --------------------- --------------- ------------------------------
  `TOKEN_EOF`           `EOF`           `<EOF>`
  `TOKEN_KEYWORD`       `KEYWORD`       `int`, `return`, `struct`
  `TOKEN_IDENTIFIER`    `IDENTIFIER`    `main`, `x`, `MAX_LEXEME`
  `TOKEN_NUMBER`        `NUMBER`        `42`, `1024`
  `TOKEN_FLOAT`         `FLOAT`         `3.14e2`, `2.5`
  `TOKEN_STRING`        `STRING`        `"hello"`
  `TOKEN_CHAR`          `CHAR`          `'a'`, `'\n'`
  `TOKEN_OPERATOR`      `OPERATOR`      `+`, `==`, `<<`, `->`
  `TOKEN_PUNCTUATION`   `PUNCTUATION`   `(`, `)`, `{`, `}`, `;`, `,`
  `TOKEN_UNKNOWN`       `UNKNOWN`       `#`, `.`, `@`
  `TOKEN_ERROR`         `ERROR`         unterminated string/char

------------------------------------------------------------------------

## 🛠️ Build & Run

### 🔧 Compile

``` bash
gcc main.c token.c type.c creat_xls.c -o lexer
```

### ▶️ Run (normal mode)

``` bash
./lexer sample.c
```

### ▶️ Run (verbose mode --- adds numeric VALUE column)

``` bash
./lexer sample.c -v
```

> 💡 On Windows, compile with any C compiler (e.g. `gcc`, MinGW, MSVC)
> and run `lexer.exe sample.c`.

------------------------------------------------------------------------

## 📖 Usage

``` text
./lexer <source_file> [-v]

<source_file>   📂 Path to the C source file to analyze
-v              📊 (optional) Verbose mode: adds numeric VALUE for NUMBER / FLOAT
```

> ℹ️ The program reads the file, tokenizes it, prints a table, and
> writes `Output.xls` in the current directory.

------------------------------------------------------------------------

## 📋 Menu

After launch, an interactive menu appears:

``` text
🧭 MENU :-
-----------
1. DO LEXICAL ANALYZE
2. GET SUMMARY
3. GET XL Sheet
4. EXIT
```

### Option descriptions

-   🟢 **1** --- Tokenize the file, print the token table, write
    `Output.xls`
-   🟡 **2** --- Show per-type counts and total errors (requires option
    1 first)
-   🔵 **3** --- Confirm the generated Excel sheet name
-   🔴 **4** --- Exit (asks for `Y/N` confirmation)

------------------------------------------------------------------------

## 🖥️ Sample Output

### 💻 Console (verbose)

``` text
---------------------------------------------------------------------------
| ROW  |COLUMN|TYPE           |LEXICAL                              |VALUE    |
---------------------------------------------------------------------------
|1    |2     |KEYWORD        |int                                  |         |
---------------------------------------------------------------------------
|1    |6     |IDENTIFIER     |main                                 |         |
---------------------------------------------------------------------------
|2    |18    |NUMBER         |42                                   |42       |
---------------------------------------------------------------------------
|3    |18    |FLOAT          |3.14e2                               |314      |
---------------------------------------------------------------------------
|5    |1     |EOF            |<EOF>                                |         |
---------------------------------------------------------------------------
```

### 📊 Summary (menu option 2)

``` text
Summary:
-------------
KEYWORDS     : 5
IDENTIFIERS  : 3
INTEGERS     : 2
FLOATS       : 1
STRINGS      : 0
CHARS        : 1
OPERATORS    : 4
PUNCTUATION  : 7
UNKNOWN      : 0
ERRORS       : 0
```

### 📁 Output.xls (CSV format)

``` csv
"ROW","COLUMN","TYPE","LEXICAL"
1,2,"KEYWORD","int"
1,6,"IDENTIFIER","main"
2,18,"NUMBER","42"
2,18,"FLOAT","3.14e2"
3,1,"EOF","<EOF>"
```

------------------------------------------------------------------------

## 🚦 Exit Codes

  Code     Meaning
  -------- ----------------------------------------------
  ✅ `0`   Clean run (no lexical errors)
  ⚠️ `1`   Bad arguments (file missing / invalid count)
  ❌ `2`   Lexical errors were found during analysis

------------------------------------------------------------------------

## ⚙️ How It Works

1.  **`main.c`** parses arguments, opens the source file and
    `Output.xls`, then enters the menu loop.
2.  On **"DO LEXICAL ANALYZE"**, it rewinds the input, resets the lexer
    state, writes the CSV header, and repeatedly calls `tokenizer()`
    until `TOKEN_EOF`.
3.  **`token.c`** holds the scanning logic --- it skips whitespace and
    comments (`//`, `/* ... */`), dispatches to `scan_alpha`,
    `scan_number`, `scan_string`, `scan_char`, or handles
    operators/punctuation inline, and each scanner builds a lexeme
    buffer and returns a `Token`.
4.  **`type.c`** provides the keyword table, `is_keyword()`, and
    `token_type_name()`.
5.  **`creat_xls.c`** writes CSV rows, quoting lexemes and escaping
    embedded double quotes (`"` becomes `""`).
6.  Counts are accumulated per `TokenType` and displayed via the **GET
    SUMMARY** menu option.

------------------------------------------------------------------------

## ⚠️ Known Limitations

-   ⚠️ **No preprocessor awareness** --- `#include`, `#define` etc. are
    tokenized as `UNKNOWN` (`#`) followed by identifiers/operators. This
    is by design (it's a lexical analyzer, not a preprocessor).
-   ⚠️ **`lx_next()` / `lx_get()` lookahead quirk** --- the
    two-character operator and comment detection relies on peeking; this
    works for common cases but the lookahead buffer is single-slot, so
    exotic inputs may mis-tokenize around comments.
-   ⚠️ **Numbers like `1....1`** --- the scanner consumes the first `.`
    as the start of a float, leaving `...1` to be scanned separately;
    this is expected lexer behavior, not a bug.
-   ⚠️ **No Unicode / wide-character support** --- input is treated as
    bytes.
-   ⚠️ **`Output.xls` is actually CSV** --- Excel opens it fine, but it
    is not a true `.xlsx` binary. Some Excel versions may prompt about
    format; choose *Yes* to open.

------------------------------------------------------------------------

## 👨‍💻 Author

**👤 Vangimalla Naveen Kumar Reddy**

-   📧 Email: <vangimallanaveenkumarreddy1@gmail.com>
-   💼 LinkedIn:
    [naveenkumar-reddy-vangimalla](https://www.linkedin.com/in/naveenkumar-reddy-vangimalla-310184415/)

------------------------------------------------------------------------

## 📜 License

📖 This project is provided for educational purposes. Feel free to use,
modify, and share. 🤝

------------------------------------------------------------------------

🌟 **End of File** 🌟
