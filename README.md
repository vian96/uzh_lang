# Uzh programming language

This is uzh lang (уж in russian - a snake).

It uses basic syntax of python with minor changes (see [examples folder](url)).

Source code is parsed to tree with [standart](https://github.com/futherus/Language) made by [futherus](https://github.com/futherus).

# Requirements

Onegin library, assembler and cpu from [vian96/study_tasks](https://github.com/vian96/study_tasks) (the last one also requires stack from that repo).

# Setup

If you downloaded this repo without change and folder `uzh_lang` is in the same directory as repo `study_tasks` you can just run `lang.bat` for compiling. Otherwise you need to change files in these bat-files to yours. 

To compile uzh file call `uzh file_name` ***WITHOUT extension***. You can run your app with `app file_name` or simply pass it to your cpu executor. E.g. you can run `uzh examples/ackerman && "examples/ackerman.boac"`.

# Problems

- You can not have more than 100 entities in one line (don't ask why).

- Keywords _const_ and _main_ are currently not in use.

- Currently, symbol '_' is not supported.

- You have to have return at the end of every function (except main, see further)

- You can not use return in main

- You can not return nothing

- You probably (I am not sure lol) can not leave garbage in stack (i.e. you should always use result of calculating of expression)

- There is no any proper documentation

- Params are currently calculated right-to-left, but it can change so it is UB

- You can not have more than 1000 local or global variables

# TODO

- `break` keyword

- Else bracnch for while

- For loop (I have no idea what way I should realise it tho)

- Remove main function probably?

- Structs

- More proper array declaration

- Dynamic memory management (I need to write malloc in _uzh_ itself)

- Change to float ~~lol~~.



