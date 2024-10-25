# SLR1_Parser

# 🚧 Project Status

This project is still under development. Currently, it displays the LR automaton and the SLR(1) tables but does not yet support parsing input strings.
If you find any issues or have suggestions, please feel free to open an issue—contributions are welcome!

# 📋 Requirements
- Linux (currently): The dlfcn.h header is used for dynamic library loading.
- Flex: Required to generate and compile the lexer file.
- gcc: Used for compilation within the code.
- g++ (or clangd, although untested): The Makefile is set to use g++, but it can be adjusted for other compilers.

# 🛠️ Compilation
Run make to compile the project.

# ▶️ Running the Program
Once compiled, you can run the program as follows:
- `./slrparser <GRAMMAR_FILENAME>`. Generates and displays the LR automaton and the transition/action tables for the provided grammar.

# 📌 Considerations
- The default end-of-line character is `$`. This can be changed using the instruction `set EOL char (...)`.
- The end-of-line character can be omitted in the grammar file (see grammar.txt), but it's recommended to add a rule like `S -> E EOL`, where `S` is the start symbol.
- For terminal symbols, note that order matters. If two regexes have common elements, place the more specific one first, as in this example:
~~~
terminal WH while;
terminal WORD [a-zA-Z][a-zA-Z]*;
~~~

# 📄 Structure of grammar.txt
The grammar file has two sections separated by `;`: **symbol definition** and **grammar definition**.

## Symbol Definition
Define the start symbol and terminals as follows:
~~~
start with S;
terminal <IDENTIFIER> <REGEX>;
~~~
Example:
~~~
terminal a a;
start with S;
;
~~~
## Grammar Definition
Grammar rules follow this structure:
~~~
S -> A$;
A -> aaA;
A ->;
;
~~~
Here, `A ->;` represents an empty production.

## Full Example of grammar.txt
~~~
terminal a a;
start with S;
;
S -> A$;
A -> aaA;
A ->;
;
~~~
