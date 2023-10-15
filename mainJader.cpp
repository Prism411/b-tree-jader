#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "treeOnDisk.h"
#include "treeOnDisk.cpp"

void clear() {
    #ifdef _WIN32
        system("cls"); 
    #elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif
}

int main() {
    // Criar uma nova �rvore B
    BTree* tree = createBTree();

    // Chaves de exemplo para inserir na �rvore
    char* names[] = {"Alice", "Bob", "Cathy", "David", "Ethan", "Fred", "George"};
    int rnns[] = {20, 5, 10, 3, 30, 40, 50};

    // Inserir chaves na �rvore
    for (int i = 0; i < 7; i++) {
        Key k;
        strcpy(k.name, names[i]);
        k.rnn = rnns[i];
        insert(tree, k);
        printf("Inserindo: %s, %d\n", k.name, k.rnn);
    }

    // Procurar uma chave na �rvore
    char nameToSearch[] = "Cathy";
    printf("\nBuscando pela chave: %s\n", nameToSearch);
    BTreeNode* result = search(tree->root, nameToSearch);
    if (result != NULL) {
        printf("A chave '%s' foi encontrada!\n", nameToSearch);
    } else {
        printf("A chave '%s' n�o foi encontrada.\n", nameToSearch);
    }

    // Percorrer a �rvore e imprimir as chaves
    printf("\nTravessia da �rvore B: ");
    traverse(tree->root);
    printf("\n");

    // Quando terminar com a �rvore, libere a mem�ria alocada
    freeBTree(tree);

    return 0;
}
/*
    char buffer[50];
    char method[3];
    char login[50], modality[50], gender[2];

    clear();

    while(1) {

        fflush(stdin);
        fgets(buffer, sizeof(buffer), stdin);

        sscanf(buffer, "%s", method);

        if (strcmp(method, "IC") == 0) {

            sscanf(buffer, "%s %s %s %s", method, login, modality, gender);
            printf("output: %s %s %s %s\n", method, login, modality, gender);

        } else if(strcmp(method, "BM") == 0) {

            sscanf(buffer, "%s %s", method, modality);
            printf("output: %s %s\n", method, modality);

        } else if(strcmp(method, "BS") == 0) {

            sscanf(buffer, "%s %s", method, gender);
            printf("output: %s %s\n", method, gender);

        } else if(strcmp(method, "BD") == 0) {

            sscanf(buffer, "%s %s %s", method, modality, gender);
            printf("output: %s %s %s\n", method, modality, gender);

        } else if(strcmp(method, "PR") == 0) {

            // 
            printf("output: %s\n", method);

        } else if(strcmp(method, "FM") == 0) {

            //
            printf("output: %s\n", method);

        } else {
            printf("error: invalid input\n");
        }

        memset(buffer,   0, sizeof(buffer));
        memset(method,   0, sizeof(method));
        memset(login,    0, sizeof(login));
        memset(modality, 0, sizeof(modality));
        memset(gender,   0, sizeof(gender));
    }
*/

