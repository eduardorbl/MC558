#include <iostream> // Biblioteca para entrada e saída padrão
#include <vector> // Biblioteca para usar o vetor
#include <unordered_map> // Biblioteca para usar o mapa não ordenado
#include <unordered_set> // Biblioteca para usar o conjunto não ordenado
#include <queue> // Biblioteca para usar a fila
#include <string> // Biblioteca para usar strings
#include <sstream> // Biblioteca para usar string streams

// Classe que representa um vértice no grafo
class Vertice {
public:
    int valor; // Valor do vértice
    std::string presente; // Presente associado ao vértice
    Vertice* ancestral; // Ponteiro para o vértice ancestral
    std::vector<Vertice*> vizinhos; // Lista de vizinhos do vértice
    int status; // Status do vértice
    int dist; // Distância do vértice a partir do vértice inicial

    // Construtor da classe Vertice
    Vertice(int valor, const std::string& presente) 
        : valor(valor), presente(presente), ancestral(nullptr), status(0), dist(0) {}

    // Adiciona um vizinho ao vértice
    void adicionar_vizinho(Vertice* vertice) {
        vizinhos.push_back(vertice);
    }

    // Define o vértice ancestral
    void definir_ancestral(Vertice* vertice_pai) {
        ancestral = vertice_pai;
    }

    // Define a distância do vértice
    void definir_dist(int dist) {
        this->dist = dist;
    }

    // Define o presente do vértice
    void definir_presente(const std::string& presente) {
        this->presente = presente;
    }

    // Define o status do vértice
    void definir_status(int status) {
        this->status = status;
    }

    // Retorna o vértice ancestral
    Vertice* get_ancestral() {
        return ancestral ? ancestral : this;
    }

    // Retorna a distância do vértice
    int get_dist() const {
        return dist;
    }

    // Retorna o presente do vértice
    const std::string& get_presente() const {
        return presente;
    }

    // Retorna o status do vértice
    int get_status() const {
        return status;
    }

    // Retorna o valor do vértice
    int get_valor() const {
        return valor;
    }

    // Retorna a lista de vizinhos do vértice
    const std::vector<Vertice*>& listar_vizinhos() const {
        return vizinhos;
    }
};

// Classe que representa o grafo
class Grafo {
public:
    std::unordered_map<int, Vertice*> vertices; // Mapa de vértices

    // Destrutor da classe Grafo
    ~Grafo() {
        for (auto& pair : vertices) {
            delete pair.second;
        }
    }

    // Adiciona uma aresta entre dois vértices
    void adicionar_aresta(int valor1, int valor2) {
        Vertice* vertice1 = vertices[valor1];
        Vertice* vertice2 = vertices[valor2];
        vertice1->adicionar_vizinho(vertice2);
        vertice2->adicionar_vizinho(vertice1);
    }

    // Adiciona um vértice ao grafo
    void adicionar_vertice(int valor, const std::string& presente) {
        if (vertices.find(valor) == vertices.end()) {
            vertices[valor] = new Vertice(valor, presente);   // Cria um novo vértice com o valor e o presente
        }
    }

    // Lê as arestas do grafo
    void ler_arestas(int num_arestas) {
        for (int i = 0; i < num_arestas; ++i) {
            int valor1, valor2;
            std::cin >> valor1 >> valor2;
            adicionar_aresta(valor1, valor2);   // Adiciona a aresta entre os vértices de valor1 e valor2
        }
        processing();
    }

    // Lê as perguntas e responde a cada uma
    void ler_perguntas(int num_perguntas) {
        for (int i = 0; i < num_perguntas; ++i) {
            int v1, v2;
            std::cin >> v1 >> v2;
            std::cout << problem(vertices[v1], vertices[v2]) << std::endl;  // Resolve o problema entre os vértices v1 e v2
        }
    }

    // Processa o grafo a partir de um vértice inicial
    void processing(int s = 1) {
        Vertice* start_vertex = vertices[s];    // Vértice inicial
        start_vertex->definir_dist(0);  // Define a distância do vértice inicial como 0
        std::queue<Vertice*> Q; // Fila de vértices
        Q.push(start_vertex);   // Adiciona o vértice inicial à fila
        start_vertex->definir_status(-1);   // Define o status do vértice inicial como visitando

        while (!Q.empty()) {    // Enquanto a fila não estiver vazia
            Vertice* u = Q.front(); // Vértice u é o primeiro vértice da fila
            Q.pop();    // Remove o vértice u da fila
            int u_dist = u->get_dist(); // Distância do vértice u
            const auto& u_vizinhos = u->listar_vizinhos();  // Lista de vizinhos do vértice u

            for (Vertice* v : u_vizinhos) { // Para cada vizinho v do vértice u
                if (v->get_status() == 0) { // Se o vértice v não foi visitado
                    v->definir_status(-1);  // Define o status do vértice v como visitando
                    v->definir_dist(u_dist + 1);    // Define a distância do vértice v
                    v->definir_ancestral(u);    // Define o vértice u como ancestral do vértice v
                    Q.push(v);  // Adiciona o vértice v à fila
                }
            }

            u->definir_status(1);   // Define o status do vértice u como visitado
        }
    }

    // Resolve o problema entre dois vértices
    int problem(Vertice* v1, Vertice* v2) {
        if (v1 == v2) {
            return 1;
        } else {
            Vertice* v1_aux = v1;   // Vértice auxiliar para o vértice 1
            Vertice* v2_aux = v2;   // Vértice auxiliar para o vértice 2
            v1->definir_status(0);  // Define o status do vértice 1 como visitado
            v2->definir_status(0);  // Define o status do vértice 2 como visitado
            std::unordered_set<std::string> set_gifts = {v1->get_presente(), v2->get_presente()};   // Conjunto de presentes
            std::unordered_set<Vertice*> set_vertices = {v1, v2};   // Conjunto de vértices
            int v1_dist = v1_aux->get_dist();   // Distância do vértice 1
            int v2_dist = v2_aux->get_dist();   // Distância do vértice 2

            while (true) {
                if (v1_dist >= v2_dist) {   
                    Vertice* v1_ancestral = v1_aux->get_ancestral();    // Vértice ancestral do vértice 1
                    if (v1_ancestral->get_status() == 0) {  // Se o vértice ancestral do vértice 1 já foi visitado
                        break;  // Sai do loop
                    } else {
                        v1_aux = v1_ancestral;  // Atualiza o vértice auxiliar para o vértice 1
                        v1_aux->definir_status(0);  // Define o status do vértice 1 como visitado
                        set_gifts.insert(v1_aux->get_presente());   // Insere o presente do vértice 1 no conjunto de presentes
                        set_vertices.insert(v1_aux);    // Insere o vértice 1 no conjunto de vértices
                        v1_dist = v1_aux->get_dist();   // Atualiza a distância do vértice 1
                    }
                } else {
                    Vertice* v2_ancestral = v2_aux->get_ancestral();    // Vértice ancestral do vértice 2
                    if (v2_ancestral->get_status() == 0) {  // Se o vértice ancestral do vértice 2 já foi visitado
                        break;  // Sai do loop
                    } else {
                        v2_aux = v2_ancestral;  // Atualiza o vértice auxiliar para o vértice 2
                        v2_aux->definir_status(0);  // Define o status do vértice 2 como visitado
                        set_gifts.insert(v2_aux->get_presente());   // Insere o presente do vértice 2 no conjunto de presentes
                        set_vertices.insert(v2_aux);    // Insere o vértice 2 no conjunto de vértices
                        v2_dist = v2_aux->get_dist();   // Atualiza a distância do vértice 2
                    }
                }
            }

            for (Vertice* v : set_vertices) {   // Para cada vértice no conjunto de vértices visitados
                v->definir_status(1);   // Define o status do vértice como não visitado
            }
            return set_gifts.size();    // Retorna o tamanho do conjunto de presentes
        }
    }

    // Adiciona os presentes aos vértices
    void vertices_presentes(const std::vector<std::string>& presentes) {    
        for (size_t i = 0; i < presentes.size(); ++i) {
            adicionar_vertice(i + 1, presentes[i]);   // Adiciona o vértice da casa i + 1 com o presente i
        }
    }
};

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::string> presentes(n);  // Vetor de presentes
    for (int i = 0; i < n; ++i) {
        std::cin >> presentes[i];   // Lê o presente da casa i
    }

    Grafo g;
    g.vertices_presentes(presentes);    // Cria os vértices
    g.ler_arestas(n - 1);   // Lê as arestas do grafo em input
    g.ler_perguntas(m); // Lê as perguntas e responde a cada uma

    return 0;
}