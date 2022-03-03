//Pedro Henrique De Sousa Alcântara
//512237

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Tamanho da estrutura dos blocos
int tam=0;

//Tamanho da estrutura das hashtags
int tam_hash=0;

//Variáveis de controle de bloco.
int livre=5;
int espaco=0;
int tam_bloco=5;//Bloco possui capacidade de aramazenamento de 5 tweets.

typedef struct bloco{
        char  **vetor_tweets;
        struct bloco *prox;
        struct bloco *ant;

    }BO;

BO *inicio=NULL;
BO *fim=NULL;

typedef struct tag{
        char  *hashtag;
        int contabilizado;//Vai servir para fazer a contagem de hashtags em um função mais a frente.
        int lido;
        int vezes;
        struct tag *prox;
        struct tag *ant;
        char pertence[280];
    }tg;

tg *inicio_hash=NULL;
tg *fim_hash=NULL;

tg *inicio_print=NULL;
tg *fim_print=NULL;

void add_hash(char *tag, char* nome){
    tg *new;
    new=malloc(sizeof(tg));
    new->hashtag=tag;
    strcpy(new->pertence, nome);
    new->contabilizado=1;
    tg *aux;
    if(inicio_hash==NULL){
        inicio_hash=new;
        new->prox=NULL;
        new->ant=NULL;
        fim_hash=new;
    }
    else{
        fim_hash->prox=new;
        new->ant=fim_hash;
        new->prox=NULL;
        fim_hash=new;


    }
    tam_hash++;
}


void imprimir(){

    BO * aux = inicio;
    puts("\n");
    puts("----------------Imprimindo lista de tweets em ordem------------------- ");
    for (int j=0; j < tam; ++j)
    {
        if(j==tam-1){//Para evitar ler lixo de memória, aqui verifico se é o ultimo bloco. Se sim, apenas leias o N tweets salvos.
            for (int i = 0; i<espaco; ++i)
            {
            printf("%s\n",aux->vetor_tweets[i] );
            }
        }
        else{//Se não, pode ler o o 5 espaços ocupados(tam_bloco).
            for (int i = 0; i<tam_bloco; ++i)
            {
                printf("%s\n",aux->vetor_tweets[i] );
            }
        }
        if(aux->prox==NULL){
            break;
        }
        aux=aux->prox;
    }
    puts("\n---------------------------------------------------------------------\n");
    puts("\n");
}


void tweet(char *txt){
    char *tag;
    char *nome=txt;
    int size=strlen(txt);
    strcat(txt, " ");//Por conta do laço abaixo que nao entra se o caractere for \0, se a ultima palavra do
    //tweet for uma hashtag, ele não vai salvar ela.
    int i=0;
    while(txt[i]!='\0')
    {
        char *tag=malloc(sizeof(char)*size);
        char *p;
        char *q;
        int n;
        p=strchr(txt,'#');

        if(p==NULL){
            break;
        }

        q=strchr(p,' ');

        if(q==NULL){//Caso tente procurar um espaço a frente e não achar, quero que procure o \0
            q=strchr(p,'\0');
        }

        n=q-p;

        for (int z = 0; z < n; ++z)
        {
            tag[z]=p[z];
            tag[z+1]='\0';
        }

        txt=q;

        i++;
        add_hash(tag, nome);
    }

    livre--;
    espaco++;


}
void alocar_memo(BO *new){
    new->vetor_tweets=malloc(tam_bloco*sizeof(char*));
    for (int i = 0; i < tam_bloco; ++i)
    {
        new->vetor_tweets[i]=malloc(280*sizeof(char));
    }
}
void add(BO **bloco_anterior){
    livre=5;
    espaco=0;
    BO *NOVO=malloc(sizeof(BO));
    alocar_memo(NOVO);
    fim->prox=NOVO;
    NOVO->ant=fim;
    fim=NOVO;
    tam++;
    *bloco_anterior=NOVO;

}

void sort(tg *inicio_print){
    tg *aux=inicio_print;
    tg *aux_p=NULL;
    int v;
    char *hashtag_b;

    while(aux!=NULL){
        aux_p=aux->prox;
        while(aux_p!=NULL){
            if(aux->vezes<aux_p->vezes){//Basicamente irá trocas os valores entre o menor e o maior.

                v=aux->vezes;
                aux->vezes=aux_p->vezes;
                aux_p->vezes=v;
                //////////////////////////
                hashtag_b=aux->hashtag;
                aux->hashtag=aux_p->hashtag;
                aux_p->hashtag=hashtag_b;
                ////////////////////////////
            }
            aux_p=aux_p->prox;
        }
        aux=aux->prox;
    }
}

void adm(){
    puts("\n");
    puts("-----------------Funcoes de Administrador------------------- ");
    if(tam>1){
        printf("Quantidade de tweets: %d\n", (5*(tam-1)+espaco));
    }
    else{
        printf("Quantidade de tweets: %d\n", espaco);
    }
    printf("Quantidade de blocos: %d\n", tam);
    printf("Quantidade de hashtags: %d\n", tam_hash);
    printf("Espaco livre no ultimo bloco: %d\n", livre);
    puts("------------------------------------------------------------ ");
    puts("\n");
}

void trending_topics(){
    //Criei uma terceira lista encadeada, mas nessa não haverá hashtags duplicados e repetidos.
    //Sera contabilizado o numero de aparições de cada hashtag.
    int contador=1;
    for (tg *aux=inicio_hash; aux!=NULL; aux=aux->prox)//Irá correr todos os itens da lista encadeada 1(onde há hashtags repetidos).
    {
        for (tg *aux2=aux->prox; aux2!=NULL; aux2=aux2->prox)//Irá verificar se ao resto da lista há outras hashtags iguais.
        {                                                       //Se houver, a variável aux2->contabilizador será maior que 1.
            if(strcmp(aux->hashtag, aux2->hashtag) == 0){       //Assim quando, for printar, há um if que apenas os aux que o contabilizado 1
                aux2->contabilizado+=1;                         //Sejam printados.
                contador+=1;
            }
        }
        if(aux->contabilizado==1){
            tg *new=malloc(sizeof(tg));
            if(inicio_print==NULL){
                inicio_print=new;
                fim_print=new;
                new->hashtag=aux->hashtag;
                new->vezes=contador;
            }
            else{
                fim_print->prox=new;
                new->ant=fim_print;
                new->prox=NULL;
                fim_print=new;
                new->hashtag=aux->hashtag;
                new->vezes=contador;

            }
        }
        contador=1;
    }
    sort(inicio_print);//Função que organizará a lista encadeada 2 já sem elementos repetidos e já contabilizados.
    puts("\n");
    puts("----------Trending Topics------------------- ");
    for (tg *aux=inicio_print; aux!=NULL; aux=aux->prox)
    {
        printf("%d - %s ",aux->vezes, aux->hashtag );
        puts("\n");
    }
    puts("-------------------------------------------- ");
    puts("\n");
    for (tg *aux=inicio_print; aux!=NULL; aux=aux->prox)//Limpando tudo, para que na proxima chamada das trendings, a contagem ocorra de novo.
    {
        free(aux);
    }
    inicio_print=NULL;
    fim_print=NULL;
}
void filtrar(char *hash){
    puts("\n");
    puts("----------Resultados da busca por hashtag------------------- ");
    for (tg *aux=inicio_hash; aux!=NULL; aux=aux->prox)
    {
        if(strcmp(aux->hashtag, hash) == 0){//verifica se as hashtags batem, se sim, a função strcmp retorna 0.
            printf("%s\n", aux->pertence);
        }
    }
    puts("------------------------------------------------------------ ");
    puts("\n");
}

void escrever(){
    char nome[50];
    printf("Digite o nome do arquivo: ");
    scanf("%s", nome);
    strcat(nome, ".txt");
    FILE *arq=fopen(nome, "w");

    time_t data_atual;
    time(&data_atual);

    char *str=ctime(&data_atual);

    struct tm *pronto = localtime(&data_atual);

    fprintf(arq, "%d/%d/%d - %d:%d \n", pronto->tm_mday, pronto->tm_mon +1 , pronto->tm_year+1900, pronto->tm_hour, pronto->tm_min);


    BO * aux = inicio;

    for (int j=0; j < tam; ++j)
    {
        if(j==tam-1){//Para evitar ler lixo de memória, aqui verifico se é o ultimo bloco. Se sim, apenas leias o N tweets salvos.
            for (int i = 0; i<espaco; ++i)
            {
                fputs(aux->vetor_tweets[i], arq);
                fputs("\n", arq);
            }
        }
        else{//Se não, pode ler o o 5 espaços ocupados(tam_bloco).
            for (int i = 0; i<tam_bloco; ++i)
            {
                fputs(aux->vetor_tweets[i], arq);
                fputs("\n", arq);
            }
        }
        aux=aux->prox;
    }
    puts("\nTweets salvos em arquivo com sucesso! \n");
    fclose(arq);
}

int main(){

    BO *bloco_atual=malloc(sizeof(BO));
    alocar_memo(bloco_atual);
    inicio=bloco_atual;
    fim=bloco_atual;
    tam++;

    int v;
    while(1){
        puts("Tweet: Apenas digite o tweet: [0]\n"
            "Carregar a partir de um arquivo salvo[1]:\n"
            "Salvar Arquivos salvos[2]:\n"
            "Filtrar Hashtags[3]: \n"
            "Ver informacoes de Administrador[4]: \n"
            "Ver Trending Topics[5]: \n"
            "Imprimir Todos os Tweets[6]: \n"
             "Sair[7]: \n");
        scanf("%d", &v);
        if(v==7){
            break;
        }
        if(v==3){
            char *tweet_filtro=malloc(sizeof(char)*280);
            printf("Digite o Tweet a ser filtrado: \n");
            scanf("%s", tweet_filtro);
            fpurge(stdin);
            filtrar(tweet_filtro);
        }
        if(v==4){
            adm();
        }
        if(v==5){
            trending_topics();
        }
        if(v==1){
            int pergunta;
            char nome_arquivo[50];
            FILE *arquivo;
            do{
            printf("Digite o nome do arquivo: \n");
            scanf("%s", nome_arquivo);
            strcat(nome_arquivo, ".txt");

            arquivo = fopen(nome_arquivo,"r");
            if(arquivo==NULL){
                puts("Não foi possivel abrir o arquivo!\n");
                puts("Ou tente novamente. \n");
            }
            }while(arquivo==NULL);

            printf("Deseja adicionar os tweets salvos sem limpar lista atual[0] ou deseja limpar a lista antes de adicionar[1]?:\n");
            scanf("%d", &pergunta);
            if(pergunta==1){
                for (tg *aux=inicio_hash ; aux!=NULL; aux=aux->prox)
                {
                    free(aux);
                }
                inicio_hash=NULL;
                fim_hash=NULL;
                for (tg *aux2=inicio_print; aux2!=NULL; aux2=aux2->prox)
                {
                    free(aux2);
                }
                inicio_print=NULL;
                fim_print=NULL;
                for (int i = 0; i < tam; ++i)
                {
                    for (int j = 0; j < tam_bloco; ++j)
                    {
                        free(inicio->vetor_tweets[j]);
                    }
                    inicio=inicio->prox;
                }
                alocar_memo(bloco_atual);
                inicio=bloco_atual;
                fim=bloco_atual;
                livre=5;
                espaco=0;
                tam=1;
                tam_hash=0;
            }
            char linha[280]; //variável para ler as linhas
            int  n = 1;      //variável para numerar as linhas
            while(fgets(linha,280,arquivo) != NULL){
                if(n==1){
                    printf("O ultimo salvamento desse aquivo foi: %s\n", linha);
                    n++;
                }
                else{
                    if(livre==0){
                        add(&bloco_atual);
                    }
                    strcpy(bloco_atual->vetor_tweets[espaco], linha);
                    tweet(linha);
                    n++;
                }
            }

        }
        if(v==6){
            imprimir();
        }
        if(v==2){
            escrever();
        }
        if(v==0){
            if(livre==0){
                add(&bloco_atual);
            }
            printf("Digite o Tweet: \n");
            fpurge(stdin);
            char *txt;
            txt=gets(bloco_atual->vetor_tweets[espaco]);
            tweet(txt);
        }
    }

    return 0;
}
