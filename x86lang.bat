set "files=main.cpp frontend/lexer/lexer.cpp ../study_tasks/onegin/file_utils.cpp ../study_tasks/asm/asm/string_utils.cpp frontend/frontend.cpp tree/lang_tree.cpp x86backend/backend.cpp x86backend/name_table/name_table.cpp"
set "warnings=-Wall -Wextra -Wpedantic -Wno-sign-compare"
g++ %files% %warnings% -I "../study_tasks" -o uzh %* 
