#include <stdio.h>

int get_from_name_table (const char *name, int size, bool *is_global, bool is_param = false);

int add_to_global_names (const char *name, int size);

void init_name_tables ();

void free_name_tables ();

void clear_local_names ();

int get_local_offset ();
