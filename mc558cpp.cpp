#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

class Queue {
public:
    queue<void*> items;

    bool is_empty() {
        return items.empty();
    }

    void enqueue(void* item) {
        items.push(item);
    }

    void* dequeue() {
        if (!is_empty()) {
            void* item = items.front();
            items.pop();
            return item;
        } else {
            throw runtime_error("dequeue from empty queue");
        }
    }

    size_t size() {
        return items.size();
    }
};

class Vertice {
public:
    int valor;
    int presente;
    Vertice* ancestral;
    vector<Vertice*> vizinhos;
    int status;
    int dist;

    Vertice(int valor, int presente) : valor(valor), presente(presente), ancestral(nullptr), status(0), dist(0) {}

    void adicionar_vizinho(Vertice* vertice) {
        vizinhos.push_back(vertice);
    }

    vector<Vertice*> listar_vizinhos() {
        return vizinhos;
    }

    void definir_presente(int presente) {
        this->presente = presente;
    }

    int get_presente() {
        return presente;
    }

    void definir_ancestral(Vertice* vertice_pai) {
        ancestral = vertice_pai;
    }

    Vertice* get_ancestral() {
        return ancestral ? ancestral : this;
    }

    int get_status() {
        return status;
    }

    int get_valor() {
        return valor;
    }

    void definir_status(int status) {
        this->status = status;
    }

    void definir_dist(int dist) {
        this->dist = dist;
    }

    int get_dist() {
        return dist;
    }
};

class Grafo {
public:
    unordered_map<int, Vertice*> vertices;

    void adicionar_vertice(int valor, int presente) {
        if (vertices.find(valor) == vertices.end()) {
            vertices[valor] = new Vertice(valor, presente);
        }
    }

    void adicionar_aresta(int valor1, int valor2) {
        Vertice* vertice1 = vertices[valor1];
        Vertice* vertice2 = vertices[valor2];
        vertice1->adicionar_vizinho(vertice2);
        vertice2->adicionar_vizinho(vertice1);
    }

    void PreProcessing(int s = 1) {
        vertices[s]->definir_dist(0);
        Queue Q;
        Q.enqueue(vertices[s]);
        vertices[s]->definir_status(-1);
        while (!Q.is_empty()) {
            Vertice* u = static_cast<Vertice*>(Q.dequeue());
            for (Vertice* v : u->listar_vizinhos()) {
                if (v->get_status() == 0) {
                    v->definir_status(-1);
                    v->definir_dist(u->get_dist() + 1);
                    v->definir_ancestral(u);
                    Q.enqueue(v);
                }
            }
            u->definir_status(1);
        }
    }

    int problem(Vertice* v1, Vertice* v2) {
        if (v1 == v2) {
            return 1;
        } else {
            Vertice* v1_aux = v1;
            Vertice* v2_aux = v2;
            v1_aux->definir_status(0);
            v2_aux->definir_status(0);
            unordered_set<int> set_gifts;
            unordered_set<Vertice*> set_vertices;
            set_vertices.insert(v1_aux);
            set_vertices.insert(v2_aux);
            set_gifts.insert(v1_aux->get_presente());
            set_gifts.insert(v2_aux->get_presente());
            bool finish = false;
            while (!finish) {
                if (v1_aux->get_dist() >= v2_aux->get_dist()) {
                    if (v1_aux->get_ancestral()->get_status() == 0) {
                        finish = true;
                    } else {
                        v1_aux = v1_aux->get_ancestral();
                        v1_aux->definir_status(0);
                        set_gifts.insert(v1_aux->get_presente());
                        set_vertices.insert(v1_aux);
                    }
                } else {
                    if (v2_aux->get_ancestral()->get_status() == 0) {
                        finish = true;
                    } else {
                        v2_aux = v2_aux->get_ancestral();
                        v2_aux->definir_status(0);
                        set_gifts.insert(v2_aux->get_presente());
                        set_vertices.insert(v2_aux);
                    }
                }
            }
            for (Vertice* v : set_vertices) {
                v->definir_status(1);
            }
            return set_gifts.size();
        }
    }

    void ler_arestas(int num_arestas) {
        for (int i = 0; i < num_arestas; ++i) {
            int valor1, valor2;
            cin >> valor1 >> valor2;
            adicionar_aresta(valor1, valor2);
        }
        PreProcessing();
    }
};

void vertices_presentes(Grafo& grafo, const vector<int>& presentes) {
    for (size_t i = 0; i < presentes.size(); ++i) {
        grafo.adicionar_vertice(i + 1, presentes[i]);
    }
}

void ler_perguntas(Grafo& grafo, int num_perguntas) {
    for (int i = 0; i < num_perguntas; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        cout << grafo.problem(grafo.vertices[v1], grafo.vertices[v2]) << endl;
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> presentes(n);
    for (int i = 0; i < n; ++i) {
        cin >> presentes[i];
    }

    Grafo g;
    vertices_presentes(g, presentes);
    g.ler_arestas(n - 1);
    ler_perguntas(g, m);

    return 0;
}