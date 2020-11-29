#include <glib.h>
#include <gmodule.h>
#include <glib/gprintf.h>
#include <stdio.h>

#ifndef OWN_DELIMITERS
#define OWN_DELIMITERS " \n\t\\/:?!.,;-"
#endif

void process_line(GHashTable* map, const gchar* line)
{
	char** words = g_strsplit_set(line, OWN_DELIMITERS, 0);
	for (gint i = 0; words[i] != NULL; ++i)
	{
		gchar* word = words[i];
		if (g_strcmp0(word, "") == 0)
			continue;

		gint value = -1;
		gchar* origKey = NULL;
		if(TRUE == g_hash_table_lookup_extended(map, word, (void**)&origKey, (void**)&value))
		{
			value += 1;
			g_hash_table_replace(map, g_strdup(origKey), GINT_TO_POINTER(value));
		} else {
			gint newVal = 1;
			g_hash_table_insert(map, g_strdup(word), GINT_TO_POINTER(newVal)); 
		}
	}	
	g_strfreev(words);
}

gint cmp(gconstpointer a, gconstpointer b, gpointer user_data)
{
	GHashTable* counter = (GHashTable*)user_data;
	int value1 = GPOINTER_TO_INT(g_hash_table_lookup(counter, a));
       	int value2 = GPOINTER_TO_INT(g_hash_table_lookup(counter, b));
	if (value1 > value2)
	{
		return -1;
	} else if (value2 > value1) {
		return 1;
	} else {
		return g_strcmp0((gchar*)a, (gchar*)b);
	}
}

int main(int argc, char** argv)
{
	GHashTable* counter = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
	gchar* data = g_malloc(80);
	size_t len = 80;
	
	while (getline(&data, &len, stdin) != -1)
	{
		process_line(counter, data);
	}

	GList* namesList = g_hash_table_get_keys(counter);
	namesList = g_list_sort_with_data(namesList, cmp, counter);
	
	for (GList* l = namesList; l != NULL; l = l->next)
	{
		int value = GPOINTER_TO_INT(g_hash_table_lookup(counter, l->data));
		g_printf("%s = %d\n", (gchar*)l->data, value);
	}
	
	g_list_free(namesList);
	g_hash_table_remove_all(counter);
	g_free(data);
	return 0;
}
