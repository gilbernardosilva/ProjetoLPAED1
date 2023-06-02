#include "projeto.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer"


/* Parte do Algoritmo Genético */

void init_gene(gene *g, int x, int y, int id, char name[20])
{
    g->x = x;
    g->y = y;
    g->id = id;
    strcpy(g->name, name);
}

void init_chromosome(chromosome *c, int size)
{
    c->csize = size;
    c->gene_array = (gene *) calloc(size, sizeof(gene));
    c->fitness = 0.0f;
    gene *ptr = NULL;
    ptr = c->gene_array;
    int id1, x1, y1, i = 0;
    char name[20];
    FILE *fp = fopen("cities.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open the file.");
        exit(1);
    }
    while (i < size)
    {
        fscanf(fp, "%d%*c%d%*c%d%*c%s", &id1, &x1, &y1, name);
        init_gene(ptr, x1, y1, id1, name);
        i++;
        ptr++;
    }
    shuffle_chromosome(c);
    fclose(fp);
}



void init_population(population *p, int psize)
{
    p->psize = psize;
    p->chromosome_array = (chromosome *) calloc(psize, sizeof(chromosome));
    for (int i = 0; i < psize; i++)
    {
        init_chromosome(&p->chromosome_array[i], CHROMOSOME_SIZE);
    }

}

double calculate_fitness(chromosome *c)
{
    double distance = 0.0f;
    for (int i = 0; i < c->csize - 1; i++)
    {
        distance += (pow(c->gene_array[i].x - c->gene_array[i + 1].x, 2) +
                     (pow(c->gene_array[i].y - c->gene_array[i + 1].y, 2)));

    }
    double fitness = 1.0 / sqrt(distance);
    return fitness;
}

void evaluatePop(population *p)
{
    double totalfitness=0;
    for (int i = 0; i < p->psize; i++)
    {
        p->chromosome_array[i].fitness = calculate_fitness(&p->chromosome_array[i]);
        totalfitness=totalfitness+ calculate_fitness(&p->chromosome_array[i]);
    }
    p->totalfitness=totalfitness;

}

void totalPop(population *p){
    double totalfitness;
    for (int i = 0; i < p->psize; i++) {
        totalfitness=totalfitness+ calculate_fitness(&p->chromosome_array[i]);
    }
    p->totalfitness=totalfitness;
}


void doseed(void) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    unsigned seed = (unsigned) tp.tv_usec;
    srand(seed);
}

double randomDouble(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

int randomInt(int min, int max)
{
    int number;
    number = (rand() % (max - min + 1)) + min;
    return number;
}

void swapGene(chromosome *c, int i, int j)
{
    int id1, x1, y1;
    char name[20];

    id1 = c->gene_array[j].id;
    x1 = c->gene_array[j].x;
    y1 = c->gene_array[j].y;
    strcpy(name, c->gene_array[j].name);

    c->gene_array[j].id = c->gene_array[i].id;
    c->gene_array[j].x = c->gene_array[i].x;
    c->gene_array[j].y = c->gene_array[i].y;
    strcpy(c->gene_array[j].name, c->gene_array[i].name);

    c->gene_array[i].id = id1;
    c->gene_array[i].x = x1;
    c->gene_array[i].y = y1;
    strcpy(c->gene_array[i].name, name);
}

void swapChromosome(population *p, int a, int b)
{
    chromosome *c1 = (chromosome *) calloc(1, sizeof(chromosome));
    init_chromosome(c1, CHROMOSOME_SIZE);

    c1->gene_array = p->chromosome_array[a].gene_array;
    c1->fitness = p->chromosome_array[a].fitness;

    p->chromosome_array[a].gene_array = p->chromosome_array[b].gene_array;
    p->chromosome_array[a].fitness = p->chromosome_array[b].fitness;

    p->chromosome_array[b].gene_array = c1->gene_array;
    p->chromosome_array[b].fitness = c1->fitness;

}

void matchcities(chromosome *c)
{
    gene *ptr = NULL;
    ptr = c->gene_array;

    strcpy((ptr + c->csize - 1)->name, ptr->name);
    (ptr + c->csize - 1)->x = ptr->x;
    (ptr + c->csize - 1)->y = ptr->y;
    (ptr + c->csize - 1)->id = ptr->id;
}

void shuffle_chromosome(chromosome *c)
{
    for (int i = c->csize - 1; i >= 1; i--)
    {
        int j = rand() % (i + 1);
        swapGene(c, i, j);
    }
    matchcities(c);
}

int roullete_wheel(population *p)
{
    int idx = 0;
    double random, TotalFitness = 0;

    for (int i = 0; i < p->psize; i++)
    {
        TotalFitness = TotalFitness + p->chromosome_array[i].fitness;
    }
    random = ((double) rand() / (double) RAND_MAX * TotalFitness);
    for (int j = 0; j < p->psize - 1 && random > 0; j++)
    {
        random -= p->chromosome_array[j].fitness;
        idx++;
    }
    return idx;
}

void apply_mutation(population *p)
{
    int ch1, ch2, index, index1;

    do
    {
        ch1 = randomInt(1, CHROMOSOME_SIZE - 1);
        ch2 = randomInt(1, CHROMOSOME_SIZE - 1);
        index = randomInt(0, POP_SIZE - 1);
        index1 = randomInt(0, POP_SIZE - 1);
    } while (ch2 == ch1 && index == index1);

    if (randomDouble(0, 1) < MUTATION)
    {
        swapGene(&p->chromosome_array[index], ch1, ch2);
        swapGene(&p->chromosome_array[index1], ch1, ch2);
    }
}

void bubbleSort(population *p)
{
    for (int i = 0; i < p->psize - 1; i++)
    {
        for (int j = 0; j < p->psize - i - 1; j++)
        {
            if (p->chromosome_array[j].fitness > p->chromosome_array[j + 1].fitness)
                swapChromosome(p, j, j + 1);
        }
    }
}

int checkDuplicates(chromosome *c)
{
    for (int i = 0; i < c->csize; i++)
    {
        for (int j = i + 1; j < c->csize; j++)
        {
            if (c->gene_array[i].id == c->gene_array[j].id)
                return j;
        }
    }
    return 0;
}

void apply_selection(population *p)
{
    int auxArray[POP_SIZE - ELITISM_SIZE], dupChild1, dupChild2, id1, x1, y1, slice1, slice2;
    char name[20];
    bubbleSort(p);

    for (int x = 0; x < POP_SIZE - ELITISM_SIZE; x = x + 2)
    {
        do
        {
            auxArray[x] = roullete_wheel(p);
            auxArray[x + 1] = roullete_wheel(p);
        } while (auxArray[x] == auxArray[x + 1]);
    }

    chromosome *child1 = (chromosome *) calloc(1, sizeof(chromosome));
    chromosome *child2 = (chromosome *) calloc(1, sizeof(chromosome));
    init_chromosome(child1, CHROMOSOME_SIZE);
    init_chromosome(child2, CHROMOSOME_SIZE);

    FILE *fp = fopen("cities.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open the file.");
        exit(1);
    }


    do
    {
        slice1 = randomInt(1, CHROMOSOME_SIZE - 1);
        slice2 = randomInt(slice1, CHROMOSOME_SIZE - 1);
    } while (slice1 == slice2);


    for (int x = 0; x < POP_SIZE - ELITISM_SIZE; x = x + 2)
    {
        for (int a = slice1; a < slice2; a++)
        {
            child1->gene_array[a] = p->chromosome_array[auxArray[x]].gene_array[a];
            child2->gene_array[a] = p->chromosome_array[auxArray[x]].gene_array[a];
        }
        for (int k = 0; k < slice1; k++)
        {
            child1->gene_array[k] = p->chromosome_array[auxArray[x + 1]].gene_array[k];
            child2->gene_array[k] = p->chromosome_array[auxArray[x]].gene_array[k];
        }
        for (int k1 = slice2; k1 < CHROMOSOME_SIZE - 1; k1++)
        {
            child1->gene_array[k1] = p->chromosome_array[auxArray[x]].gene_array[k1];
            child2->gene_array[k1] = p->chromosome_array[auxArray[x + 1]].gene_array[k1];
        }

        dupChild1 = checkDuplicates(child1);
        rewind(fp);
        while (dupChild1 != 0)
        {
            fscanf(fp, "%d%*c%d%*c%d%*c%s", &id1, &x1, &y1, name);
            init_gene(&child1->gene_array[dupChild1], x1, y1, id1, name);
            dupChild1 = checkDuplicates(child1);
        }

        dupChild2 = checkDuplicates(child2);
        rewind(fp);
        while (dupChild2 != 0)
        {
            fscanf(fp, "%d%*c%d%*c%d%*c%s", &id1, &x1, &y1, name);
            init_gene(&child2->gene_array[dupChild2], x1, y1, id1, name);
            dupChild2 = checkDuplicates(child2);
        }

        matchcities(child1);
        matchcities(child2);

        for (int m = 0; m < CHROMOSOME_SIZE; m++)
        {
            p->chromosome_array[x].gene_array[m] = child1->gene_array[m];
            p->chromosome_array[x + 1].gene_array[m] = child2->gene_array[m];
        }
    }
    free(child1);
    free(child2);
}

void print(population *p, int size)
{
    gene *ptr = NULL;
    chromosome *ptr1 = NULL;
    ptr1 = p->chromosome_array;

    printf("Generation %d\n\n", p->generation);
    for (int i = 0; i < size; i++)
    {
        ptr = ptr1->gene_array;
        printf("Chromossoma numero %d\n", i + 1);
        for (int j = 0; j < p->chromosome_array->csize; j++)
        {
            printf("ID:%d, X:%d, Y:%d Name:%s", ptr->id, ptr->x, ptr->y, ptr->name);
            printf("\n");
            ptr++;
        }
        printf("Fitness: %f\n", ptr1->fitness);
        ptr1++;
    }
}

void printList(population *head)
{
    population *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        print(tmp, POP_SIZE);
    }
}

void freeList(population *head)
{
    population *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }

}

int searchbygeneration(population *head, int value)
{
    population *tmp=head;
    while (head != NULL)
    {
        if(tmp->generation==value){
            return printf("This generation ID exists");
            tmp=tmp->next;
        }
    }
    return printf("This generation ID doesn't exist");
}

population * InsertNodeAtTail(population *head, population* node){

    node->next=NULL;

    if(head==NULL){
        head=node;
        return head;
    }
    else{
        population *tmp=head;
        while(tmp->next!=NULL)
        {
            tmp=tmp->next;
        }
        tmp->next=node;
    }
}

population *InsertNodeAtHead(population *head, population* node){
    node->next=head;
    head=node;
    return head;
}


void init_ag()
{
    population *temp;
    population *aux = (population *) malloc(sizeof(population));
    population *head = (population *) malloc(sizeof(population));
    if (head == NULL || aux == NULL)
    {
        printf("Unable to allocate memory.");
        exit(0);
    }

    init_population(aux, POP_SIZE);
    init_population(head, POP_SIZE);

    head->generation = 0;
    head->next = NULL;
    temp = head;
    int generation = 1;
    evaluatePop(head);
    print(head, POP_SIZE);

    for (int i = 0; i < aux->psize; i++)
    {
        aux->chromosome_array[i] = head->chromosome_array[i];
    }

    for (int i = 0; i < MAX_GENERATIONS; i++)
    {
        population *newPop = (population *) malloc(sizeof(population));
        if (newPop == NULL)
        {
            printf("Unable to allocate memory.");
            exit(0);
        }
        init_population(newPop, POP_SIZE);
        newPop->generation=generation++;

        for (int j = 0; j < aux->psize; j++)
        {
            newPop->chromosome_array[j] = aux->chromosome_array[j];
        }

        newPop->next=NULL;
        temp->next=newPop;
        temp=temp->next;

        apply_mutation(newPop);
        apply_selection(newPop);
        evaluatePop(newPop);

        for (int j = 0; j < aux->psize; j++)
        {
            aux->chromosome_array[j] = newPop->chromosome_array[j];
        }

    }
    printList(head);
    free(aux);
    free(temp);
    freeList(head);


}


//_________________________________________ Parte do codigo_____________________________________________//


//_________________________________________________________clientes __________________________________________________________________________________//
LISTA_CLIENTES *create_lista_clientes()
{
    LISTA_CLIENTES *lc = (LISTA_CLIENTES *) calloc(1, sizeof(LISTA_CLIENTES));
    lc->pcliente = NULL;
    return lc;
}

void insert_cliente(LISTA_CLIENTES *lista_clientes, char nome[], int NIF, char morada_cliente[], int contacto, int IBAN,int data_nascimento, int data_registo)
{
    CLIENTES *clientes = (CLIENTES *) calloc(1, sizeof(CLIENTES));
    clientes->nome_cliente = (char *) malloc(sizeof(char) * strlen(nome) + 1);
    strcpy(clientes->nome_cliente, nome);
    clientes->NIF = NIF;
    clientes->morada_cliente = (char *) malloc(sizeof(char) * strlen(morada_cliente) + 1);
    strcpy(clientes->morada_cliente, morada_cliente);
    clientes->contacto = contacto;
    clientes->IBAN = IBAN;
    clientes->data_nascimento = data_nascimento;
    clientes->data_registo = data_registo;
    CLIENTES *pcl = lista_clientes->pcliente;
    if (pcl == NULL) {
        pcl = clientes->proximo;
        clientes->proximo = NULL;
    } else {
        clientes->proximo = pcl;
        pcl = clientes->proximo;
    }
    if (pcl == lista_clientes->pcliente) {
        clientes->proximo = lista_clientes->pcliente;
        lista_clientes->pcliente = clientes;
        lista_clientes->n_clientes++;
        return;
    } else {
        clientes->proximo = pcl;
        lista_clientes->n_clientes++;
    }
}

void insert_cliente_cauda(LISTA_CLIENTES *lista_clientes, char nome[], int NIF, char morada_cliente[], int contacto,int IBAN, int data_nascimento,int data_registo)
{
    CLIENTES *c = lista_clientes->pcliente;
    CLIENTES *clientes;
    clientes = (CLIENTES *) calloc(1, sizeof(CLIENTES));
    clientes->nome_cliente = (char *) malloc(sizeof(char) * strlen(nome) + 1);
    strcpy(clientes->nome_cliente, nome);
    clientes->NIF = NIF;
    clientes->morada_cliente = (char *) malloc(sizeof(char) * strlen(morada_cliente) + 1);
    strcpy(clientes->morada_cliente, morada_cliente);
    clientes->contacto = contacto;
    clientes->IBAN = IBAN;
    clientes->data_nascimento = data_nascimento;
    clientes->data_registo = data_registo;
    clientes->proximo = NULL;
    if (lista_clientes->pcliente == NULL) {
        lista_clientes->pcliente = clientes;
    } else {
        while (c->proximo != NULL) {
            c = c->proximo;

        }
        c->proximo = clientes;
    }
    lista_clientes->n_clientes++;

}

CLIENTES *encontrar_clientes(const LISTA_CLIENTES *listaClientes, int NIF)
{
    CLIENTES *c = listaClientes->pcliente;
    while (c != NULL) {
        if (c->NIF == NIF)return c;
        c = c->proximo;
    }
    return NULL;
}

void remove_cliente(LISTA_CLIENTES *lista_clientes, int NIF)
{
    CLIENTES *encontrar_cliente = encontrar_clientes(lista_clientes, NIF);
    if (encontrar_cliente == NULL) {
        printf("Cliente  nao encontrado\n");
        return;
    }
    CLIENTES *ppre = NULL;
    CLIENTES *pcur = lista_clientes->pcliente;
    while (pcur != encontrar_cliente && pcur != NULL) {
        ppre = pcur;
        pcur = pcur->proximo;
    }
    if (lista_clientes->pcliente == encontrar_cliente) {
        lista_clientes->pcliente = pcur->proximo;
        lista_clientes->n_clientes--;
        printf("%d %d\n", pcur->NIF);
        return;
    }
    if (lista_clientes->pcliente->NIF == encontrar_cliente) {
        lista_clientes->pcliente->proximo = pcur->proximo;
        lista_clientes->n_clientes--;
        return;
    }
    ppre->proximo = pcur->proximo;
    lista_clientes->n_clientes--;
}

void print_clientes(LISTA_CLIENTES *listaClientes)
{
    CLIENTES *aux = listaClientes->pcliente;
    printf("Numero de clientes %d\n", listaClientes->n_clientes);
    while (aux != NULL) {
        printf(" Nome: %s NIF: %d Contacto: %d Iban: %d Data Nascimento: %d Data Registo: %d \n",
               aux->nome_cliente,
               aux->NIF, aux->contacto, aux->IBAN, aux->data_nascimento, aux->data_registo);
        aux = aux->proximo;
    }
}

CLIENTES *encontrar_clientes_NIF(const LISTA_CLIENTES *listaClientes, int NIF)
{
    CLIENTES *c = listaClientes->pcliente;
    while (c != NULL) {
        if (c->NIF == NIF)
            printf("Nome: %s NIF: %d Contacto: %d Iban: %d Data Nascimento: %d Data Registo: %d \n",
                   c->nome_cliente,
                   c->NIF, c->contacto, c->IBAN, c->data_nascimento, c->data_registo);
        c = c->proximo;

    }
    return NULL;
}

CLIENTES *encontrar_clientes_Nome(const LISTA_CLIENTES *listaClientes,char nome[])
{
    CLIENTES *c = listaClientes->pcliente;
    while (c != NULL) {
        int iguais = strcmp(c->nome_cliente, nome);
        if (iguais == 0) {
            printf("Nome: %s NIF: %d Contacto: %d Iban: %d Data Nascimento: %d Data Registo: %d \n", c->nome_cliente,
                   c->NIF, c->contacto, c->IBAN, c->data_nascimento, c->data_registo);
            break;
        }
        c = c->proximo;
    }
    return NULL;
}

void bubbleSort_clientes(LISTA_CLIENTES *Head)
{
    int swapped;
    CLIENTES *ptr, *lptr, *aux;
    lptr = NULL;
    aux = Head->pcliente;
    if (aux == NULL)
        return;
    do {
        swapped = 0;
        ptr = aux;

        while (ptr->proximo != lptr) {
            if (ptr->NIF > ptr->proximo->NIF) {
                comparar_clientes(ptr, ptr->proximo);
                swapped = 1;
            }
            ptr = ptr->proximo;
        }
        lptr = ptr;
    } while (swapped);
}

void comparar_clientes(CLIENTES *a, CLIENTES *b)
{
    int temp = a->NIF;
    a->NIF = b->NIF;
    b->NIF = temp;
}

//_________________________________________________________cidades __________________________________________________________________________________//


LISTA_CIDADES *create_lista_cidades()
{
    LISTA_CIDADES *lc = (LISTA_CIDADES *) calloc(1, sizeof(LISTA_CIDADES));
    lc->p_cidade = NULL;
    return lc;
}

void insert_cidades(LISTA_CIDADES *lista_cidades, char nome[], int id_cidade, int cordx, int cordy, char informacao[],char poi[])
{
    CIDADES *cidades = (CIDADES *) calloc(1, sizeof(CIDADES));
    id_cidade++;
    cidades->id_cidade = id_cidade;
    cidades->nome_cidade = (char *) malloc(sizeof(char) * strlen(nome) + 1);
    strcpy(cidades->nome_cidade, nome);
    cidades->locx = cordx;
    cidades->locy = cordy;
    cidades->informacao = (char *) malloc(sizeof(char) * strlen(informacao) + 1);
    strcpy(cidades->informacao, informacao);
    cidades->poi = (char *) malloc(sizeof(char) * strlen(poi) + 1);
    strcpy(cidades->poi, poi);
    CIDADES *ppre = NULL;
    CIDADES *pcd = lista_cidades->p_cidade;
    if (pcd == NULL) {
        pcd = cidades->proximo;
        cidades->proximo = NULL;
    } else {
        cidades->proximo = pcd;
        pcd = cidades->proximo;
    }
    if (pcd == lista_cidades->p_cidade) {
        cidades->proximo = lista_cidades->p_cidade;
        lista_cidades->p_cidade = cidades;
        lista_cidades->n_cidades++;
        return;
    } else {
        cidades->proximo = pcd;
        lista_cidades->n_cidades++;
    }
}

CIDADES *encontrar_cidades(const LISTA_CIDADES *listaCidades, char nome[])
{
    CIDADES *c = listaCidades->p_cidade;
    while (c != NULL) {
        int iguais = strcmp(c->nome_cidade, nome);
        if (iguais == 0) {

            return c;
        }
        c = c->proximo;
    }
    return NULL;
}

void remove_cidade(LISTA_CIDADES *listaCidades, char nome[])
{
    CIDADES *encontrar_cidade = encontrar_cidades(listaCidades, nome);
    if (encontrar_cidade == NULL) {
        printf("Cidade  nao encontrada\n");
        return;
    }
    CIDADES *ppre;
    CIDADES *pcur = listaCidades->p_cidade;
    while (pcur != encontrar_cidade && pcur != NULL) {
        ppre = pcur;
        pcur = pcur->proximo;
    }
    if (listaCidades->p_cidade == encontrar_cidade) {
        listaCidades->p_cidade = pcur->proximo;
        listaCidades->n_cidades--;
        printf("%d %d\n", pcur->id_cidade);
        return;
    }
    if (listaCidades->p_cidade->id_cidade == encontrar_cidade) {
        listaCidades->p_cidade->proximo = pcur->proximo;
        listaCidades->n_cidades--;
        return;
    }
    ppre->proximo = pcur->proximo;
    listaCidades->n_cidades--;
}

void print_cidades(LISTA_CIDADES *listaCidades) {
    CIDADES *aux = listaCidades->p_cidade;
    printf("Numero de cidades %d\n", listaCidades->n_cidades);
    while (aux != NULL) {
        printf("Id: %d nome: %s informacao: %s locx: %d locy: %d Pontos de Interesse: %s\n", aux->id_cidade,
               aux->nome_cidade,
               aux->informacao, aux->locx, aux->locy, aux->poi);
        aux = aux->proximo;
    }
}

CIDADES *encontrar_cidades_nome(const LISTA_CIDADES *listaCidades,char nome[])
{
    CIDADES *c = listaCidades->p_cidade;
    while (c != NULL) {
        int iguais = strcmp(c->nome_cidade, nome);
        if (iguais == 0) {
            printf("Id: %d nome: %s informacao: %s locx: %d locy: %d Pontos de Interesse: %s\n", c->id_cidade,
                   c->nome_cidade,
                   c->informacao, c->locx, c->locy, c->poi);
        }
        c = c->proximo;
    }
    return NULL;
}

void editar_informacao_cidade(LISTA_CIDADES *listaCidades, char informacao[], char nome_cidade[])
{
    CIDADES *c = encontrar_cidades(listaCidades, nome_cidade);
    c->informacao = informacao;
}

void editar_poi_cidade(LISTA_CIDADES *listaCidades, char poi[],char nome_cidade[])
{
    CIDADES *c = encontrar_cidades(listaCidades, nome_cidade);
    c->poi = poi;
}

//____________________________________________________Viagens______________________________________________________________________________________//

LISTA_VIAGEM *create_lista_viagens()
{
    LISTA_VIAGEM *lc = (LISTA_VIAGEM *) calloc(1, sizeof(LISTA_VIAGEM));
    lc->p_viagem = NULL;
    return lc;
}

void insert_viagem(char nome_viagem[], int NIF, char destino[], LISTA_VIAGEM *lista_viagens)
{
    VIAGEM *viagens = (VIAGEM *) calloc(1, sizeof(VIAGEM));
    viagens->NIF = NIF;
    viagens->nome = (char *) malloc(sizeof(char) * strlen(nome_viagem) + 1);
    strcpy(viagens->nome, nome_viagem);
    viagens->cidades = (char *) malloc(sizeof(char) * strlen(destino) + 1);
    strcpy(viagens->cidades, destino);
    VIAGEM *pcl = lista_viagens->p_viagem;
    if (pcl == NULL) {
        pcl = viagens->proximo;
        viagens->proximo = NULL;
    } else {
        viagens->proximo = pcl;
        pcl = viagens->proximo;
    }
    if (pcl == lista_viagens->p_viagem) {
        viagens->proximo = lista_viagens->p_viagem;
        lista_viagens->p_viagem = viagens;
        lista_viagens->n_viagens++;
        return;
    } else {
        viagens->proximo = pcl;
        lista_viagens->n_viagens++;
    }
}

VIAGEM *encontrar_viagens(const LISTA_VIAGEM *listaViagem, int NIF)
{
    VIAGEM *c = listaViagem->p_viagem;
    while (c != NULL) {
        if (c->NIF == NIF)return c;
        c = c->proximo;
    }
    return NULL;
}

void remove_viagem(LISTA_VIAGEM *lista_viagem, int NIF)
{
    VIAGEM *encontrar_viagem = encontrar_viagens(lista_viagem, NIF);
    if (encontrar_viagem == NULL) {
        printf("Viagem  nao encontrado\n");
        return;
    }
    VIAGEM *ppre = NULL;
    VIAGEM *pcur = lista_viagem->p_viagem;
    while (pcur != encontrar_viagem && pcur != NULL) {
        ppre = pcur;
        pcur = pcur->proximo;
    }
    if (lista_viagem->p_viagem == encontrar_viagem) { //Primeiro
        lista_viagem->p_viagem = pcur->proximo;
        lista_viagem->n_viagens--;
        printf("%d %d\n", pcur->NIF);
        return;
    }
    if (lista_viagem->p_viagem->NIF == encontrar_viagem) {
        lista_viagem->p_viagem->proximo = pcur->proximo;
        lista_viagem->n_viagens--;
        return;
    }
    ppre->proximo = pcur->proximo;
    lista_viagem->n_viagens--;
}

VIAGEM *encontrar_viagens_NIF(LISTA_VIAGEM *listaViagem, int NIF)
{
    VIAGEM *c = listaViagem->p_viagem;
    while (c != NULL) {
        if (c->NIF == NIF)
            printf("Nome: %s NIF: %d Locais: %s \n", c->nome, c->NIF, c->cidades);
        c = c->proximo;
    }
    return NULL;
}

void editar_cidades_viagem(LISTA_VIAGEM *listaViagem, char nome_viagem[], char cidades[]) {
    VIAGEM *c = encontrar_viagens_nome(listaViagem, nome_viagem);
    if (c != NULL) {
        c->cidades = cidades;
    }
    if (c == NULL) {
        printf("Viagem com esse nome nao encontrada\n");
    }
}

void editar_nome_viagem(LISTA_VIAGEM *listaViagem, char nome_viagem[], char novo_nome[])
{
    VIAGEM *c = encontrar_viagens_nome(listaViagem, nome_viagem);
    c->nome = novo_nome;
}

VIAGEM *
encontrar_viagens_nome(const LISTA_VIAGEM *listaViagem, char nome[])
{
    VIAGEM *c = listaViagem->p_viagem;
    while (c != NULL) {
        int iguais = strcmp(c->nome, nome);
        if (iguais == 0) {
            return c;
        }
        c = c->proximo;
    }
    return NULL;
}

void print_viagens(LISTA_VIAGEM *listaViagem, int NIF)
{
    VIAGEM *aux = listaViagem->p_viagem;
    printf("Numero de Viagens disponiveis %d\n", listaViagem->n_viagens);
    while (aux != NULL && aux->NIF == NIF) {
        printf("Nome: %s NIF: %d Cidades: %s\n", aux->nome, aux->NIF, aux->cidades);
        aux = aux->proximo;
    }
}

int main() {
    doseed();
    init_ag();
}


//Projeto efetuado por Gil Silva 41805 e João Silva 39839