#include <iostream>
#include <vector>
#include <queue>

using namespace std;

//Vertice do Grafo
struct Vertice
{
    int cor;
    int grau;
    bool verificado;
};

void imprime_matriz(int **matriz, int tamanho);
void imprime_sudoku(Vertice *vetor, int tamanho);
void imprime_vertices(Vertice *vetor, int tamanho);

int **inicializa_matriz_adj(int tamanho);
void inicializa_vertices(Vertice *vetor, int tamanho);
void colorir(int **matriz, Vertice *vetor, int tamanho);
vector<int> cores_iniciais(int tamanho);
int prox_vertice(int **matriz, Vertice *vetor, int tamanho);
void colorir_vertices(int **matriz, Vertice *vetor, int qnt_vertices, int inicio);

int **retorna_matriz_adj(int linha, int coluna, int tamanho);
void cria_matriz_adj(int **matriz, int tam_sudoku);

int main()
{
    int tamanho = 81;                               // Quantidade de vertices
    int **matriz_adj;                               // Matriz adjscente
    Vertice *vetor_vertices = new Vertice[tamanho]; // Vetor de vertices - grau e cor

    inicializa_vertices(vetor_vertices, tamanho);
    matriz_adj = inicializa_matriz_adj(tamanho); // Matriz de Adjascencia criada e inicia com 0 mas posicoes

    cria_matriz_adj(matriz_adj, 9); // Sudoku 4x4/9x9/16x16
    //imprime_matriz(matriz_adj, 81);

    colorir(matriz_adj, vetor_vertices, tamanho); // Funcao que inicia a coloracao

    cout << "\n";
    //imprime_matriz(matriz_adj,tamanho);
    //imprime_vertices(vetor_vertices,tamanho);
    cout << "\n";
    imprime_sudoku(vetor_vertices, tamanho);

    return 0;
}

//CRIACAO MATRIX_ADJ
// Retorna o bloco de vertices adj com 1
int **retona_bloco_completo(int **matriz, int lin_min, int lin_max, int col_min, int col_max)
{
    for (int i = lin_min; i < lin_max; i++)
    {
        for (int j = col_min; j < col_max; j++)
        {
            matriz[i][j] = 1;
        }
    }
    return matriz;
}

int **retorna_matriz_adj(int linha, int coluna, int tamanho)
{
    int **matriz = inicializa_matriz_adj(tamanho);
    matriz = new int *[tamanho];
    for (int i = 0; i < tamanho; i++)
    { // Inicia linha e coluna adj ao vertice com 1
        matriz[i] = new int[tamanho];
        for (int j = 0; j < tamanho; j++)
        {
            if (linha == i)
            {
                matriz[i][j] = 1;
            }
            if (coluna == j)
            {
                matriz[i][j] = 1;
            }
        }
    }
    //sudoku 4x4
    if (tamanho == 4)
    {
        if ((coluna + 2) % 2 == 0)
        {
            if ((linha + 2) % 2 == 0)
                matriz[linha + 1][coluna + 1] = 1;
            else
                matriz[linha - 1][coluna + 1] = 1;
        }
        else
        {
            if ((linha + 2) % 2 == 0)
                matriz[linha + 1][coluna - 1] = 1;
            else
                matriz[linha - 1][coluna - 1] = 1;
        }
    }
    //sudoku 9x9
    if (tamanho == 9)
    {
        if (linha < 3)
        {
            if (coluna < 3)
                matriz = retona_bloco_completo(matriz, 0, 3, 0, 3);
            else if ((coluna > 2) && (coluna < 6))
                matriz = retona_bloco_completo(matriz, 0, 3, 3, 6);
            else
                matriz = retona_bloco_completo(matriz, 0, 3, 6, 9);
        }
        else if ((linha > 2) && (linha < 6))
        {
            if (coluna < 3)
                matriz = retona_bloco_completo(matriz, 3, 6, 0, 3);
            else if ((coluna > 2) && (coluna < 6))
                matriz = retona_bloco_completo(matriz, 3, 6, 3, 6);
            else
                matriz = retona_bloco_completo(matriz, 3, 6, 6, 9);
        }
        else
        {
            if (coluna < 3)
                matriz = retona_bloco_completo(matriz, 6, 9, 0, 3);
            else if ((coluna > 2) && (coluna < 6))
                matriz = retona_bloco_completo(matriz, 6, 9, 3, 6);
            else
                matriz = retona_bloco_completo(matriz, 6, 9, 6, 9);
        }
    }
    //sudoku 16x16
    if (tamanho == 16)
    {
        if (linha < 4)
        {
            if (coluna < 4)
                matriz = retona_bloco_completo(matriz, 0, 4, 0, 4);
            else if ((coluna > 3) && (coluna < 8))
                matriz = retona_bloco_completo(matriz, 0, 4, 4, 8);
            else if ((coluna > 3) && (coluna < 8))
                matriz = retona_bloco_completo(matriz, 0, 4, 8, 12);
            else
                matriz = retona_bloco_completo(matriz, 0, 4, 12, 16);
        }
        else if ((linha > 3) && (linha < 8))
        {
            if (coluna < 4)
                matriz = retona_bloco_completo(matriz, 4, 8, 0, 4);
            else if ((coluna > 3) && (coluna < 8))
                matriz = retona_bloco_completo(matriz, 4, 8, 4, 8);
            else if ((coluna > 3) && (coluna < 8))
                matriz = retona_bloco_completo(matriz, 4, 8, 8, 12);
            else
                matriz = retona_bloco_completo(matriz, 4, 8, 12, 16);
        }
        else if ((linha > 7) && (linha < 12))
        {
            if (coluna < 4)
                matriz = retona_bloco_completo(matriz, 8, 12, 0, 4);
            else if ((coluna > 3) && (coluna < 8))
                matriz = retona_bloco_completo(matriz, 8, 12, 4, 8);
            else if ((coluna > 3) && (coluna < 8))
                matriz = retona_bloco_completo(matriz, 8, 12, 8, 12);
            else
                matriz = retona_bloco_completo(matriz, 0, 4, 12, 16);
        }
        else
        {
            if (coluna < 4)
                matriz = retona_bloco_completo(matriz, 12, 16, 0, 4);
            else if ((coluna > 3) && (coluna < 8))
                matriz = retona_bloco_completo(matriz, 12, 16, 4, 8);
            else if ((coluna > 3) && (coluna < 8))
                matriz = retona_bloco_completo(matriz, 12, 16, 8, 12);
            else
                matriz = retona_bloco_completo(matriz, 12, 16, 12, 16);
        }
    }
    matriz[linha][coluna] = 0;
    return matriz;
}

void atualiza_matriz_adj(int **matriz, int **aux, int linha, int tam_sudoku)
{
    int contador = 0;
    for (int i = 0; i < tam_sudoku; i++)
    {
        for (int j = 0; j < tam_sudoku; j++)
        {
            if (aux[i][j] == 1)
            {
                matriz[linha][contador] = 1;
            }
            contador++;
        }
    }
}

void cria_matriz_adj(int **matriz, int tam_sudoku)
{
    int **aux; // Matriz auxiliar - representa o sudoku
    int contador = 0;
    for (int i = 0; i < tam_sudoku; i++)
    {
        for (int j = 0; j < tam_sudoku; j++)
        {
            aux = retorna_matriz_adj(i, j, tam_sudoku);
            //imprime_matriz(aux,tam_sudoku);
            atualiza_matriz_adj(matriz, aux, contador, tam_sudoku);
            contador++;
        }
        //cout<<"\n->>>"<<i<<"\n";
    }
}

//COLORACAO E PRINT

void colorir(int **matriz, Vertice *vetor, int tamanho)
{
    int vertice_atual = 0;  // Inicio da coloracao - VERTICE ATUAL
    int contador = 0;       // Contador para verificar vertices coloridos - CRITERIO DE PARADA
    vector<int> cores_disp; // Vetor para verificar cores disponiveis

    vetor[vertice_atual].cor = 1;           // Vertice inicial -> primeira cor
    vetor[vertice_atual].verificado = true; // Coloca vertice como ja verificado

    while (contador < tamanho)
    { // Enquanto nao colorir todos faça
        contador = 0;
        cores_disp = cores_iniciais(tamanho); // Inicia o vetor de cores com as cores possiveis (MAX_CORES == TAMANHO)

        /*cout<<"\nCores: ";
        for(int i=0; i<tamanho; i++) cout<<" "<< cores_disp[i];
        cout<<"\n";
        imprime_vertices(vetor,tamanho);*/

        //cout<<"\n->"<<vertice_atual<<"\n";
        vertice_atual = prox_vertice(matriz, vetor, tamanho); // Atualiza vertice atual
        for (int i = 0; i < tamanho; i++)
            vetor[i].grau = 0; // Zera o grau

        for (int i = 0; i < tamanho; i++)
        { // Percorre o vetor de vertices
            if ((vetor[i].verificado) && (matriz[vertice_atual][i] == 1))
            { // SE ja possui cor e adjascente entao
                for (int k = 1; k < tamanho + 1; k++)
                { // Verifica qual a cor do vertice adj
                    if (cores_disp[k - 1] == vetor[i].cor)
                    {
                        cores_disp[k - 1] = -1; // Seta a cor do vertice adj como -1 - NAO PODE UTILIZAR ESSA COR
                        break;
                    }
                }
            }
        }

        for (int j = 0; j < tamanho; j++)
        { // Verifica qual a primeira cor disponivel do vetor de cores
            if (cores_disp[j] != -1)
            { // A primeira diferente de -1
                vetor[vertice_atual].cor = cores_disp[j];
                vetor[vertice_atual].verificado = true;
                //cout<<"Cor usada: "<<vetor[vertice_atual].cor<<"\n";
                break; // JA sai do laço quando achar a cor
            }
        }

        for (int i = 0; i < tamanho; i++)
        { // Verifica se todos os vertices estao coloridos
            if (vetor[i].verificado)
                contador++;
        }
    }
}

int maior_grau(Vertice *vetor, int tamanho)
{
    int maior = 0;
    for (int i = 1; i < tamanho; i++)
    {
        if (vetor[i].grau > vetor[maior].grau)
        {
            maior = i;
        }
    }
    return maior;
}

int grau_sub_grafo(int **matriz, Vertice *vetor, int tamanho, queue<int> iguais, int maior)
{
    Vertice *aux = new Vertice[tamanho];
    aux = vetor;
    int contador = 0, atual = maior;
    for (int i = 0; i < tamanho; i++)
    { // Verifica quantidade de vertices adj - PARA O MAIOR APENAS
        if ((matriz[atual][i] == 1) && (!vetor[i].verificado))
            contador++;
    }
    aux[atual].grau = contador;
    contador = 0;
    while (!iguais.empty())
    {
        atual = iguais.front();
        cout << "\natual=" << atual << "\n";
        getchar();
        for (int i = 0; i < tamanho; i++)
        { // Verifica quantidade de vertices adj
            if ((matriz[atual][i] == 1) && (!vetor[i].verificado))
                contador++;
        }
        aux[atual].grau = contador;
        contador = 0;
        iguais.pop();
    }
    return maior_grau(vetor, tamanho);
}

// Funcao que retorna o proximo vertice, a partir de grau de saturacao de cada vertice
int prox_vertice(int **matriz, Vertice *vetor, int tamanho)
{
    int contador = 0;
    vector<int> cores_disp = cores_iniciais(tamanho);
    for (int i = 0; i < tamanho; i++)
    { // Percorre o vetor de vertices
        if (!vetor[i].verificado)
        { // Se nao tem cor
            for (int k = 0; k < tamanho; k++)
            { // Verifica quantidade de vertices adj com cores
                if ((matriz[i][k] == 1) && (vetor[k].verificado))
                {
                    for (int j = 0; j < tamanho; j++)
                    {
                        if (vetor[k].cor == cores_disp[j])
                        { // Tira do vetor de cores as cores que estao no adj
                            contador++;
                            cores_disp[j] = -1;
                        }
                    }
                }
            }
            //cout<<"\nV:"<<i<<" - "<<contador<<"\n";
            vetor[i].grau = contador;             // Atualiza o grau de saturacao do vertice
            cores_disp = cores_iniciais(tamanho); // Reinicia o vetor de cores disponiveis
            contador = 0;
        }
    }
    return maior_grau(vetor, tamanho);
}

// Coloca todas as corres possives no vetor
vector<int> cores_iniciais(int tamanho)
{
    vector<int> aux;
    for (int i = 0; i < tamanho; i++)
    {
        aux.push_back(i + 1);
    }
    return aux;
}

void inicializa_vertices(Vertice *vetor, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i].cor = 0;
        vetor[i].grau = 0;
        vetor[i].verificado = false;
    }
}

int **inicializa_matriz_adj(int tamanho)
{
    int **matriz = new int *[tamanho];
    for (int i = 0; i < tamanho; i++)
    {
        matriz[i] = new int[tamanho];
        for (int j = 0; j < tamanho; j++)
        {
            matriz[i][j] = 0; //Inicializa com 0 a posicao
        }
    }
    return matriz;
}

void imprime_vertices(Vertice *vetor, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        cout << "Vertice:" << i << ", cor: " << vetor[i].cor << ", Verificado: " << vetor[i].verificado
             << ", Grau: " << vetor[i].grau << endl;
    }
}

void imprime_sudoku(Vertice *vetor, int tamanho)
{
    int cont = 0;
    for (int i = 0; i < tamanho; i++)
    {
        cout << "    " << vetor[i].cor << "   ";
        cont++;
        if (cont == 9)
        {
            cout << "\n\n";
            cont = 0;
        }
    }
    cout << "\n";
}

void imprime_matriz(int **matriz, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
        {
            cout << matriz[i][j] << "  ";
        }
        cout << "\n";
    }
    cout << "\n";
}
