#include "name_table.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define DEB(...) printf (__VA_ARGS__)

static const int NOT_FOUND_TABLE = -0x1F0DE;

// TODO remove global vars
static NameTable global_names = {};
static NameTable local_names = {};

static int find_in_table (const char *name, NameTable *table);

static int add_to_table (const char *name, int size, NameTable *table);

static int get_table_offset (const NameTable *table);

void init_name_tables ()
    {
    global_names.vars = (VarName*) calloc (1024, sizeof (VarName));
    local_names.vars = (VarName*) calloc (1024, sizeof (VarName));
    }

void free_name_tables ()
    {
    free (global_names.vars);
    global_names.vars = nullptr;
    free (local_names.vars);
    local_names.vars = nullptr;
    }

void clear_local_names ()
    {
    local_names.cnt = 0;
    }

int get_from_name_table (const char *name, int size, bool *is_global)
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
        offset = add_to_table (name, size, &local_names);

    DEB ("Success local\n");
    return offset;
    }

static int find_in_table (const char *name, NameTable *table)
    {
    assert (name);
    assert (table);
    
    for (int i = 0; i < table->cnt; i++)
        if (!strcmp (table->vars[i].name, name))
            return table->vars[i].offset;
            
    return NOT_FOUND_TABLE;
    }

static int add_to_table (const char *name, int size, NameTable *table)
    {
    assert (name);
    assert (size);
    assert (table);
    
    if (!table->cnt)
        {
        DEB ("Adding to begin\n");
        table->vars[0].name = strdup (name);
        table->vars[0].offset = 0;
        table->vars[0].size = size;
        table->cnt++;
        DEB ("Added to begin, return 0\n");
        return 0;
        }
    
    table->vars[table->cnt].offset = table->vars[table->cnt - 1].offset + 
                                        table->vars[table->cnt - 1].size;
    table->vars[table->cnt].name = strdup (name);
    table->vars[table->cnt].size = size;
    table->cnt++;
    return table->vars[table->cnt - 1].offset;
    }

int add_to_global_names (const char *name, int size)
    {
    return add_to_table (name, size, &global_names);
    }


int get_local_offset ()
    {
    return get_table_offset (&local_names);
    }

int get_global_offset ()
    {
    return get_table_offset (&global_names);
    }

static int get_table_offset (const NameTable *table)
    {
    assert (table);

    if (!table->cnt)
        return 0;
    
    return table->vars[table->cnt - 1].offset + table->vars[table->cnt - 1].size;
    }
