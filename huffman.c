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

typedef struct codedChar
{
    char character;
    char *codification;
} CodedChar;

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


void findChar(Node *node, char c, char *codification,  int i, char *result) {
    if (node == NULL) {
        return;
    }
    if (node->character == c) {
        codification[i] = '\0';
        strcpy(result, codification);
        return;
    }
    if (node->left) {
        codification[i] = '0';
        findChar(node->left, c, codification, i + 1, result);
    }
    if (node->right) {
        codification[i] = '1';
        findChar(node->right, c, codification, i + 1, result);
    }
}
int main()
{
    FILE *sample = fopen("amostra.txt", "r");
    if (sample == NULL)
    {
        exit(1);
    }

    char characters[SIZE] = {0}, *codification[SIZE] = {0};
    int frequencies[SIZE] = {0};

    char c = fgetc(sample);
    int flag = 0, uniqueCharacters = 0, numCharacters = 0;

    while (!feof(sample))
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
            c = fgetc(sample);
            continue;
        }
        numCharacters++;
        characters[uniqueCharacters] = c;
        frequencies[uniqueCharacters]++;
        uniqueCharacters++;
        c = fgetc(sample);
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

    rewind(sample); // Volta o leitor da pasta ao seu início.
    c = fgetc(sample);
    char result[SIZE];
    char encodedText[SIZE * 8] = {0};

    for (int i = 0; i < uniqueCharacters; i++) {
        findChar(root, characters[i], codification, 0, result);
    }
    CodedChar codedCharList[uniqueCharacters];
    for (int i = 0; i < uniqueCharacters; i++) {
        codedCharList[i].character = characters[i];
        findChar(root, codedCharList[i].character, codification, 0, result);
        codedCharList[i].codification = strdup(result);
        printf("Caractere '%c': %s\n", characters[i], codedCharList[i].codification);
    }
    // Percorre toda a pasta novamente, até encontrar uma quebra de linha.
    while (!feof(sample))
    {
        findChar(root, c, codification, 0, result);        
        strcat(encodedText, result);
        c = fgetc(sample);
    }

    fclose(sample);
    FILE *coded = fopen("codificado.txt", "w");
    fprintf(coded, encodedText);
    fclose(coded);
    coded = fopen("codificado.txt", "r");
    char string[SIZE] = {0};
    while (!feof(coded)){
        c = fgetc(coded);
        char temp[2] = {c, '\0'};
        strcat(string, temp);
    }
    fclose(coded);
    FILE *decoded = fopen("decodificado.txt", "w+");
    char comparation[SIZE] = {0};
    for (int i = 0; i < strlen(string)-1; i++)
    {
        char aux[2] = {string[i], '\0'};
        strcat(comparation, aux);
            for (int j = 0; j < uniqueCharacters; j++)
            {
                if (strcmp(codedCharList[j].codification, comparation) == 0)
                {
                    fputc(codedCharList[j].character, decoded);
                    strcpy(comparation, "");
                }
                
            }
            
    }
           
    
}
