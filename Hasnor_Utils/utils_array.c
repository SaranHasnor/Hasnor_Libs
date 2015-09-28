#include "utils_array.h"

#include "utils.h"

void array_init(array_t *ar)
{
	ar->content = NULL;
	ar->size = 0;
}

void array_add(array_t *ar, void *object)
{
	ar->content = (void**)mem_realloc(ar->content, sizeof(void*) * (ar->size+1));
	ar->content[ar->size] = object;
	ar->size++;
}

void array_insert(array_t *ar, void *object, uint pos)
{
	uint i;
	ar->content = (void**)mem_realloc(ar->content, sizeof(void*) * (ar->size+1));
	for (i = ar->size; i > pos; i--)
	{
		ar->content[i] = ar->content[i-1];
	}
	ar->content[pos] = object;
	ar->size++;
}

void array_remove(array_t *ar, void *object)
{
	uint i;
	bool found = false;
	for (i = 0; i < ar->size; i++)
	{
		if (ar->content[i] == object)
		{
			found = true;
		}

		if (found && i < ar->size - 1)
		{
			ar->content[i] = ar->content[i+1];
		}
	}
	if (found)
	{
		if (ar->size == 1)
		{
			ar->content = NULL;
		}
		else
		{
			ar->content = (void**)mem_realloc(ar->content, sizeof(void*) * (ar->size-1));
		}
		ar->size--;
	}
}

void array_removeAt(array_t *ar, uint pos)
{
	if (pos < ar->size)
	{
		uint i;
		for (i = pos; i < ar->size - 1; i++)
		{
			ar->content[i] = ar->content[i+1];
		}
		if (ar->size == 1)
		{
			ar->content = NULL;
		}
		else
		{
			ar->content = (void**)mem_realloc(ar->content, sizeof(void*) * (ar->size-1));
		}
		ar->size--;
	}
}

int array_indexOf(array_t *ar, void *object)
{
	uint i = 0;
	while (i < ar->size)
	{
		if (ar->content[i] == object)
		{
			return (int)i;
		}
		i++;
	}
	return -1;
}

int array_indexOfFirstMatch(array_t *ar, bool (*predicate)(void *object))
{
	uint i = 0;
	while (i < ar->size)
	{
		if (predicate(ar->content[i]))
		{
			return (int)i;
		}
		i++;
	}
	return -1;
}