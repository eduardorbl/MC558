from collections import deque

class Vertice:
    def __init__(self, valor, presente):
        self.valor = valor
        self.presente = presente
        self.ancestral = None  # Ancestral (ou pai) do vértice
        self.vizinhos = []  # Lista de vértices conectados (adjacentes)
        self.status = 0  # Status do vértice (0 = não visitado, 1 = visitado, -1 = em processamento)
        self.dist = 0  # Distância do vértice até a raiz

    def adicionar_vizinho(self, vertice):
        self.vizinhos.append(vertice)

    def definir_ancestral(self, vertice_pai):
        self.ancestral = vertice_pai

    def definir_dist(self, dist):
        self.dist = dist

    def definir_presente(self, presente):
        self.presente = presente

    def definir_status(self, status):
        self.status = status

    def get_ancestral(self):
        return self.ancestral if self.ancestral else self

    def get_dist(self):
        return self.dist

    def get_presente(self):
        return self.presente

    def get_status(self):
        return self.status

    def get_valor(self):
        return self.valor

    def listar_vizinhos(self):
        return self.vizinhos

class Grafo:
    def __init__(self):
        self.vertices = {}

    def adicionar_aresta(self, valor1, valor2):
        vertice1 = self.vertices[valor1]
        vertice2 = self.vertices[valor2]
        vertice1.adicionar_vizinho(vertice2)
        vertice2.adicionar_vizinho(vertice1)

    def adicionar_vertice(self, valor, presente):
        if valor not in self.vertices:
            self.vertices[valor] = Vertice(valor, presente)

    def exibir_grafo(self):
        for valor, vertice in self.vertices.items():
            ances = vertice.get_ancestral().valor if vertice.get_ancestral() else "Nenhum"
            adjacencias = [v.valor for v in vertice.listar_vizinhos()]
            print(f"Vértice {valor}: Adjacências {adjacencias}, Ancestral: {ances}")

    def ler_arestas(self, num_arestas):
        for _ in range(num_arestas):
            entrada = input()
            valor1, valor2 = map(int, entrada.split())
            self.adicionar_aresta(valor1, valor2)
        self.processing()

    def ler_perguntas(self, num_perguntas):
        for _ in range(num_perguntas):
            v1, v2 = map(int, input().split())
            print(self.problem(self.vertices[v1], self.vertices[v2]))

    def processing(self, s=1):
        start_vertex = self.vertices[s]
        start_vertex.definir_dist(0)
        Q = deque([start_vertex])
        start_vertex.definir_status(-1)
        
        while Q:
            u = Q.popleft()
            u_dist = u.get_dist()
            u_vizinhos = u.listar_vizinhos()
            
            for v in u_vizinhos:
                if v.get_status() == 0:
                    v.definir_status(-1)
                    v.definir_dist(u_dist + 1)
                    v.definir_ancestral(u)
                    Q.append(v)
            
            u.definir_status(1)

    def problem(self, v1, v2):
        if (v1 == v2):
            return 1
        else:
            v1_aux, v2_aux = v1, v2
            v1.definir_status(0)
            v2.definir_status(0)
            set_gifts = {v1.get_presente(), v2.get_presente()}
            set_vertices = {v1, v2}
            v1_dist, v2_dist = v1_aux.get_dist(), v2_aux.get_dist()
            while True:
                if v1_dist >= v2_dist:
                    v1_ancestral = v1_aux.get_ancestral()
                    if v1_ancestral.get_status() == 0:
                        break
                    else:
                        v1_aux = v1_ancestral
                        v1_aux.definir_status(0)
                        set_gifts.add(v1_aux.get_presente())
                        set_vertices.add(v1_aux)
                        v1_dist = v1_aux.get_dist()  # Atualiza a distância de v1_aux
                else:
                    v2_ancestral = v2_aux.get_ancestral()
                    if v2_ancestral.get_status() == 0:
                        break
                    else:
                        v2_aux = v2_ancestral
                        v2_aux.definir_status(0)
                        set_gifts.add(v2_aux.get_presente())
                        set_vertices.add(v2_aux)
                        v2_dist = v2_aux.get_dist()  # Atualiza a distância de v2_aux

            for v in set_vertices:
                v.definir_status(1)
            return len(set_gifts)

    def vertices_presentes(self, presentes):
        for i in range(1, len(presentes) + 1):
            self.adicionar_vertice(i, presentes[i - 1])

def __main__():
    n, m = map(int, input().split())
    presentes = input().split()

    g = Grafo()

    g.vertices_presentes(presentes)
    g.ler_arestas(n - 1)
    g.ler_perguntas(m)

if __name__ == "__main__":
    __main__()