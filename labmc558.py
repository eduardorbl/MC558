class Queue:
    def __init__(self):
        self.items = {}
        self.front = 0
        self.rear = 0

    def is_empty(self):
        return self.front == self.rear

    def enqueue(self, item):
        self.items[self.rear] = item
        self.rear += 1

    def dequeue(self):
        if not self.is_empty():
            item = self.items.pop(self.front)
            self.front += 1
            return item
        else:
            raise IndexError("dequeue from empty queue")

    def size(self):
        return self.rear - self.front

class Vertice:
    def __init__(self, valor, presente):
        self.valor = valor
        self.presente = presente
        self.ancestral = None  # Ancestral (ou pai) do vértice
        self.vizinhos = []  # Lista de vértices conectados (adjacentes)
        self.status = 0  # Status do vértice (0 = não visitado, 1 = visitado, -1 = em processamento)
        self.dist = 0  # Distância do vértice até a raiz

    def listar_vizinhos(self):
        return self.vizinhos

    def definir_presente(self, presente):
        self.presente = presente

    def get_presente(self):
        return self.presente
    
    def adicionar_vizinho(self, vertice):
        self.vizinhos.append(vertice)

    def definir_ancestral(self, vertice_pai):
        self.ancestral = vertice_pai
    
    def get_ancestral(self):
        if self.ancestral == None:
            return self
        else:
            return self.ancestral
    
    def get_status(self):
        return self.status
    
    def get_valor(self):
        return self.valor
    
    def definir_status(self, status):
        self.status = status

    def definir_dist(self, dist):
        self.dist = dist
    
    def get_dist(self):
        return self.dist

class Grafo:
    def __init__(self):
        self.vertices = {}

    def adicionar_vertice(self, valor, presente):
        if valor not in self.vertices:
            self.vertices[valor] = Vertice(valor, presente)

    def adicionar_aresta(self, valor1, valor2):
        vertice1 = self.vertices[valor1]
        vertice2 = self.vertices[valor2]
        vertice1.adicionar_vizinho(vertice2)
        vertice2.adicionar_vizinho(vertice1)
    
    def exibir_grafo(self):
        for valor, vertice in self.vertices.items():
            ances = vertice.get_ancestral().valor if vertice.get_ancestral() else "Nenhum"
            adjacencias = [v.valor for v in vertice.listar_vizinhos()]
            print(f"Vértice {valor}: Adjacências {adjacencias}, Ancestral: {ances}")
        
    def PreProcessing(self, s=1):
        self.vertices[s].definir_dist(0)
        Q = Queue()
        Q.enqueue(self.vertices[s])
        self.vertices[s].definir_status(-1)
        while not Q.is_empty():
            u = Q.dequeue()
            for v in u.listar_vizinhos():
                if v.get_status() == 0:
                    v.definir_status(-1)
                    v.definir_dist(u.get_dist() + 1)
                    v.definir_ancestral(u)
                    Q.enqueue(v)
            u.definir_status(1)
        
    def problem(self, v1, v2):
        if (v1 == v2):
            return 1
        else:
            v1_aux = v1
            v2_aux = v2
            v1_aux.definir_status(0)
            v2_aux.definir_status(0)
            set_gifts = set()
            set_gifts.add(v1_aux.get_presente())
            set_gifts.add(v2_aux.get_presente())
            finish = False
            while not finish:
                if (v1_aux.get_dist() >= v2_aux.get_dist()):
                    if (v1_aux.get_ancestral().get_status() == 0):
                        finish = True
                    else:
                        v1_aux = v1_aux.get_ancestral()
                        v1_aux.definir_status(0)
                        set_gifts.add(v1_aux.get_presente())
                elif (v1_aux.get_dist() < v2_aux.get_dist()):
                    if (v2_aux.get_ancestral().get_status() == 0):
                        finish = True
                    else:
                        v2_aux = v2_aux.get_ancestral()
                        v2_aux.definir_status(0)
                        set_gifts.add(v2_aux.get_presente())
        for v in self.vertices.values():
            v.definir_status(1)

        return len(set_gifts)

    def ler_arestas(self, num_arestas):
        for _ in range(num_arestas):
            entrada = input()
            valor1, valor2 = map(int, entrada.split())
            self.adicionar_aresta(valor1, valor2)
        self.PreProcessing()

def vertices_presentes(grafo, presentes):
    for i in range(1, len(presentes) + 1):
        grafo.adicionar_vertice(i, presentes[i - 1])

def ler_perguntas(grafo, num_perguntas):
    for _ in range(num_perguntas):
        v1, v2 = map(int, input().split())
        print(grafo.problem(grafo.vertices[v1], grafo.vertices[v2]))

def __main__():
    try:
        n, m = map(int, input().split())
    except ValueError:
        print("Erro: Por favor, insira dois números inteiros separados por espaço.")
        return

    presentes = input().split()
    g = Grafo()
    
    vertices_presentes(g, presentes)
    g.ler_arestas(n - 1)
    ler_perguntas(g, m)

if __name__ == "__main__":
    __main__()