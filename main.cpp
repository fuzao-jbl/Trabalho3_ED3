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
        {
            Grafo grafo(arquivo);
            grafo.printa_grafo();
            break;
        }
        case 11:
        {
            Grafo grafo(arquivo);
            int numero_presas;
            std::cin >> numero_presas;
            for (int i = 0; i < numero_presas; i++)
            {
                std::string presa;
                std::getline(std::cin, presa, '"'); // descarta o primeiro ' "' de todo string
                std::getline(std::cin, presa, '"');
                grafo.printa_cacadores(presa);
                std::cout << std::endl;
            }
            break;
        }
    }

    // fecha o arquivo
    arquivo.close();

    return 0;
}
