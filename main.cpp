// Pedro Fuziwara Filho - 13676840

#include "registro.hpp"
#include "grafo.hpp"

int main()
{
    // primeiro recebemos as informacoes que precisamos
    int n;
    std::string nome;
    std::cin >> n >> nome;

    // busca o arquivo
    std::ifstream arquivo;
    arquivo.open(nome);
    if (!arquivo.is_open())
    {
        printf("Falha no processamento do arquivo\n");
        return 1;
    }

    // executa funcionalidade desejada
    switch (n)
    {
        case 10:
            Grafo grafo(arquivo);
            grafo.printa_grafo();
            break;
    }

    // fecha o arquivo
    arquivo.close();

    return 0;
}
