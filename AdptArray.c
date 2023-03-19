#include <stdlib.h>
#include "AdptArray.h"


typedef struct AdptArray_
{
	PElement* arr;
	int size;

	COPY_FUNC copy_element;
	DEL_FUNC delete_element;
	PRINT_FUNC print_element;
}AdptArray;


PAdptArray CreateAdptArray(COPY_FUNC copy, DEL_FUNC del, PRINT_FUNC print)
{
	PAdptArray paa = (PAdptArray)malloc(sizeof(AdptArray));

	paa->arr = NULL;
	paa->size = 0;
	paa->copy_element = copy;
	paa->delete_element = del;
	paa->print_element = print;
	return paa; // NULL if malloc was failed.
}


void DeleteAdptArray(PAdptArray paa)
{
	// free the elements inside the array.
	for (int i = 0; i < paa->size; i++)
	{
		if (paa->arr[i])
		{
			paa->delete_element(paa->arr[i]);
			paa->arr[i] = NULL;
		}
	}
	
	// free the array itself. 
	if (paa->arr) {free(paa->arr);}
	paa->arr = NULL;

	// clean the rest.
	paa->size = 0;
	paa->copy_element = NULL;
	paa->delete_element = NULL;
	paa->print_element = NULL;

	free(paa);
}


Result SetAdptArrayAt(PAdptArray paa, int index, PElement pe)
{
	// index is exist
	if (index < paa->size)
	{
		// free the older element if it was there.
		if (paa->arr[index])
		{
			paa->delete_element(paa->arr[index]);
		}
		
		// assign a copy of the new element.
		paa->arr[index] = paa->copy_element(pe);
	}
	else
	{
		// the first time
		if (!paa->arr)
		{
			paa->arr = (PElement*)calloc(index + 1, sizeof(PElement));
		}
		else
		{
			//paa->arr = (PElement*)realloc(paa->arr, sizeof(PElement) * (index + 1));
			PElement* new_arr = (PElement*)calloc(index + 1, sizeof(PElement));
			for (int i = 0; i < paa->size; i++)
			{
				new_arr[i] = paa->arr[i];
			}
			free(paa->arr);
			paa->arr = new_arr;
		}

		// assign a copy of the new element.
		paa->arr[index] = paa->copy_element(pe);
		
		// update the new size. (-1 at fails)
		paa->size = (paa->arr != NULL) ? index + 1 : -1;
	}

	return paa->size != -1 ? SUCCESS : FAIL;
}


PElement GetAdptArrayAt(PAdptArray paa, int index)
{
	if (index >= paa->size)
	{
		return NULL;
	}

	PElement pe = paa->arr[index];

	return pe != NULL ? paa->copy_element(pe) : NULL;
}


int GetAdptArraySize(PAdptArray paa)
{
	return paa->size;
}


void PrintDB(PAdptArray paa)
{
	for (int i = 0; i < paa->size; i++)
	{
		if (paa->arr[i])
		{
			paa->print_element(paa->arr[i]);
		}
	}
}