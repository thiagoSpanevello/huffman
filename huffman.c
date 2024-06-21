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

// Struct para organizar a letra com sua codificação;
typedef struct codedChar
{
    char character;
    char *codification;
} CodedChar;

// Cria um novo nodo e retorna um ponteiro para ele.
Node *makeNode(char character, int frequency)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    // Atribuição dos parâmetros ao atributo do nó;
    newNode->character = character;
    newNode->frequency = frequency;

    // Ancorando os valores para não apontarem para endereços aleatórios na mamória.
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

// Ordena o vetor de nós primariamente por frequência e então por ordem alfabética.
void bubbleSort(Node **array, int size)
{
    // Ordenação por frequência.
    for (int i = 0; i < size - 1; i++)
    {
        // Cada iteração de i diminui em 1 o número de iterações de j,
        // devido ao último elemento da lista já estar ordenado.
        for (int j = 0; j < size - i - 1; j++)
        {
            // Se a frequência do atual for maior que a frequência de seu sucessor, troca as posições.
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
            // Caso o elemento j e j+1 possuírem a mesma frequência, serão ordenados alfabeticamente.
            if (array[j]->frequency == array[j + 1]->frequency && array[j]->character > array[j + 1]->character)
            {
                Node *aux = array[j];
                array[j] = array[j + 1];
                array[j + 1] = aux;
            }
        }
    }
}

// Transforma um vetor de vários nós em em um vetor de uma posição que contenha o nó da raiz de Huffman.
Node *makeHuffman(Node **array, int size)
{
    // Continua juntando nós até o tamanho do vetor ser 1;
    while (size > 1)
    {
        // No início de cada iteração o vetor é ordenado.
        bubbleSort(array, size);

        // Cria um novo nó sem letra alguma,
        // com sua frequência sendo a soma das frequências do nó da esquerda e direita.
        Node *newNode = makeNode('\0', array[0]->frequency + array[1]->frequency);
        newNode->left = array[0];
        newNode->right = array[1];

        array[0] = newNode; // O primeiro elemento do vetor é substituído pelo novo nó.

        // Para reorganizar a lista depois de transformar 2 nós em apenas 1,
        // o novo nó ocupará a posição 0 e então o vetor será iterado de modo
        // em que os elementos restantes passam para a posição anterior.
        for (int i = 2; i < size; i++)
        {
            array[i - 1] = array[i];
        }
        // Já que todos os valores foram passados para a posição anterior,
        // os 2 últimos valores serão iguais, por isso o último elemento será nulo.
        array[size - 1] = NULL;

        size--; // Conforme novos nós são criados, o tamanho do vetor irá ser reduzido.
    }
    return array[0];
}

// Função que procura o código resultante da codificação de huffman do caractere passado por parâmetro.
void findChar(Node *node, char c, char *codification, int i, char *result)
{
    // Caso o nó passado para a função seja nulo, retorna e sai da função.
    if (node == NULL)
    {
        return;
    }
    if (node->character == c) // Testa se o caractere do nó é igual ao caractere passado por parametro.
    {
        codification[i] = '\0';       // Coloca o \0 no fim da string de codificação para saber que é o fim dela.
        strcpy(result, codification); // Copia o conteúdo do vetor codificação para a variavel result.
        return;                       // Sai da função após achar o valor codificado do caractere procurado.
    }
    if (node->left) // Testa se a esquerda do nó atual é não nula, e entra no bloco se não for nula.
    {
        // Coloca um 0 na codificação pelo fato de que vamos andar para a esquerda na árvore de huffman.
        codification[i] = '0';

        // Chama a função de maneira recursiva, andando para a esquerda da árvore de huffman.
        findChar(node->left, c, codification, i + 1, result);
    }
    if (node->right)
    {
        // Coloca um 1 na codificação pelo fato de que vamos andar para a direita na árvore de huffman.
        codification[i] = '1';

        // Chama a função de maneira recursiva, andando para a direita da árvore de huffman.
        findChar(node->right, c, codification, i + 1, result);
    }
}

int main()
{
    // Abre o arquivo.
    FILE *sample = fopen("amostra.txt", "r");
    if (sample == NULL)
    {
        exit(1);
    }

    // Declaração de variáveis.
    char characters[SIZE] = {0}, *codification[SIZE] = {0};
    int frequencies[SIZE] = {0};
    int flag = 0, uniqueCharacters = 0, numCharacters = 0; // A variável flag diz se a letra já foi encontrada ou não.
    char c = fgetc(sample);

    // Percorre o arquivo até encontrar o final do arquivo.
    while (!feof(sample))
    {
        flag = 0;
        // Percorre a lista de caracteres únicos.
        for (int i = 0; i < uniqueCharacters; i++)
        {
            // Caso o caracter lidio já esteja na lista.
            if (c == characters[i])
            {
                flag = 1;
                frequencies[i]++;
                continue;
            }
        }
        // Caso a letra já esteja na lista caracteres únicos, apenas continua a leitura do arquivo.
        if (flag == 1)
        {
            c = fgetc(sample);
            continue;
        }

        numCharacters++;
        characters[uniqueCharacters] = c; // Adiciona a letra no índice do valor de caracteres únicos.
        frequencies[uniqueCharacters]++;  // Incrementa a frequência no índice do valor de caracteres únicos.
        uniqueCharacters++;
        c = fgetc(sample);
    }
    // Declaração do vetor que armazenará os nós.
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

    rewind(sample); // Volta o leitor da pasta ao seu início.

    char result[SIZE];                // Armazenará a codificação de cada caracter.
    char encodedText[SIZE * 8] = {0}; // Armazenará a codificação do texto completo.

    // Vetor que armazenará os caracteres com sua respectiva codificação.
    CodedChar codedCharList[uniqueCharacters];

    // Busca a codificação para cada caracter único.
    // Esse for serve apenas pra printar a codificação de um único caracter.
    for (int i = 0; i < uniqueCharacters; i++)
    {
        codedCharList[i].character = characters[i];                          // Adiciona o caracter da posição i para o vetor de caracteres codificados.
        findChar(root, codedCharList[i].character, codification, 0, result); // Encontra o código do tal caracter.
        codedCharList[i].codification = strdup(result);                      // Copia o resultado da codificação para o vetor.
        printf("Caractere '%c': %s\n", characters[i], codedCharList[i].codification);
    }
    c = fgetc(sample);

    // Percorre todo o arquivo novamente, até encontrar o final do arquivo.
    // Agora a codificação completa será feita.
    while (!feof(sample))
    {
        findChar(root, c, codification, 0, result); // Encontra a codificação do caracter.
        strcat(encodedText, result);                // Concatena o texto codificado com o código do caracter.
        c = fgetc(sample);
    }
    fclose(sample);

    FILE *coded = fopen("codificado.txt", "w");
    fprintf(coded, encodedText); // Grava o texto codificado no arquivo.
    fclose(coded);

    coded = fopen("codificado.txt", "r");

    // Armazenará o texto decodificado.
    char string[SIZE] = {0};
    while (!feof(coded))
    {
        c = fgetc(coded);
        // Concatenação do caracter decodificado com o texto.
        // Por não ser possível concatenar string com um caracter usando strcat, criamos uma string temporária.
        char temp[2] = {c, '\0'};
        strcat(string, temp);
    }
    fclose(coded);

    FILE *decoded = fopen("decodificado.txt", "w+");
    char comparation[SIZE] = {0}; // Armazena uma codificação até encontrar uma compatível.
    for (int i = 0; i < strlen(string) - 1; i++)
    {
        // Novamente, é necessário criar uma string temporária para poder concatená-la ao texto decodificado.
        char temp[2] = {string[i], '\0'};
        strcat(comparation, temp);
        for (int j = 0; j < uniqueCharacters; j++)
        {
            // Caso a string comparation seja igual a uma das codificações válidas:
            if (strcmp(codedCharList[j].codification, comparation) == 0)
            {
                // Adiciona o caracter da tal codificação no arquivo decodificado.
                fputc(codedCharList[j].character, decoded);
                // Limpa a string de comparação para recomeçar a busca por uma nova codificação válida.
                strcpy(comparation, "");
            }
        }
    }
    fclose(decoded);
}
