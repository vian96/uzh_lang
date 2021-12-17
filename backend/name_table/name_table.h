#include "../backend.h"

struct VarName {
    char *name;
    int offset;
    int size;
};

struct NameTable {
    size_t cnt;
    VarName *vars;
};

int get_from_name_table (const char *name, int size, bool *is_global);

int add_to_global_names (const char *name, int size);

void init_name_tables ();

void free_name_tables ();

void clear_local_names ();
