#include "name_table.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define DEB(...) printf (__VA_ARGS__)

struct VarName {
    char *name;
    int offset;
    int size;
};

struct NameTable {
    size_t cnt_vars;
    size_t cnt_params;
    VarName *vars;
};

static const int NOT_FOUND_TABLE = -0x1F0DE;

// TODO remove global vars
static NameTable global_names = {};
static NameTable local_names = {};

static int find_in_table (const char *name, NameTable *table);

static int add_to_table (const char *name, int size, NameTable *table, bool is_param = false);

// TODO do i need to change using these constants?
void init_name_tables ()
    {
    global_names.vars = (VarName*) calloc (1024, sizeof (VarName)) + 128;
    global_names.cnt_params = 1;
    global_names.cnt_vars = 0;
    local_names.vars = (VarName*) calloc (1024, sizeof (VarName)) + 128;
    global_names.cnt_params = 1;
    local_names.cnt_vars = 0;
    }

void free_name_tables ()
    {
    free (global_names.vars -  128);
    global_names.vars = nullptr;
    free (local_names.vars -  128);
    local_names.vars = nullptr;
    }

void clear_local_names ()
    {
    local_names.cnt_params = 1;
    local_names.cnt_vars = 0;
    }

int get_from_name_table (const char *name, int size, bool *is_global, bool is_param)
    {
    assert (name);
    assert (is_global);

    int offset = find_in_table (name, &global_names);
    DEB ("Through global\n");
    if (offset != NOT_FOUND_TABLE)
        {
        *is_global = true;
        return offset;
        }
    DEB ("Trying local\n");
    
    *is_global = false;
    offset = find_in_table (name, &local_names);
    DEB ("Through local\n");
    if (offset == NOT_FOUND_TABLE)
        offset = add_to_table (name, size, &local_names, is_param);

    DEB ("Success local\n");
    return offset;
    }

static int find_in_table (const char *name, NameTable *table)
    {
    assert (name);
    assert (table);
    
    for (int i = 0; i < table->cnt_vars; i++)
        if (!strcmp (table->vars[i].name, name))
            return table->vars[i].offset;

    for (int i = 1; i < table->cnt_params; i++)
        if (!strcmp (table->vars[-i].name, name))
            return table->vars[-i].offset;
            
    return NOT_FOUND_TABLE;
    }

static int add_to_table (const char *name, int size, NameTable *table, bool is_param)
    {
    assert (name);
    assert (size);
    assert (table);

    DEB ("param: %d, cnt: %d\n\n", is_param, table->cnt_vars);
    DEB ("%s\n", name);
    DEB ("PRINTED\n");
    
    if (!is_param && !table->cnt_vars)
        {
        DEB ("Adding to begin\n");
        table->vars[0].name = strdup (name);
        DEB ("Dupped name\n");
        table->vars[0].offset = 1; // for saving ebp
        table->vars[0].size = size;
        table->cnt_vars++;
        DEB ("Added to begin, return 0\n");
        return 1;
        }

    else if (is_param && table->cnt_params == 1)
        {
        DEB ("Adding to begin of params\n");
        table->vars[-1].name = strdup (name);
        table->vars[-1].offset = -1 - size; // for saving ret address
        table->vars[-1].size = size;
        table->cnt_params++;
        DEB ("Added first param\n");
        return -1-size;
        }
    
    if (!is_param) 
        {
        table->vars[table->cnt_vars].offset = table->vars[table->cnt_vars - 1].offset + 
                                                  table->vars[table->cnt_vars - 1].size;
        table->vars[table->cnt_vars].name = strdup (name);
        table->vars[table->cnt_vars].size = size;
        table->cnt_vars++;
        return table->vars[table->cnt_vars - 1].offset;
        }

    // is_param is true
    table->vars[-table->cnt_params].offset = table->vars[-table->cnt_params + 1].offset -
                                                  table->vars[-table->cnt_params + 1].size;
    table->vars[-table->cnt_params].name = strdup (name);
    table->vars[-table->cnt_params].size = size;
    table->cnt_params++;
    return table->vars[-table->cnt_params + 1].offset;
    
    }

int add_to_global_names (const char *name, int size)
    {
    return add_to_table (name, size, &global_names);
    }

int get_local_offset ()
    {
    if (!local_names.cnt_vars)
        return 0;
    
    return local_names.vars[local_names.cnt_vars - 1].offset + 
                local_names.vars[local_names.cnt_vars - 1].size;
    }

