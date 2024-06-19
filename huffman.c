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
} Node;

Node *makeNode(char character, int frequency)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void bubbleSort(Node **array, int size)
{
    // Ordenação por frequência.
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (array[j]->frequency > array[j + 1]->frequency)
            {
                Node *aux = array[j];
                array[j] = array[j + 1];
                array[j + 1] = aux;
            }
        }
    }

    // Ordenação alfabética, mantendo a ordem de frequência.
    for (int i = 1; i < size; i++)
    {
        for (int j = 0; j < size - 1; j++)
        {
            if (array[j]->frequency == array[j + 1]->frequency && array[j]->character > array[j + 1]->character)
            {
                Node *aux = array[j];
                array[j] = array[j + 1];
                array[j + 1] = aux;
            }
        }
    }
}

Node *makeHuffman(Node **array, int size)
{
    while (size > 1)
    {
        bubbleSort(array, size);

        Node *left = array[0];
        Node *right = array[1];

        Node *newNode = makeNode('\0', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        array[0] = newNode; // O primeiro elemento do vetor é substituído pelo novo nó.
        for (int i = 2; i < size; i++)
        {
            array[i - 1] = array[i];
        }
        array[size - 1] = NULL;
        size--;
    }
    return array[0];
}

void buscaLetra(Node *node, char c, char **codification, int i)
{
    if (node->right)
    {
        // add 1 na codificação
        codification[i] = '1';
        i++;
        return buscaLetra(node->right, c, codification, i);
    }
    else if (node->left)
    {
        // add 0 na codificação
        codification[i] = '0';
        i++;
        return buscaLetra(node->left, c, codification, i);
    }
}

int main()
{
    FILE *amostra = fopen("amostra.txt", "r");
    if (amostra == NULL)
    {
        exit(1);
    }

    char characters[SIZE] = {0}, *codification[SIZE] = {0};
    int frequencies[SIZE] = {0};

    char c = fgetc(amostra);
    int flag = 0, uniqueCharacters = 0, numCharacters = 0;

    while (!feof(amostra))
    {
        flag = 0;
        for (int i = 0; i < uniqueCharacters; i++)
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
        numCharacters++;
        characters[uniqueCharacters] = c;
        frequencies[uniqueCharacters]++;
        uniqueCharacters++;
        c = fgetc(amostra);
    }
    Node *arrayNodes[numCharacters];

    // Criação dos nós, com a inserção de seu caracter e frequência.
    for (int i = 0; i < uniqueCharacters; i++)
    {
        arrayNodes[i] = makeNode(characters[i], frequencies[i]);
    }

    // Ordenação inicial do vetor.
    bubbleSort(arrayNodes, numCharacters);

    // Criação da árvore de Huffman.
    Node *root = makeHuffman(arrayNodes, numCharacters);
    // printf("%c", root->right->right->right->character);

    rewind(amostra); // Volta o leitor da pasta ao seu início.
    c = fgetc(amostra);

    // Percorre toda a pasta novamente, até encontrar uma quebra de linha.
    while (!feof(amostra))
    {
        buscaLetra(root, c, codification, 0);
        c = fgetc(amostra);
    }

    printf("%s", codification);
}
