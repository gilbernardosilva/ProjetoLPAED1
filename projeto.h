#ifndef LP1AEDPROJETOALGORITMO_PROJETO_H
#define LP1AEDPROJETOALGORITMO_PROJETO_H
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>


/**
 * Estrutura do Gene
 * Contem os campos de informação de um Gene
 */
typedef struct GENE{
    int id;
    int x;
    int y;
    char name[20];
}gene;

/**
 * Estrutura de um Cromossoma
 * Contêm os campos de informação de um Cromossoma e um apontador do tipo Gene para um array de Genes
 */
typedef struct CHROMOSOME{
    gene *gene_array;
    int csize;
    double fitness;
}chromosome;

/**
 * Estrutura de um Cromossoma
 * Contêm os campos de informação de uma População e um apontador do tipo Cromossoma para um array de Cromossomas
 * Contêm também um apontador do tipo popuçação para o proximo elemento da lista ligada de Populações
 */
typedef struct POPULATION{
    chromosome *chromosome_array;
    double totalfitness;
    int psize;
    int generation;
    struct POPULATION *next;
}population;


/**
 * Parametros do Algoritmo Genético
 */
#define MAX_GENERATIONS 20 /* Numero de Gerações */
#define CHROMOSOME_SIZE 20 /* Numero de Genes (cidades) */
#define POP_SIZE 10 /* Numero de Cromossomas (trajetos) */
#define ELITISM_SIZE 4 /* Numero de individuos que passam por elitismo*/
#define MUTATION 0.5 /* Probabilidade de Mutação */


/**
 * Funcao que inicializa um gene através dos valores passados pelos parametros
 * @param g O gene aka cidade
 * @param x A coordenada x
 * @param y A coordenada y
 * @param id O id da cidade
 * @param name O nome da cidade
 */
void init_gene(gene *g, int x, int y, int id,char name[20]);

/**
 * Funcao que recebe um apontador para inicializar os valores de um cromossoma com o tamanho passado pelo segundo parámetro e com os valores contidos num ficheiro de texto
 * @param c Apontador Cromossoma
 * @param size Tamanho do Cromossoma (CHROMOSOME_SIZE)
 */
void init_chromosome(chromosome *c, int size);

/**
 * Funcao que recebe um apontador para inicializar os valores de uma população com o tamanho passado pelo segundo parámetro
 * @param p Apontador População
 * @param psize Tamanho da População (POPSIZE)
 */
void init_population(population *p, int psize);

/**
 * Função utilizada para calcular o fitness(aptidão) de um cromossoma passado como parametro e retorna um double com o valor do fitness
 * @param c Apontador Cromossoma
 * @return Double com o valor do fitness
 */
double calculate_fitness(chromosome *c);

/**
 * Função que em conjunto com a função de calcular o fitness, calcula o fitness de todos os elementos de uma população e guarda o valor no campo fitness de cada cromossoma
 * Também guarda o valor total de fitness de uma população na estrutura da mesma
 * @param p Apontador População
 */
void evaluatePop(population *p);

/**
 * Função que se deve inicializar antes de correr qualquer codigo para incializar uma "seed random"
 */
void doseed(void);

/**
 * Função que gera um double entre um intervalo passado pelos parametros
 * @param min
 * @param max
 * @return O double gerado
 */
double randomDouble(double min, double max);

/**
 * Função que gera um inteiro entre um intervalo passado pelos parametros
 * @param min
 * @param max
 * @return O inteiro gerado
 */
int randomInt(int min, int max);

/**
 * Função que recebe um apontador um cromossoma e troca desse cromossoma os genes cujas posições foram passadas pelos parámetros
 * @param c Apontador Cromossoma
 * @param i Posição 1
 * @param j Posição 2
 */
void swapGene(chromosome*c,int i,int j);

/**
 *Função que recebe um apontador para uma população e troca dessa população os cromossomas cujas posições foram passadas pelos parámetros
 * @param p Apontador População
 * @param a Posição 1
 * @param b Posição 2
 */
void swapChromosome(population *p,int a, int b);

/**
 * Função que recebe um apontador para um cromossoma e define a primeira cidade igual á ultima
 * @param c
 */
void matchcities(chromosome * c);

/**
 * Função que recebe um apontador um cromossoma e lhe da shuffle, garantido que a primeira e a ultima cidade são iguais
 * @param c Apontador Cromossoma
 */
void shuffle_chromosome(chromosome *c);

/**
 * Função que aplica a roullete wheel implementada no enunciado do projeto, recebe uma população como parámetro e retorna um indice aleatório
 * @param p Apontador População
 * @return Indice aleatório do cromossoma que resultou
 */
int roullete_wheel(population *p);

/**
 * Função que aplica uma mutação baseada na probabilidade passada pelos parametros do AG
 * A mesma aplica o swap chromosome através de dois indices aleatorios entre 1 e CHROMOSOME_SIZE-1
 * @param p Apontador População
 */
void apply_mutation(population *p);

/**
 * Função que aplica o algoritmo bubbleSort que recebe um apontador para uma população e organiza-a de menor para o maior em relação ao campo de fitness
 * O bubblesort obriga a que os elementos presentes no elitismo que passam diretamente já fiquem no fim da população e não tenham de ser alterados
 * @param p Apontador população
 */
void bubbleSort(population *p);

/**
 * Função utilizada na seleção para verificar a presença de duplicados após o crossover
 * @param c Apontador Cromossoma
 * @return 0 se não houver duplicados ou o indice do primeiro duplicado encontrado
 */
int checkDuplicates(chromosome *c);

/**
 * Função que aplica a seleção utilizando a roullete wheel, o elitismo e o crossover que neste caso é um partially mapped crossover
 * A mesma gera indices aleatorios para serem usados na roullete wheel e inicializa duas "childs" para serem o output do crossover
 * A leitura e abertura do ficheiro é para garantir que o crossover caso tenha duplicados inicializemos a cidade que falta através da função checkduplicates
 * Após o crossover e de termos feito todos os "filhos" libertamos a memoria dos filhos
 * @param p
 */
void apply_selection(population *p);

/**
 * Função utilizada para imprimir os dados de uma população
 * @param p Apontador População
 * @param size Tamanho População (POP_SIZE)
 */
void print(population *p,int size);

/**
 * Função para imprimir a lista ligada das populações utilizando a função print
 * @param head Apontador do tipo população para a primeira população (head)
 */
void printList(population * head);


/**
 * Função que recebe um apontador para a cabeça de uma lista ligada e dá free á memoria utilizada pela mesma
 * @param head Apontador para a cabeça
 */
void freeList(population * head);

/**
 * Função que recebe um valor e uma cabeça de uma lista ligada e verifica se uma certa geração existe
 * @param head Apontador cabeca
 * @param value Geração a procurar
 * @return Dá print a se existe ou não
 */
int searchbygeneration(population *head, int value);

/**
 * Função para inserir um node á cauda
 * Raramente utilizada pois o algoritmo genetico implementa automaticamente este conceito
 * @param head Cabeça lista ligada
 * @param node Node a adicionar
 * @return Apontador cabeca
 */
population * InsertNodeAtTail(population *head, population* node);

/**
 * Função para inserir um node á cabeça
 * Raramente utilizada pois o algoritmo genetico implementa uma inserção á cauda automatica o que seria de esperar num algoritmo que analisa gerações
 * @param head Cabeça lista ligada
 * @param node Node a adicionar
 * @return Apontador cabeca
 */
population *InsertNodeAtHead(population *head, population* node);

/**
 * Função utilizada para inicializar o AG
 *
 */
void init_ag();

/**
 * estrutura clientes que contem toda a informaçao pedida para um cliente e que ainda contem um apontador para o proximo cliente
 */
typedef struct clientes {
    int id_cliente;
    char *nome_cliente;
    int NIF;
    char *morada_cliente;
    int contacto;
    int IBAN;
    int data_nascimento;
    int data_registo;
    struct clientes *proximo;
} CLIENTES;

/**
 * estrutura lista de clientes que associa todos os clientes dentro de uma lista
 */
typedef struct lista_de_clientes {
    int n_clientes;
    CLIENTES *pcliente;
} LISTA_CLIENTES;

typedef struct cidades {
    int id_cidade;
    char *nome_cidade;
    int locx;
    int locy;
    char *informacao;
    char *poi;
    struct cidades *proximo;
} CIDADES;

typedef struct lista_de_cidades {
    int n_cidades;
    CIDADES *p_cidade;
} LISTA_CIDADES;

typedef struct viagem{
    char *nome;
    int NIF;
    char *cidades;
    struct viagem *proximo;
}VIAGEM;

typedef struct lista_viagem{
    int n_viagens;
    VIAGEM *p_viagem;
}LISTA_VIAGEM;


//FUNÇOES CLIENTES
/**
 * funçao usada para inicializar a lista de clientes
 * @return
 */
LISTA_CLIENTES *create_lista_clientes();
/**
 * funçao usada para adicionar um cliente a cabeça da função colocando os parametros passados nas respetivas caixas da estrutura clientes
 * @param lista_clientes
 * @param nome
 * @param NIF
 * @param morada_cliente
 * @param contacto
 * @param IBAN
 * @param data_nascimento
 * @param data_registo
 */
void insert_cliente_cabeca(LISTA_CLIENTES *lista_clientes,char nome[],int NIF,char morada_cliente[],int contacto,int IBAN,int data_nascimento,int data_registo);
/**
 * Funçao usada para inserir um cliente a cauda da lista , colocando os parametros passados nas respetivas caixas da estrutura clientes
 * @param lista_clientes
 * @param nome
 * @param NIF
 * @param morada_cliente
 * @param contacto
 * @param IBAN
 * @param data_nascimento
 * @param data_registo
 */
void insert_cliente_cauda(LISTA_CLIENTES *lista_clientes, char nome[], int NIF, char morada_cliente[],int contacto, int IBAN, int data_nascimento, int data_registo);
/**
 * Funçao auxiliar usada para encontrar um cliente na lista
 * @param listaClientes
 * @param id_cliente
 * @return
 */
CLIENTES *encontrar_clientes(const LISTA_CLIENTES *listaClientes, int id_cliente);
/**
 * Funçao usada para remover um cliente apartir do seu NIF
 * @param listaClientes
 * @param NIF
 */
void remove_cliente(LISTA_CLIENTES *listaClientes, int NIF);
/**
 * funçao usada para imprimir a lista de clientes que temos disponivel no momento
 * @param listaClientes
 */
void print_clientes(LISTA_CLIENTES *listaClientes);
/**
 * Funçao usada para encontrar um cliente apartir do seu NIF , no qual sera imprimida toda a informaçao acerca desse mesmo cliente
 * @param listaClientes
 * @param NIF
 * @return
 */
CLIENTES *encontrar_clientes_NIF(const LISTA_CLIENTES *listaClientes, int NIF);
/**
 * Funçao usada para encontrar um cliente apartir do seu nome no qual serao imprimidos todos os dados acerca do mesmo
 * @param listaClientes
 * @param nome   recebe um nome apartir de uma string ou de um vetor iniciado anteriormente
 * @return
 */
CLIENTES *encontrar_clientes_Nome(const LISTA_CLIENTES *listaClientes, char nome[]);

/**
 * Funçao auxiliar que serve para comparar os NIF dos clientes de maneira a ver qual e o maior
 * @param a
 * @param b
 */
void comparar_clientes(CLIENTES *a, CLIENTES*b);

/**
 * Funçao que ordena os clientes da lista de clientes atraves do seu NIF
 * @param Head
 */
void bubbleSort_clientes(LISTA_CLIENTES *Head);

//FUNCOES CIDADES
/**
 * Funçao que aloca memoria para criar a lista de cidades
 * @return
 */
LISTA_CIDADES *create_lista_cidades();
/**
 * Funçao que e usada para inserir uma cidade e associa a lista de cidades e onde os parametros sao adicionados a cada caixa da estrutura cidades
 * @param lista_cidades
 * @param nome
 * @param id_cidade
 * @param cordx
 * @param cordy
 * @param informacao
 * @param poi
 */
void insert_cidades(LISTA_CIDADES *lista_cidades,char nome[], int id_cidade,int cordx, int cordy, char informacao[],char poi[]);
/**
 * Funçao auxiliar usada para encontrar cidade apartir do nome que e recebido como parametro
 * @param listaCidades
 * @param nome
 * @return
 */
CIDADES *encontrar_cidades(const LISTA_CIDADES *listaCidades, char nome[]);
/**
 * Funçao usada para remover uma cidade apartir do seu nome recebido como parametro
 * @param listaCidades
 * @param nome
 */
void remove_cidade(LISTA_CIDADES *listaCidades,char nome[]);
/**
 * Funçao que imprime as cidades adicionas ate ao momento na lista de cidades
 * @param listaCidades
 */
void print_cidades(LISTA_CIDADES *listaCidades);
/**
 * Funçao que ser para editar a informaçao contida dentro de uma cidade
 * @param listaCidades
 * @param informacao  (um dos nos da estrutura cidade )
 * @param nome_cidade
 */
void editar_informacao_cidade(LISTA_CIDADES *listaCidades,char informacao[],char nome_cidade[]);
/**
 * Funçao usada para editar os pontos de interesse dentro de uma respetiva cidade
 * @param listaCidades
 * @param poi
 * @param nome_cidade
 */
void editar_poi_cidade(LISTA_CIDADES *listaCidades,char poi[],char nome_cidade[]);
//funçoes viagens
/**
 * Funçao que aloca memoria para criar uma lista de viagens
 * @return
 */
LISTA_VIAGEM *create_lista_viagens();
/**
 * Funçao usada para criar uma viagem que fica associada a uma lista de viagens
 * @param nome_viagem
 * @param NIF
 * @param destino
 * @param lista_viagens
 */
void insert_viagem(char nome_viagem[],int NIF, char destino[],LISTA_VIAGEM *lista_viagens);
/**
 * Funçao usada para encontrar uma viagem atraves do NIF associada a mesma
 * @param listaViagem
 * @param NIF
 * @return
 */
VIAGEM *encontrar_viagens(const LISTA_VIAGEM *listaViagem, int NIF);
/**
 * Funçao usada para remover uma viagem associada a um NIF
 * @param lista_viagem
 * @param NIF
 */
void remove_viagem(LISTA_VIAGEM*lista_viagem,int NIF);
/**
 * Funçao que edita as cidades associadas a uma viagem ja criada
 * @param listaViagem
 * @param nome_viagem
 * @param cidades
 */
void editar_cidades_viagem(LISTA_VIAGEM *listaViagem,char nome_viagem[],char cidades[]);
/**
 * Funçao que permite encontrar todas as viagens associadas a um NIF
 * @param listaViagem
 * @param NIF
 * @return
 */
VIAGEM *encontrar_viagens_NIF(LISTA_VIAGEM *listaViagem, int NIF);
/**
 * Funçao que permite encontrar as viagens com um determinado nome
 * @param listaViagem
 * @param nome
 * @return
 */
VIAGEM *encontrar_viagens_nome(const LISTA_VIAGEM *listaViagem, char nome[]);
/**
 * Funçao que funciona como historico no qual e possivel imprimir as viagens associadas a um cliente com um NIF especifico
 * @param listaViagem
 * @param NIF
 */
void print_viagens(LISTA_VIAGEM *listaViagem,int NIF);





#endif //LP1AEDPROJETOALGORITMO_PROJETO_H
