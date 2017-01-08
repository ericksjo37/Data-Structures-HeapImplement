/*
 * CS 261 Assignment 5
 * Name: Josh Erickson
 * Date: 7/31/16
 */

#include "dynamicArray.h"
#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
*Compares a task with each task already in the heap.   
*@param heap
*@param the new task to compare to the heap 
*@Returns: 0 if not present, 1 if present  
*/

int duplicateCheck(DynamicArray *heap, struct Task *task)
{
	int i;
	DynamicArray* temp = dyNew(1);
	dyCopy(heap, temp);
	for (i = dySize(heap); i > 0; i--)
	{
		Task* tempTask = dyHeapGetMin(temp);
		if ((taskCompare(tempTask, task) == 0) && ((strcmp(tempTask->name, task->name) == 0)))
		{
			dyDelete(temp);
			return 1;
		}
		dyHeapRemoveMin(temp, taskCompare);
	}
	dyDelete(temp);
	return 0;
}

/**
 * Loads into heap a list from a file with lines formatted like
 * "priority, name".
 * @param heap
 * @param file
 */
void listLoad(DynamicArray* heap, FILE* file)
{
    const int FORMAT_LENGTH = 256;
    char format[256];
 snprintf(format, FORMAT_LENGTH, "%%d, %%%d[^\n]", TASK_NAME_SIZE);
    
    Task temp;
    while (fscanf(file, format, &temp.priority, &temp.name) == 2)
    { 
	struct Task *newTask = taskNew(temp.priority, temp.name);           	
	if (duplicateCheck(heap, newTask) == 0) 
  	if (duplicateCheck(heap, newTask) == 0){        
		  dyHeapAdd(heap, newTask, taskCompare);
	}      
    }     

 }


/**
 * Writes to a file all tasks in heap with lines formatted like
 * "priority, name".
 * @param heap
 * @param file
 */
void listSave(DynamicArray* heap, FILE* file)
{
    for (int i = 0; i < dySize(heap); i++)
    {
        Task* task = dyGet(heap, i);
        fprintf(file, "%d, %s\n", task->priority, task->name);
    }
}

/**
 * Prints every task in heap.
 * @param heap
 */
void listPrint(DynamicArray* heap)
{
	//Create a new container for after we trash our other one by sorting
	int i;
    DynamicArray* temp = dyNew(1);
    dyCopy(heap, temp);
	for (i = dySize(heap); i > 0; i--)
    {
        Task* task = dyHeapGetMin(temp);
        printf("\n");
        taskPrint(task);
        printf("\n");
        dyHeapRemoveMin(temp, taskCompare);
    }
    dyDelete(temp);
}

/**
 * Handles the given command.
 * @param list
 * @param command
 */
void handleCommand(DynamicArray* list, char command)
{
	char taskName[128], *newLine, fileTitle[28];
	int priority;
	Task *topPrio;
	FILE *file;
	switch (command)
	{
	case 'l':
	{
		printf("Please enter the filename: ");

		if (fgets(fileTitle, sizeof(fileTitle), stdin) != NULL)
		{
			newLine = strchr(fileTitle, '\n');
			if (newLine)
				*newLine = '\0';
		}

		file = fopen(fileTitle, "r");
		if (file == NULL)
		{
			fprintf(stderr, "Cannot open %s\n", fileTitle);
			return;
		}

		listLoad(list, file);
		fclose(file);
		printf("The list has been loaded from file successfully.\n\n");
		break;
	}

	case 's':
	{
		if (dySize(list) > 0)
		{
			printf("Please enter the filename: ");
			if (fgets(fileTitle, sizeof(fileTitle), stdin) != NULL)
			{
				newLine = strchr(fileTitle, '\n');
				if (newLine)
					*newLine = '\0';
			}
			file = fopen(fileTitle, "w");
			if (file == NULL)
			{
				fprintf(stderr, "Cannot open %s\n", fileTitle);
				return;
			}

			listSave(list, file);

			fclose(file);
			printf("The list has been saved into the file successfully.\n\n");

			break;
		}
	}

	case 'a':
	{
		Task *new;
		printf("Please enter the task description: ");
		if (fgets(taskName, sizeof(taskName), stdin) != NULL)
		{
			newLine = strchr(taskName, '\n');
			if (newLine)
				*newLine = '\0';
		}
		
		printf("Please enter the task priority (0-999): ");
		scanf("%d", &priority);
		while (priority < 0 || priority > 999)
		{
			printf("Please enter the task priority (0-999): ");
			scanf("%d", &priority);
		}

		while (getchar() != '\n');
		

		new = taskNew(priority, taskName);
		if (duplicateCheck(list, new) == 1)
				{
					printf("That task is already in your to-do list\n\n");
					break;
				}
		dyHeapAdd(list, new, taskCompare);
		printf("The task '%s' has been added to your to-do list.\n\n", taskName);
		break;
	}

	case 'g':
	{
		if (dySize(list) > 0)
		{
			topPrio = dyHeapGetMin(list);
			printf("Your first task is: %s\n\n", topPrio->name);
		}
		else
			printf("Your to-do list is empty!\n\n");
		break;
	}

	case 'r':
	{
		if (dySize(list) > 0)
		{
			topPrio = dyHeapGetMin(list);
			dyHeapRemoveMin(list, taskCompare);
			printf("Your first task '%s' has been removed from the list\n\n", topPrio->name);
		}
		else
			printf("Your to-do list is empty!\n\n");
		break;
	}

	case 'p':
	{
		if (dySize(list) > 0)
		{
			listPrint(list);
		}
		else
			printf("Your to-do list is empty!\n\n");
		break;
	}
	case 'e':
	{
		printf("Bye!\n\n");
		break;
	}
	}
}

int main()
{
    // Implement
    printf("\n\n** TO-DO LIST APPLICATION **\n\n");
    DynamicArray* list = dyNew(8);
    char command = ' ';
    do
    {
        printf("Press:\n"
               "'l' to load to-do list from a file\n"
               "'s' to save to-do list to a file\n"
               "'a' to add a new task\n"
               "'g' to get the first task\n"
               "'r' to remove the first task\n"
               "'p' to print the list\n"
               "'e' to exit the program\n"
        );
        command = getchar();
        // Eat newlines
        while (getchar() != '\n');
        handleCommand(list, command);
    }
    while (command != 'e');
    dyDelete(list);
    return 0;
}
