#include <iostream>
#include <unordered_map>
#include <vector>
#include <deque>
#include <set>
#include <limits>
#include <stdexcept>

using namespace std;

class Queue {
private:
    deque<int> items;

public:
    bool is_empty() {
        return items.empty();
    }

    void enqueue(int item) {
        items.push_back(item);
    }

    int dequeue() {
        if (!is_empty()) {
            int item = items.front();
            items.pop_front();
            return item;
        } else {
            throw out_of_range("dequeue from empty queue");
        }
    }

    int size() {
        return items.size();
    }
};

class Vertice {
public:
    int valor;
    string presente;
    Vertice* ancestral;
    vector<Vertice*> vizinhos;
    int status;
    int dist;

    Vertice(int valor, string presente) : valor(valor), presente(presente), ancestral(nullptr), status(0), dist(numeric_limits<int>::max()) {}

    void adicionar_vizinho(Vertice* vertice) {
        vizinhos.push_back(vertice);
    }

    Vertice* get_ancestral() {
        return ancestral ? ancestral : this;
    }
};

class Grafo {
private:
    unordered_map<int, Vertice*> vertices;

public:
    void adicionar_vertice(int valor, string presente) {
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
        vertices[s]->dist = 0;
        Queue Q;
        Q.enqueue(s);
        vertices[s]->status = -1;

        while (!Q.is_empty()) {
            int u_val = Q.dequeue();
            Vertice* u = vertices[u_val];

            for (Vertice* v : u->vizinhos) {
                if (v->status == 0) {
                    v->status = -1;
                    v->dist = u->dist + 1;
                    v->ancestral = u;
                    Q.enqueue(v->valor);
                }
            }
            u->status = 1;
        }
    }

    int problem(Vertice* v1, Vertice* v2) {
        if (v1 == v2) {
            return 1;
        } else {
            Vertice* v1_aux = v1;
            Vertice* v2_aux = v2;
            v1_aux->status = 0;
            v2_aux->status = 0;
            set<string> set_gifts;
            set_gifts.insert(v1_aux->presente);
            set_gifts.insert(v2_aux->presente);
            bool finish = false;

            while (!finish) {
                if (v1_aux->dist >= v2_aux->dist) {
                    if (v1_aux->get_ancestral()->status == 0) {
                        finish = true;
                    } else {
                        v1_aux = v1_aux->get_ancestral();
                        v1_aux->status = 0;
                        set_gifts.insert(v1_aux->presente);
                    }
                } else {
                    if (v2_aux->get_ancestral()->status == 0) {
                        finish = true;
                    } else {
                        v2_aux = v2_aux->get_ancestral();
                        v2_aux->status = 0;
                        set_gifts.insert(v2_aux->presente);
                    }
                }
            }

            for (auto& pair : vertices) {
                pair.second->status = 1;
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

    Vertice* get_vertice(int valor) {
        return vertices[valor];
    }
};

void vertices_presentes(Grafo& grafo, vector<string>& presentes) {
    for (int i = 0; i < presentes.size(); ++i) {
        grafo.adicionar_vertice(i + 1, presentes[i]);
    }
}

void ler_perguntas(Grafo& grafo, int num_perguntas) {
    for (int i = 0; i < num_perguntas; ++i) {
        int v1, v2;
        cin >> v1 >> v2;
        cout << grafo.problem(grafo.get_vertice(v1), grafo.get_vertice(v2)) << endl;
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<string> presentes(n);
    for (int i = 0; i < n; ++i) {
        cin >> presentes[i];
    }

    Grafo g;
    vertices_presentes(g, presentes);
    g.ler_arestas(n - 1);
    ler_perguntas(g, m);

    return 0;
}