#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256
typedef struct node
{
    char character;
    int frequency;
    struct node *right;
    struct node *left;
} node;

node* makeNode(char character, int frequency){
    node* newNode = (node*)malloc(sizeof(node));
    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void bubbleSort(node* array[]){
    for (int i = 0; i < SIZE-1; i++)
    {
        for (int j = 0; j < SIZE-i-1; i++)
        {
            if (array[j]->frequency > array[j+1]->frequency)
            {
                node* aux = (node*)malloc(sizeof(node));
                aux = array[j];
                array[j] = array[j+1];
                array[j+1] = aux;
            }
        }
    }
    // for (int i = 1; i < SIZE; i++)
    // {
    //     for (int j = 0; j < SIZE-1; i++)
    //     {
    //         if (array[j]->frequency == array[j+1]->frequency && array[j]->character > array[j+1]->character)
    //         {
    //             node* aux = (node*)malloc(sizeof(node));
    //             aux = array[j];
    //             array[j] = array[j+1];
    //             array[j+1] = aux;
    //         }
            
    //     }
        
    // }
    
}

int main()
{
    FILE *amostra = fopen("amostra.txt", "r");
    if (amostra == NULL)
    {
        exit(1);
    }
    char characters[SIZE] = {0};
    int frequencies[SIZE] = {0};
    char c = fgetc(amostra);
    int flag = 0;
    int j = 0;
    while (!feof(amostra))
    {
        flag = 0;
        for (int i = 0; i < j; i++)
        {
            if (c == characters[i])
            {
                flag = 1;
                frequencies[i]++;
                continue;
            }
        }
        if (flag == 1)
        {
            c = fgetc(amostra);
            continue;
        }
        characters[j] = c;
        frequencies[j]++;
        j++;
        c = fgetc(amostra);
    }
    node* arrayNodes[j];
    for (int i = 0; i < j; i++)
    {
        // printf("caractere: %c, frequencia: %d\n", characters[i], frequencies[i]);
        arrayNodes[i] = makeNode(characters[i], frequencies[i]);
    }
    for (int i = 0; i < j; i++) 
    {
        printf("caractere: %c, frequencia: %d\n", arrayNodes[i]->character, arrayNodes[i]->frequency);
    }
    
    bubbleSort(arrayNodes);
    printf("ORDENADO\n");
    for (int i = 0; i < j; i++) 
    {
        printf("caractere: %c, frequencia: %d\n", arrayNodes[i]->character, arrayNodes[i]->frequency);
    }
    
    return 0;
}

