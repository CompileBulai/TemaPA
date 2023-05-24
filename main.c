#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Player
{
    char *firstName;
    char *secondName;
    int points;
};
typedef struct Player Player;

struct Team
{
    char *name;
    int teamSize;
    struct Player *players;
    float teamPoints;
    struct Team *next;
};

struct List
{
    int numTeams;
    struct Team *teams;
};
typedef struct Team Team;
typedef struct List List;

struct Q
{
    Team *front, *rear;
};
void freeTeam(Team *t)
{
    Team *aux;
    aux = t;

    while (aux != NULL)
    {
        t = aux;
        free(t->name);
        for (int i = 0; i < t->teamSize; i++)
        {
            free(t->players[i].firstName);
            free(t->players[i].secondName);
        }
        free(t->players);
        aux = aux->next;
        free(t);
    }
}
typedef struct Q Queue;
void increasePoints(Team *castigatoare);
void displayCastigatori(Team *castigatori, FILE *a, int round_number);
void displayWinners(Team *castigatori, FILE *a);
Queue *creeazaQueueMeciurilor()
{
    Queue *q;

    q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL)
        return NULL;
    q->front = q->rear = NULL;
    return q;
}
void enQueue(Queue *q, Team *v)
{ // printf("Crapa 1");
    Team *newNode = (Team *)malloc(sizeof(Team));
    *newNode = *v;
    // printf("Crapa 2");
    newNode->next = NULL;
    // daca nu exista niciun nod in coada
    if (q->rear == NULL)
    {

        q->rear = newNode;
        q->front = newNode;
        // nodurile noi se adauga la finalul cozii
    }
    else
    {
        (q->rear)->next = newNode;
        q->rear = newNode;
    }

    // printf(" %s \n",q->rear->name );
}
int isEmpty(Queue *q)
{
    return (q->front == NULL);
}
void deleteQueue(Queue *q)
{
    while (!isEmpty(q))
    {

        Team *deletedTeam = q->front;
        q->front = q->front->next;
        if (q->front == NULL)
        {
            q->rear = NULL;
        }
        deletedTeam->next = NULL;
        free(deletedTeam);
    }
}
void push(Team **top, Team *v)
{
    Team *newNode = (Team *)malloc(sizeof(Team));
    *newNode = *v;
    newNode->next = *top;
    *top = newNode;
}
void deleteStack(Team **top)
{
    Team *temp;
    while ((*top) != NULL)
    {
        temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}

void citireDate(List *L, FILE *f)
{

    Team *head;
    int x = 0;
    Team *aux;
    char aux_spatiu;
    fscanf(f, "%d", &L->numTeams);
    // printf("%d ",L->numTeams);
    for (int i = 0; i < L->numTeams; i++)
    {
        aux = (Team *)malloc(sizeof(Team));
        /*if(x==0)
        {
            head = aux;
            L->teams = head;
        }*/

        fscanf(f, "%d", &aux->teamSize);

        // printf("%d", aux->teamSize);
        fscanf(f, "%c", &aux_spatiu);
        aux->name = (char *)malloc(50 * sizeof(char));
        fgets(aux->name, 50, f);
        strcpy((aux->name + strlen(aux->name) - 1), (aux->name + strlen(aux->name)));
        if (aux->name[strlen(aux->name) - 2] == ' ')
        { // printf("%s \n",aux->name);
            aux->name[strlen(aux->name) - 2] = aux->name[strlen(aux->name) - 1];
            aux->name[strlen(aux->name) - 1] = '\0';
            // printf("%s \n",aux->name);
            // printf("Aici intra?");
        }

        aux->players = (Player *)malloc(aux->teamSize * sizeof(Player));
        aux->next = NULL;
        aux->teamPoints = 0;
        // printf("%f \n", aux->teamPoints);

        for (int j = 0; j < aux->teamSize; j++)
        {
            aux->players[j].firstName = (char *)malloc(50 * sizeof(char));
            fscanf(f, "%s", aux->players[j].firstName);
            aux->players[j].secondName = (char *)malloc(50 * sizeof(char));
            fscanf(f, "%s", aux->players[j].secondName);
            fscanf(f, "%d", &aux->players[j].points);
            // printf("%s %s %d \n", aux->players[j].firstName, aux->players[j].secondName, aux->players[j].points);
            aux->teamPoints = aux->teamPoints + aux->players[j].points;
        }
        aux->teamPoints = aux->teamPoints / aux->teamSize;

        if (x == 0)
        {
            head = aux;
            L->teams = head;
            x = 1;
        }
        else
        {
            aux->next = head;
            head = aux;
        }
    }

    L->teams = head;
}
void stergere_elemente(Team **head, float minim_lista)
{
    Team *temp = *head, *prev; // pentru refacerea legaturilor
    // Daca primul nod este un minim
    if (temp != NULL && temp->teamPoints == minim_lista)
    {
        *head = temp->next;
        free(temp);
        return;
    }
    // In caz de elementul de sters are alta pozitie
    while (temp != NULL && temp->teamPoints != minim_lista)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;

    prev->next = temp->next;
    free(temp->name);
    for (int i = 0; i < temp->teamSize; i++)
    {
        free(temp->players[i].firstName);
        free(temp->players[i].secondName);
    }
    free(temp->players);
    free(temp);
}

void reducere_echipe(List *L, int numar_limita)
{

    Team *aux, *curent;

    while (L->numTeams != numar_limita)
    {
        aux = L->teams;
        float minim_lista = aux->teamPoints;

        curent = aux->next;

        while (curent != NULL)
        { // printf("minimul are valoare de %.3f la pas si punctajul echipei la pas %.3f \n", minim_lista,curent ->teamPoints);

            if (curent->teamPoints < minim_lista)
            {

                minim_lista = curent->teamPoints;
                curent = curent->next;
            }
            else
                curent = curent->next;
        }
        // printf("Inainte de functie");

        stergere_elemente(&L->teams, minim_lista);

        L->numTeams--;
        // printf("%d\n", L->numTeams);
    }

    // printf("%d", L->numTeams);
}
void distribution(Team *parcurgere, Team **invinsi, Team **castigatori)
{ // printf("Declarare ok?");
    if (parcurgere->teamPoints <= parcurgere->next->teamPoints)
    {

        // printf("Cazul 1 inainte\n");

        push(invinsi, parcurgere);
        // printf("%.3f\n", parcurgere->next->teamPoints);
        increasePoints(parcurgere->next);
        // printf("%.3f\n", parcurgere->next->teamPoints);

        push(castigatori, parcurgere->next);

        // printf("Cazul 1 dupa\n");
    }
    else
    { // printf("Cazul 2 inainte\n");
        increasePoints(parcurgere);
        push(castigatori, parcurgere);
        push(invinsi, parcurgere->next);
        // printf("Cazul 2 dupa\n");
    }
}

void show_match(Queue *q, Team **invinsi, Team **castigatori, FILE *a, int *round_number)
{
    int i = 1;
    fprintf(a, "\n");
    fprintf(a, "--- ROUND NO:%d\n", *round_number);
    Team *parcurgere = q->front;
    // printf("%s", q->front->name);
    while ((parcurgere) != NULL)
    {
        if (*round_number == 1)
        {
            parcurgere->name[strlen(parcurgere->name) - 1] = '\0';
            parcurgere->next->name[strlen(parcurgere->next->name) - 1] = '\0';
        }

        fprintf(a, "%s ", parcurgere->name);

        for (int j = 0; j < 32 - strlen(parcurgere->name); j++)
            fprintf(a, " ");
        fprintf(a, "-");
        for (int j = 0; j < 32 - strlen(parcurgere->next->name); j++)
            fprintf(a, " ");
        fprintf(a, " %s", parcurgere->next->name);
        fprintf(a, "\n");
        distribution(parcurgere, invinsi, castigatori);
        parcurgere = parcurgere->next->next;
        // printf("Iese din functie!!\n");
        // printf("%d \n", i);
        i++;
    }
    fprintf(a, "\n");
    displayCastigatori(*castigatori, a, *round_number);
    (*round_number)++;
}
void increasePoints(Team *castigatoare)
{
    for (int i = 0; i < castigatoare->teamSize; i++)
    {
        castigatoare->players[i].points = castigatoare->players[i].points + 1;
    }
    castigatoare->teamPoints = castigatoare->teamPoints + 1;
}
void afisare(Team *castigatori, FILE *a)
{

    fprintf(a, "%s", castigatori->name);

    for (int j = 0; j < 34 - strlen(castigatori->name); j++)
        fprintf(a, " ");
    fprintf(a, "-");
    for (int j = 0; j < 2; j++)
        fprintf(a, " ");

    fprintf(a, "%.2f\n", castigatori->teamPoints);
}
void displayCastigatori(Team *castigatori, FILE *a, int round_number)
{
    fprintf(a, "WINNERS OF ROUND NO:%d\n", round_number);
    displayWinners(castigatori, a);
}
void displayWinners(Team *castigatori, FILE *a)
{
    if (castigatori == NULL)
        printf("E NUiL LLLLLL\n\n\n");
    while (castigatori != NULL)
    {
        afisare(castigatori, a);

        castigatori = castigatori->next;
    }
}

void matchQueue(Queue *q, Team *t)
{
    while (t != NULL)
    {
        enQueue(q, t);
        t = t->next;
    }
}
void last8(Team **newList, Team *castigatori)
{

    int x = 0;
    // printf("INTRA AICI?");
    Team *head = NULL;
    Team *aux = castigatori;
    while (aux != NULL) // AUX ESTE DOAR PENTRU VALOARE
    {                   // printf("Intrat!\n");
        Team *stocare = (Team *)malloc(sizeof(Team));
        if (x == 0)
        {
            *stocare = *aux;
            stocare->next = NULL;
            head = stocare;
            // printf("Crapa aici?");
            x = 1;
        }
        else
        {
            *stocare = *aux;
            stocare->next = head;
            head = stocare;
            // printf("Sau poate aici\n");
        }
        // printf("%s\n",head->name);
        aux = aux->next;
    }
    *newList = head;
}
struct BST
{

    Team *team;
    struct BST *left, *right;
};
typedef struct BST BST;

BST *newNode(Team *t)
{ // printf("Pe aici?\n");
    BST *node = (BST *)malloc(sizeof(BST));
    node->team = t;
    node->left = node->right = NULL;
    return node;
}
BST *insert(BST *node, Team *key)
{
    int comparare = 0;
    // printf("Intra aici??\n");
    // daca (sub)arborele este gol, creeaza nod
    if (node == NULL)
        return newNode(key);

    if (key->teamPoints < node->team->teamPoints)
    { // printf("Trecut nod stang\n");
        node->left = insert(node->left, key);
    }

    else if (key->teamPoints > node->team->teamPoints)
    { // printf("trecut nod drept\n");
        node->right = insert(node->right, key);
    }
    else if (key->teamPoints = node->team->teamPoints)
    { // printf("NU CRED CA A INTRAT AICI");
        comparare = strcmp(key->name, node->team->name);
        if (comparare >= 0)
            node->right = insert(node->right, key);
        else
            node->left = insert(node->left, key);
    }
    return node;
}

void traverseInorder(BST *root, FILE *a)
{
    if (root == NULL)
        return;
    traverseInorder(root->right, a);
    afisare(root->team, a);
    traverseInorder(root->left, a);
}

void displayWinners1(BST *root, FILE *a)
{
    fprintf(a, "\n");
    fprintf(a, "TOP 8 TEAMS:\n");
    // displayWinners(t,a);
    traverseInorder(root, a);
}
/// Cerinta 5
int max(int a, int b)
{
    return (a > b) ? a : b;
}
struct AVL
{

    int height;
    Team *team;
    struct AVL *left, *right;
};
typedef struct AVL AVL;
int height(AVL *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

AVL *rightRotation(AVL *y)
{
    AVL *x = y->left;
    AVL *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

AVL *leftRotation(AVL *x)
{
    AVL *y = x->right;
    AVL *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}
int getBalance(AVL *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

AVL *avlInsert(AVL *node, Team *key)
{
    // printf("Intra aici??\n");
    //  printf("%s ............%.2f\n", key->name, key->teamPoints);
    int comparare = 0;

    if (node == NULL)
    {
        node = (AVL *)malloc(sizeof(AVL));
        node->team = key;
        node->height = 1;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    comparare = strcmp(key->name, node->team->name);
    if (key->teamPoints < node->team->teamPoints)
    {
        // printf("Trecut nod stang\n");
        node->left = avlInsert(node->left, key);
    }

    else if (key->teamPoints > node->team->teamPoints)
    {
        // printf("trecut nod drept\n");
        node->right = avlInsert(node->right, key);
    }
    else if (key->teamPoints == node->team->teamPoints)
    {
        // printf("NU CRED CA A INTRAT AICI");
        // printf("%s\n", key->name);
        // printf("%s", node->team->name);
        comparare = strcmp(key->name, node->team->name);
        if (comparare >= 0)
            node->right = avlInsert(node->right, key);
        else
            node->left = avlInsert(node->left, key);
    }

    // printf("cheie ....%s\n", key->name);
    // printf("nod curent .....%s \n", node->team->name);

    /// REECHILIBRARI;
    node->height = 1 + max(height(node->left), height(node->right));
    // printf("Ajunge pana aici???");
    int k = getBalance(node);

    // LL == y este in stanga;
    if (k > 1 && ((key->teamPoints < node->left->team->teamPoints) || (key->teamPoints == node->left->team->teamPoints && strcmp(key->name, node->left->team->name) < 0)))
    {
        return rightRotation(node);
        // printf("ROTIRE DREAPTA\n");
    }

    // RR case == y este in dreapta == k< -1
    if (k < -1 && ((key->teamPoints > node->right->team->teamPoints) || (key->teamPoints == node->right->team->teamPoints && strcmp(key->name, node->right->team->name) > 0)))
    {
        // printf("ROTIRE STANGA\n");
        return leftRotation(node);
    }
    // LR Case == y este in stanga

    if (k > 1 && ((key->teamPoints > node->left->team->teamPoints) || (key->teamPoints == node->left->team->teamPoints && strcmp(key->name, node->left->team->name) > 0)))
    { // printf("DIN NOU AICI %s\n", node->team->name);
        // printf("ROTIREA STANGA-DREAPTA\n");
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }

    // RL Case == Y este in dreapta
    if (k < -1 && ((key->teamPoints < node->right->team->teamPoints) || (key->teamPoints == node->right->team->teamPoints && strcmp(key->name, node->right->team->name) < 0)))

    { // printf("DIN NOU AICI %s", node->team->name);
      // printf("ROTIREA DREAPTA-STANGA\n");
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }
    // printf("PANA ACUMA FARA SCHIMBARI!!!\n");

    return node; // returneaza nodul nemodificat
}
void insertInorderAVL(BST *root, AVL **root1)
{
    if (root == NULL)
        return;
    insertInorderAVL(root->right, root1);
    *root1 = avlInsert(*root1, root->team);
    insertInorderAVL(root->left, root1);
}

void printNodesAtLevel(AVL *root, int currentLevel, int level, FILE *a)
{

    if (root == NULL)
    {
        return;
    }
    if (currentLevel == level)
    {
        fprintf(a, "%s\n", root->team->name);
        return;
    }
    printNodesAtLevel(root->right, currentLevel + 1, level, a);
    printNodesAtLevel(root->left, currentLevel + 1, level, a);
}
/// Eliberari de memorie
void freeBST(BST *root)
{
    if (root == NULL)
        return;
    else
    {
        freeBST(root->left);
        freeBST(root->right);
        free(root);
    }
}
void freeAVL(AVL *root1)
{

    if (root1 == NULL)
        return;
    else
    {
        freeAVL(root1->left);
        freeAVL(root1->right);
        free(root1);
    }
}
void freePseudoList(Team *t)
{
    Team *aux = t;
    while (aux != NULL)
    {
        t = aux;
        aux = aux->next;
        free(t);
    }
}
int main(int argc, char *argv[])
{
    Team *invinsi = NULL, *head2 = NULL;
    Team *castigatori = NULL;
    List *L;
    FILE *f, *a, *c;
    Team *winners = NULL;
    BST *root = NULL;
    AVL *root1 = NULL;
    Queue *q;
    int v[5];
    // cerinta 1;
    c = fopen(argv[1], "r");
    f = fopen(argv[2], "r");
    a = fopen(argv[3], "w");
    for (int i = 0; i < 5; i++)
    {
        fscanf(c, "%d", &(v[i]));
    }
    fclose(c);
    if (v[0] == 1)
    {
        //////// cerinta1 ////////
        L = (List *)malloc(sizeof(List));

        citireDate(L, f);
        head2 = L->teams;
    }

    if (v[1] == 1)
    {
        //////// cerinta2 ////////
        int numar_limita = 1;
        while (numar_limita < L->numTeams)
        {
            numar_limita = numar_limita * 2;
        }
        if (numar_limita > L->numTeams)
            numar_limita = numar_limita / 2;

        // L->teams = head2;

        reducere_echipe(L, numar_limita);
        head2 = L->teams;

        // L->teams = head2;
    }
    while (L->teams != NULL)
    {
        fprintf(a, "%s\n", L->teams->name);
        L->teams = L->teams->next;
    }

    /////////////////cerinta 3//////////////;
    if (v[2] == 1)
    {
        // creeaza coada;
        L->teams = head2;
        q = creeazaQueueMeciurilor();
        matchQueue(q, L->teams);

        int i = 1;
        while (L->numTeams != 1)
        {
            show_match(q, &invinsi, &castigatori, a, &i);
            if (L->numTeams == 16)
                last8(&winners, castigatori);

            deleteQueue(q);
            matchQueue(q, castigatori);
            deleteStack(&invinsi);
            deleteStack(&castigatori);
            L->numTeams = L->numTeams / 2;
        }
    }
    if (v[3] == 1)
    {
        /////////////////cerinta 4//////////////;

        while (winners != NULL)
        {
            root = insert(root, winners);
            winners = winners->next;
        }

        displayWinners1(root, a);
    }

    if (v[4] == 1)
    {
        /////////////////cerinta 5//////////////;
        insertInorderAVL(root, &root1);

        fprintf(a, "\nTHE LEVEL 2 TEAMS ARE:\n");
        printNodesAtLevel(root1, 0, 2, a);
    }

    fclose(f);
    fclose(a);
    // eliberari de memorie
    deleteQueue(q);
    free(q);
    freePseudoList(winners);
    freeTeam(L->teams);
    free(L);
    return 0;
}
