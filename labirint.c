#ifdef __unix__
    #include <unistd.h>
#elif defined(_WIN32) || defined(WIN32)
    #define CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#define minn -1
#define maxx 200
int x,y;

typedef struct structura
{
	int vector[200][2];
	int varf;
}structura;

void citire(char *s)
{
	int i = 0;
	if ((s[0] = getchar()) != '\n') i++;
	while ((s[i] = getchar()) != '\n') i++;
	s[i] = '\0';
}

void separare(char *linie, int *L, int i)
{
    int j;
    for (j = 1; j < y - 1; j++)
        L[i*y+j] = linie[2*(j-1)] - '0';
}

//initializare
void init(int *L, int *iesire)
{
    FILE *f;
    int i, j, o = 0;
    char s[21], *linie = (char*) malloc (2*y*sizeof(char));

    //initializare margini
    for (i = 0; i < x; i++)
    {
        L[i*y] = 0;
        L[i*y+y-1] = 0;
    }
    for (j = 0; j < y; j++)
    {
        L[j] = 0;
        L[(x-1)*y+j] = 0;
    }

    //citire labirint
    printf("Citire labirint:\n");
    printf("1. Consola\n");
    printf("2. Fisier\n");
    do{
        printf("Optiune: ");
        scanf("%d", &o);
        switch(o)
        {
            case 1:
                for (i = 1; i < x-1; i++)
                {
                    printf("Linia %d: ", i);
                    for (j = 1; j < y-1; j++)
                        scanf("%d", &L[i*x+j]);
                }
            break;
            
            case 2:
                do{
                    printf("Nume fisier: ");
                    citire(&s[0]);
                    f = fopen(s, "r");
                    if(!f)
                        printf("Fisierul nu exista!\n");
                }while(!f);

                for(i = 1; i < x-1; i++)
                {   
                    fgets(linie, 2*y, f);
                    separare(linie, L, i);
                }
                fclose(f);
            break;

            default:
                printf("Optiunile sunt de la 1 la 2!\n");
            break;
        }
    } while (o != 1 && o != 2);
    free(linie);

    //citire iesire
    printf("Iesire labirint:\n");
    printf("Linie: ");
    scanf("%d", iesire);
    printf("Coloana: ");
    scanf("%d", (iesire+1));
}

//stiva
void initializare(structura *stiva)
{
	stiva->varf = minn;
}

int status(structura *stiva)
{
	if (stiva->varf == maxx)
		return (2);
	else if (stiva->varf == minn)
		return (1);
	else return (0);
}

void push(structura *stiva, int *element)
{
    if (status(stiva) != 2)
    {
	    (stiva->varf)++;
	    stiva->vector[stiva->varf][0] = element[0];	
        stiva->vector[stiva->varf][1] = element[1];	
    }
}

void pop(structura *stiva, int *element)
{
    if (status(stiva) != 1)
    {      
	    element[0] = stiva->vector[stiva->varf][0]; 
        element[1] = stiva->vector[stiva->varf][1];
        (stiva->varf)--;
    }
}

// rezolvare
int punct(char c)
{
    if(c == '.')
        return(0);
    else
        return(c);
}

int optiuni(int *L, int *poz)
{
    int sum = 0;
    sum = punct(L[(poz[0]+1)*y + poz[1]]) + punct(L[(poz[0]-1)*y + poz[1]]) + \
            punct(L[poz[0]*y + poz[1]+1]) + punct(L[poz[0]*y + poz[1]-1]);
    return (sum);
}

void mutare(int *L, int *poz)
{
    L[poz[0]*y+poz[1]] = '.';
    if (punct(L[poz[0]*y + poz[1]+1])) poz[1]++;
    else if (punct(L[(poz[0]+1)*y+poz[1]])) poz[0]++;
    else if (punct(L[(poz[0]-1)*y + poz[1]])) poz[0]--;
    else if (punct(L[poz[0]*y + poz[1]-1])) poz[1]--;
}

void reset(int* L, int* poz, structura* alegere, structura* drum)
{
    int n[2];
    L[poz[0]*y+poz[1]] = 0;
    pop(alegere, poz);
    while (drum->vector[drum->varf][0] != poz[0] \
        || drum->vector[drum->varf][1] != poz[1])
    {
        pop(drum, &n[0]);
        L[n[0]*y+n[1]] = 0;
    }
}

int gata(int *poz, int *iesire)
{
    if(poz[0] == iesire[0] && poz[1] == iesire[1])
        return (1);
    else 
        return (0);
}

void afisare(int *L, int *poz, int *iesire, structura *drum)
{
    int i, j;
    if (!gata(poz, iesire))
        printf("Labirintul nu are solutie!\n");
    else
    {
        printf("Solutie:\n\n");
        for (i = 0; i <= drum->varf; i++)
            L[drum->vector[i][0]*y + drum->vector[i][1]] = '.';
        L[iesire[0]*y+iesire[1]] = 'x';
        for (i = 1; i < x-1; i++)
        {
            for (j = 1; j < y-1; j++)
                if(L[i*y+j]<2)
                    printf("%d ", L[i*y+j]);
                else 
                    printf("%c ", L[i*y+j]);
            printf("\n");
        }
        printf("\n");
        printf("Pasi: ");
        for (i = 0; i <= drum->varf; i++)
            printf("%d-%d ", drum->vector[i][0], drum->vector[i][1]);
        printf("\n");
    }
}

void animatie_linux(int *L, int *poz, int o)
{
    #ifdef __unix__
    int i, j;
    for (i = 1; i < x - 1; i++)
    {
        for (j = 1; j < y -1; j++)
            if(i == poz[0] && j == poz[1])
            {
                printf("\033[1;31m");
                printf("x ");
                printf("\033[0m");
            }
            else if (L[i*y+j] == '.')
                printf("%c ", L[i*y+j]);
            else
                printf("%d ", L[i*y+j]);
        printf("\n");
    }
    if(o == 1)
        usleep(70000);
    else
        getchar();
    system("clear");
    #endif
}

void rezolvare(int *L, int *poz, int *iesire)
{
    int o = 0;
    structura drum, alegere;
    initializare(&drum);
    push(&drum, poz);
    initializare(&alegere);

    #ifdef __unix__
        printf("Animatie:\n");
        printf("1. Automat\n");
        printf("2. Input\n");
        printf("Optiune: ");
        scanf("%d", &o);
    #endif

    while ((optiuni(L, poz) || status(&alegere)!=1) && !gata(poz, iesire))
    {
        animatie_linux(L, poz, o); 
        if (optiuni(L, poz) == 0)
            reset(L, poz, &alegere, &drum);
        if (optiuni(L, poz) >= 1)
        {
            if (optiuni(L, poz) > 1)
                push(&alegere, poz);
            mutare(L, poz);
        }
        push(&drum, poz);
    }
    afisare(L, poz, iesire, &drum);
}

int main(void)
{
    printf("Dimensiune labirint (linii, coloane): ");
    scanf("%d", &x);
    scanf("%d", &y);
    x = x + 2;
    y = y + 2;
    int *L = (int*) malloc(x*y*sizeof(int));
    int poz[2] = {1,1}, iesire[2];
    init(L, &iesire[0]);
    rezolvare(L, &poz[0], &iesire[0]);
    free(L);
    return (0);
}
