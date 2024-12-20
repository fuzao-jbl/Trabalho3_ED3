// Pedro Fuziwara Filho - 13676840

#include "grafo.hpp"

Aresta::Aresta(Registro origem)
{
    _peso = origem.populacao();
    _nome = origem.alimento();
}

void Aresta::printa()
{
    std::cout << _nome << " " << _peso << std::endl;
}

Aresta& Aresta::operator=(const Aresta& aresta)
{
    if (this == &aresta) return *this;
    _peso = aresta._peso;
    _nome = aresta._nome;
    return *this;
}

Vertice::Vertice(Registro especie)
{
    _nome = especie.nome();
    _especie = especie.especie();
    _dieta = especie.dieta();
    _tipo = especie.tipo();
    _habitat = especie.habitat();
    _grau_entrada = 0;
    _grau_saida = 0;
    _grau = 0;
}

Vertice::Vertice(Vertice&& vertice) noexcept
    : _nome(std::move(vertice._nome)), _especie(std::move(vertice._especie)),
    _dieta(std::move(vertice._dieta)), _tipo(std::move(vertice._tipo)),
    _habitat(std::move(vertice._habitat)), _grau_entrada(vertice._grau_entrada),
    _grau_saida(vertice._grau_saida), _grau(vertice._grau), 
    _arestas(std::move(vertice._arestas))
{
    vertice._grau_entrada = 0;
    vertice._grau_saida = 0;
    vertice._grau = 0;
}

Vertice::Vertice(const Vertice& vertice)
    : _nome(vertice._nome), _especie(vertice._especie), _dieta(vertice._dieta),
    _tipo(vertice._tipo), _habitat(vertice._habitat), _grau_entrada(vertice._grau_entrada),
    _grau_saida(vertice._grau_saida), _grau(vertice._grau), _arestas(vertice._arestas) {}

Vertice& Vertice::operator=(const Vertice& vertice)
{
    if (this == &vertice) return *this;
    _nome = std::move(vertice._nome);
    _especie = std::move(vertice._especie);
    _dieta = std::move(vertice._dieta);
    _tipo = std::move(vertice._tipo);
    _habitat = std::move(vertice._habitat);
    _arestas = std::move(vertice._arestas);
    _grau_entrada = _grau_entrada;
    _grau_saida = _grau_saida;
    _grau = _grau;
    return *this;
}

void Vertice::printa_vertice()
{
    std::cout << _nome << ", " << _especie << ", " << _habitat
              << ", " << _dieta << ", " << _tipo << ", "
              << _grau_entrada << ", " << _grau_saida 
              << ", " << _grau << std::endl;
}

void Vertice::ordena_arestas()
{
    struct
    {
        bool operator()(Aresta a, Aresta b){return a.nome() < b.nome();}
    } compara_aresta;
    _arestas.sort(compara_aresta);
}

void Vertice::insertion_sort(Aresta aresta)
{
    if (_arestas.empty())
        _arestas.push_front(aresta);
    else
    {
        auto posicao = _arestas.begin();
        for (; posicao != _arestas.end(); posicao++)
            if (posicao->nome() >= aresta.nome())
                break;
        _arestas.insert(posicao, aresta);
     }
}

void Vertice::poe_no_fim(const Aresta& aresta)
{
    _arestas.push_back(aresta);
}

bool Vertice::caca(std::string presa)
{
    for (Aresta aresta : _arestas)
        if (aresta.nome() == presa)
            return true;
    return false;
}

Grafo::Grafo(std::ifstream& arquivo)
{
    // lemos registros e ordenamos a partir do nome
    arquivo.seekg(0, std::ios::end);
    int tamanho = arquivo.tellg();
    std::vector<Registro> registros;
    for (int offset = 1600, i = 0; offset < tamanho; offset += 160, i++)
    {
        Registro registro(arquivo, offset);
        registros.push_back(registro);
    }
    // precisamos deixar em ordem alfabetica
    // https://en.cppreference.com/w/cpp/algorithm/sort
    struct
    {
        bool operator()(Registro a, Registro b){return a.nome() < b.nome();}
    } compara_nome;    
    std::sort(registros.begin(), registros.end(), compara_nome);
    // criamos um grafo com os registros ordenados, no momento ele so possui os vertice
    // sem nenhuma aresta
    for (auto registro = registros.begin(); registro != registros.end(); registro++)
    {
        if (registro == registros.begin())
            _adjacencias.push_back(Vertice(*registro));
        else if (_adjacencias.back().nome() != registro->nome())
            _adjacencias.push_back(Vertice(*registro));
    }
    // vamos inserir as arestas
    for (long unsigned int i = 0; i < registros.size(); i++)
    {
        Aresta aresta(registros[i]);
        int origem;
        // elemento repetido
        for (origem = 0; origem < this->tamanho(); origem++)
            if (_adjacencias[origem].nome() == registros[i].nome())
                break;
        // faz fotossintese
        if (aresta.nome() == "sunlight")
            this->insere_sunlight(aresta, origem);
        // aresta existe
        else if (aresta.peso() != -1)
        {
            // para cada alimento, precisamos buscar destino na lista encadeada
            int destino;
            for (destino = 0; destino < this->tamanho(); destino++)
                if (this->vertice(destino).nome() == aresta.nome())
                    break;
            this->insere_aresta(aresta, origem, destino);
        }
    }
}

void Grafo::insere_sunlight(Aresta aresta, int origem)
{
    for (auto atual : _adjacencias[origem].arestas())
        if (atual.nome() == aresta.nome())
            return;
    _adjacencias[origem].insertion_sort(aresta);
    _adjacencias[origem].incrementa_saida();
    _adjacencias[origem].incrementa_grau();
}

void Grafo::insere_aresta(Aresta aresta, int origem, int destino)
{
    //vemos se a aresta eh valida
    if (aresta.peso() == -1 || aresta.nome() == "")
        return;
    for (auto atual : _adjacencias[origem].arestas())
        if (atual.nome() == aresta.nome())
        {
            aresta.printa();
            return;
        }
    _adjacencias[origem].insertion_sort(aresta);
    _adjacencias[origem].incrementa_saida();
    _adjacencias[origem].incrementa_grau();
    _adjacencias[destino].incrementa_entrada();
    _adjacencias[destino].incrementa_grau();
}

void Grafo::printa_grafo()
{
    for (auto vertice : _adjacencias)
    {
        for (auto aresta : vertice.arestas()) 
        {
            std::cout << vertice.nome() << " " << vertice.especie() << " " << vertice.habitat()
                      << " " << vertice.dieta() << " " << vertice.tipo() << " "
                      << vertice.grau_entrada() << " " << vertice.grau_saida() << " " << vertice.grau()
                      << " " << aresta.nome() << " " << aresta.peso() << "\n";
        }
    }
}

void Grafo::escreve(std::string nome)
{
    std::ofstream arquivo;
    arquivo.open(nome, std::ofstream::out | std::ofstream::trunc);
    for (auto vertice : _adjacencias)
    {
        for (auto aresta : vertice.arestas()) 
        {
            arquivo << vertice.nome() << " " << vertice.especie() << " " << vertice.habitat()
                      << " " << vertice.dieta() << " " << vertice.tipo() << " "
                      << vertice.grau_entrada() << " " << vertice.grau_saida() << " " << vertice.grau()
                      << " " << aresta.nome() << " " << aresta.peso() << "\n";
        }
    }
    arquivo << std::endl;
    arquivo.close();
}


void Grafo::printa_vertices()
{
    for (auto vertice : _adjacencias)
    {
        std::cout << vertice.nome() << ", " << vertice.especie() << ", " << vertice.habitat()
                  << ", " << vertice.dieta() << ", " << vertice.tipo() << ", "
                  << vertice.grau_entrada() << ", " << vertice.grau_saida() 
                  << ", " << vertice.grau() << std::endl;
    } 
}

void Grafo::printa_cacadores(std::string presa)
{
    // buscamos cacadores
    std::vector<std::string> cacadores;
    for (int i = 0; i < this->tamanho(); i++)
        if (_adjacencias[i].caca(presa))
            cacadores.push_back(_adjacencias[i].nome());
    // checa se vazio
    if (cacadores.empty())
    {
        std::cout << "Registro Inexistente." << std::endl;
        return;
    }
    std::sort(cacadores.begin(), cacadores.end());
    // printamos cacadores
    std::cout << presa << ": ";
    for (long unsigned int i = 0; i < cacadores.size(); i++)
    {
       if (i != cacadores.size() - 1)
          std::cout << cacadores[i] << ", ";
       else
          std::cout << cacadores[i] << std::endl; 
    }
}
