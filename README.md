# Uzh programming language

This is uzh lang (_Уж_ in russian - a snake).

It uses basic syntax of python with minor changes (see [examples folder](url)).

Source code is parsed to tree with [standart](https://github.com/futherus/Language) made by [futherus](https://github.com/futherus) using grammar defined in `grammar.gr`.

It is compiled into either x86 32-bit Windows NASM assembly or into CASM from [vian96/study_tasks](https://github.com/vian96/study_tasks).

# Major difference with python

- `||` and `&&` instead of `and` and `or` (in x86 mode they are also bitwise, not boolean, which is TODO)

- You can only init global vars outside of function, in the beginning, `main` is called

- You can init arrays only with numbers (`arr[99] = 0` creates array with len of 99+1 and inits element 99 with zero)

- Globals are always covering locals

- Input does not need `()`

# Requirements

For CASM mode you need Onegin library, assembler and cpu from [vian96/study_tasks](https://github.com/vian96/study_tasks) (the last one also requires stack from that repo).

For x86 version you don't need asm and cpu from [vian96/study_tasks](https://github.com/vian96/study_tasks) but you need NASM and GoLink.

# Setup

If you downloaded this repo without change and folder `uzh_lang` is in the same directory as repo `study_tasks` then you can just run `lang.bat` for CASM mode and `x86lang.bat` for compiling. Otherwise you need to change files in these bat-files to yours. 

To compile uzh file call `uzh file_name` ***WITHOUT extension***. In CASM mode, you can run your app with `app file_name` or simply pass it to your cpu executor. E.g. you can run `uzh examples/ackerman && "examples/ackerman.boac"`.

# Issues

- You can not have more than 100 entities in one line (don't ask why).

- Keywords _const_ and _main_ are currently not in use.

- Currently, symbol '_' is not supported.

- You have to have return at the end of every function (except main, see further)

- You can not use return in main

- You can not return nothing

- You probably (I am not sure lol) can not leave garbage in stack (i.e. you should always use result of calculating of expression)

- There is no any proper documentation

- Params are currently calculated right-to-left, but it can change so it is UB

- You can not have more than 1000 local or global variables and use more than 5000 memory cells

- This compiler ***completely does not show any proper errors***, so you should be very careful using it. It has A LOT of things it can do that are basically kinda UB.

- Logic operations in x86 mode are bitwise, not boolean

- Arguments of functions can not be arrays

# TODO

- `break` keyword

- Else bracnch for while

- For loop (I have no idea what way I should realise it tho)

- Remove main function probably?

- Structs (very important), it would also be better to be able to call `struct.func()`

- Dynamic memory management (I need to write malloc and free from c in _uzh_ itself), currently it has only stack-based dynamic memory allocation (see `dynamic.uzh`). Probably, it will be easier after structs

- Chars (not only putchar but also 'c')

- Files (hard)

- Types (hard probably)

- Modules/includes (also probably hard)

- More proper array declaration

- Change to float ~~lol~~

- Show errors, static analysis

