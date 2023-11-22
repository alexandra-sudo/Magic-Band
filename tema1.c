/*CIOBANU Alexandra 314CD*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 255

typedef char Item;
/***********************************COADA***************************************/
typedef struct Node {
    char *value;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

/**************************************BANDA************************************/
/* lista dublu inlantuita */
typedef struct BandaNode{
    Item value; // valoarea nodului
    struct BandaNode* left; // link catre nodul stang
    struct BandaNode* right; // link catre nodul drept
} BandaNode;

/* lista simplu inlantuita */
typedef struct Banda{
    BandaNode* santinel; // link catre santinela
    BandaNode* finger; // link catre adresa celulei in care se afla degetul
} Banda;

/* crearea nodului */
BandaNode* createNode(Item value) {
    BandaNode* node = calloc(1, sizeof(BandaNode));
    node->value = value;
    node->right = NULL;
    node->left = NULL;
    return node;
}

/* crearea benzii */
Banda* createBanda(BandaNode* node) {
    Banda* band = calloc(1, sizeof(Banda));
    band->finger = node;
    band->santinel = createNode(' ');
    band->santinel->right = band->finger;
    node->left = band->santinel;
    node->right = NULL;
    return band;
}

/*****************************COADA***********************************/
// crearea unui nod nou pentru coada
Node *create_node(char *value) {
    Node *node = calloc(1, sizeof(Node));
    node->value = calloc(20, sizeof(char));
    strcpy(node->value, value);

    return node;
}

// crearea cozii
Queue *create_queue (void) {
    Queue *q = calloc(1, sizeof(Queue));
    q->front = q->rear = NULL;

    return q;
}

/* Adaugare element in coada */
void enQueue(Queue* q, char *value)
{
    // crearea unui nod nou
    Node* temp = create_node(value);
 
    // daca coada e goala nodul devine 
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
 
    q->rear->next = temp;
    q->rear = temp;
}
 
/* Stergere nod din coada */
char* deQueue(struct Queue* q)
{
    char *ret = calloc(20, sizeof(char));
    
    if (q->front == NULL)
        return NULL;
 
    Node* temp = q->front;
    strcpy(ret, q->front->value);
 
    q->front = q->front->next;
 
    if (q->front == NULL)
        q->rear = NULL;
 
    free(temp->value);
    free(temp);

    return ret;
}

/*************************OPERATII DE TIP UPDATE**************************/ 

/* operatia de deplasare cu o pozitie la stanga */
void MOVE_LEFT(Banda* band) {
    if (band->finger->left != band->santinel) {
        band->finger = band->finger->left;
    }
}

/* operatia de deplasare cu o pozitie la dreapta */
void MOVE_RIGHT(Banda* band) {
    if (band->finger->right == NULL) {
        BandaNode* node = createNode('#');
        node->left = band->finger;
        node->right = NULL;
        band->finger->right = node;
    }
    band->finger = band->finger->right;
}

/* operatia de deplasare la stanga pana la primul caracter egal cu un anumit simbol */
void MOVE_LEFT_CHAR(Banda* band, char c, FILE *f) {
    if(band->finger->left == band->santinel) {
        fprintf(f, "ERROR\n");
    }
    BandaNode *finger_old = band->finger;
    while (band->finger != band->santinel) {
        
        if (band->finger->value == c) {
            return;
        }
        band->finger = band->finger->left;
    }
    band->finger = finger_old;
}

/* operatia de deplasare la dreapta pana la primul caracter egal cu un anumit simbol */
void MOVE_RIGHT_CHAR(Banda* band, char c) {
    if (band->finger->value == c) {
        return;
    }
    while (band->finger->right != NULL) {
        if (band->finger->right->value == c) {
            return;
        }
        band->finger = band->finger->right;
    }

    BandaNode* node = createNode('#');
    band->finger->right = node;
    band->finger = node;
}
/* operatia de actualizare a caracterului curent */
void WRITE(Banda* band, char c) {
    band->finger->value = c;
}

/* operatia de inserare la stanga */
void INSERT_LEFT(Banda* band, char c, FILE *f) {

    if (band->finger->left == band->santinel) {
        fprintf(f, "ERROR\n");
        return;
    }

    /* crearea unui nod cu simbolul specificat */
    BandaNode* node = createNode(c);

    /* actualizarea legaturilor intre noduri */
    node->right = band->finger;
    node->left = band->finger->left;
    band->finger->left->right = node;
    band->finger->left = node;
    band->finger = band->finger->left;

}

void INSERT_RIGHT(Banda* band, char c) {

    /* crearea unui nod cu simbolul specificat */
    BandaNode* node = createNode(c);

    if(band->finger->right ==  NULL) {
        node->left = band->finger;
        node->right = NULL;
        band->finger->right = node;
        band->finger = node;
        return;
    }

    /* actualizarea legaturilor intre noduri */
        node->left = band->finger;
        node->right = band->finger->right;
        band->finger->right->left = node;
        band->finger->right = node;
        band->finger = node;
}

/********************OPERATII DE TIP QUERY**************************/

/* determinarea caracterului de pe pozitia curenta */
void SHOW_CURRENT(Banda* band, FILE *f) {
    fprintf(f,"%c\n", band->finger->value);
}

/* afisare continut banda */
void SHOW(Banda* band, FILE *f) {
    BandaNode* node = band->santinel->right;
    while(node != NULL) {
        if (node == band->finger) {
            fprintf(f, "|%c|", band->finger->value);
        } else {
            fprintf(f, "%c", node->value);
        }
        node = node->right;
    }
    fprintf(f, "\n");
}

int main() {
    int nr_com;
    FILE* f = fopen("tema1.in", "r");
    FILE* f_out = fopen("tema1.out", "w");
    
    BandaNode* pon = createNode('#');
    Banda* band = createBanda(pon);
    char* buffer = malloc(20*sizeof(char));

    Queue *q = create_queue();

    fscanf(f, "%d", &nr_com);

    for (int i = 0; i <= nr_com; i++) {
        fgets(buffer, 20, f);
        if (strstr(buffer, "MOVE_RIGHT_CHAR") != NULL) {
            enQueue(q, buffer);
        } 
        else if (strncmp(buffer, "MOVE_RIGHT", 10) == 0) {
            enQueue(q, buffer);
        } 
        else if (strstr(buffer, "MOVE_LEFT_CHAR") != NULL) {
            enQueue(q, buffer);
        }
        else if (strncmp(buffer, "MOVE_LEFT", 9) == 0) {
            enQueue(q, buffer);
        }  else if (strstr(buffer, "WRITE") != NULL) {
            enQueue(q, buffer);
        } else if (strstr(buffer, "INSERT_LEFT") != NULL) {
            enQueue(q, buffer);
        } else if (strstr(buffer, "INSERT_RIGHT") != NULL) {
            enQueue(q, buffer);
        } else if (strncmp(buffer, "EXECUTE", 7) == 0) {
            char* ret = deQueue(q);
            if (strstr(ret, "MOVE_RIGHT_CHAR") != NULL) {
                char *token = calloc(20, sizeof(char));
                token = strtok(ret, " ");
                token = strtok(NULL, " ");
                MOVE_RIGHT_CHAR(band, token[0]);
            }
            else if (strncmp(ret, "MOVE_RIGHT", 10) == 0) {
                MOVE_RIGHT(band);
            } 
            else if (strstr(ret, "MOVE_LEFT_CHAR") != NULL) {
                char *token = calloc(20, sizeof(char));
                token = strtok(ret, " ");
                token = strtok(NULL, " ");
                MOVE_LEFT_CHAR(band, token[0], f_out);
            }
            else if (strncmp(ret, "MOVE_LEFT", 9) == 0) {
                MOVE_LEFT(band);
            } 
            else if (strstr(ret, "WRITE") != NULL) {
                char *token = calloc(20, sizeof(char));
                token = strtok(ret, " ");
                token = strtok(NULL, " ");
                WRITE(band, token[0]);
            } 
            else if (strstr(ret, "INSERT_LEFT") != NULL) {
                char *token = calloc(20, sizeof(char));
                token = strtok(ret, " ");
                token = strtok(NULL, " ");
                INSERT_LEFT(band, token[0], f);
            }
            else if (strstr(ret, "INSERT_RIGHT") != NULL) {
                char *token = calloc(20, sizeof(char));
                token = strtok(ret, " ");
                token = strtok(NULL, " ");
                INSERT_RIGHT(band, token[0]);
            }
        }
        else if(strncmp(buffer, "SHOW_CURRENT", 12) == 0) {
            SHOW_CURRENT(band, f_out);
        }
        else if (strncmp(buffer, "SHOW", 4) == 0) {
            SHOW(band, f_out);
        }
    }
    return 0;
}