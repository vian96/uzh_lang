set "files=main.cpp lexer/lexer.cpp ../study_tasks/onegin/file_utils.cpp ../study_tasks/asm/asm/string_utils.cpp"
set "warnings=-Wall -Wextra -Wpedantic"
g++ %files% %warnings% -I "../study_tasks" -o main 