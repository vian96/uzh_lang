# Uzh programming language

This is uzh lang (уж in russian - a snake).

It uses basic syntax of python with minor changes (see [examples folder](url)).

Source code is parced to tree with [standart](https://github.com/futherus/Language) made by [futherus](https://github.com/futherus).

# Requirements

Onegin library, assembler and cpu from [vian96/study_tasks](https://github.com/vian96/study_tasks) (the last one also requires stack from that repo).

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
