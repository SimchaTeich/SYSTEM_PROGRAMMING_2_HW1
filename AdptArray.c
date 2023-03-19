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
	}
	else
	{
		// if this is the first time of inserting element.
		if (!paa->arr)
		{
			paa->arr = (PElement*)calloc(index + 1, sizeof(PElement));
			if(paa->arr)
			{
				paa->size = index + 1;
			}
			else{ paa->size = -1; }
		}
		else
		{
			//paa->arr = (PElement*)realloc(paa->arr, sizeof(PElement) * (index + 1));
			PElement* new_arr = (PElement*)calloc(index + 1, sizeof(PElement));
			if(new_arr)
			{
				for (int i = 0; i < paa->size; i++)
				{
					new_arr[i] = paa->arr[i];
				}
				free(paa->arr);
				
				paa->arr = new_arr;
				paa->size = index + 1;
			}
			else { paa->size = -1; }
		}
	}

	// assign a copy of the new element.
	if(paa->size != -1) {paa->arr[index] = paa->copy_element(pe);}

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