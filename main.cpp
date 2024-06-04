#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <queue>
#include <algorithm>
#include <conio.h>
#include <string>
#include <climits>

class Grafo {
    private:
        int V;
        std::vector<std::vector<int>> adyacentes;
        std::map<std::pair<int, int>, std::pair<int, bool>> pesos; // {u, v} -> {peso, disponibilidad}

    public:
        Grafo(int V) {
            this->V = V;
            adyacentes = std::vector<std::vector<int>>(V);
        }

        int getNumeroVertices() {
            return V;
        }

        std::pair<int, bool> getPeso(int u, int v) {
            return pesos[{u, v}];
        }

        void agregarArista(int u, int v, int peso, bool disponibilidad = true) {
            adyacentes[u].push_back(v);
            pesos[{u, v}] = {peso, disponibilidad}; 
        }

        void setDisponibilidad(int u, int v, bool disponibilidad) {
            if (pesos.find({u, v}) != pesos.end()) {
                pesos[{u, v}].second = disponibilidad;
            }
        }

        std::vector<int> getAdyacentes(int nodo) {
            return adyacentes[nodo];
        }
};

class ColaPrioridad {
    private:
        std::multiset<std::pair<int, int>> elementos;
    public:
        ColaPrioridad() {
            std::multiset<std::pair<int, int>> elementos;
        }

        void insertar(int elemento, int prioridad) {
            elementos.insert({prioridad, elemento});
        }

        int extraerMinimo() {
            int minimo = elementos.begin()->second; //Acceder al segundo elemento del pair del primer elemento
            elementos.erase(elementos.begin()); //Eliminar el primer elemento  
            return minimo;
        }

        void insertarODecrementar(int elemento, int nuevaPrioridad) {
            elementos.erase({nuevaPrioridad, elemento});
            elementos.insert({nuevaPrioridad, elemento});
        }

        void eliminar(int elemento) {
            elementos.erase({0, elemento}); 
        }

        int encontrarElemento(int elemento) {
            auto it = elementos.find({0, elemento});
            if (it != elementos.end()) {
                return std::distance(elementos.begin(), it); //indice
            }
            return -1;
        }

        const std::multiset<std::pair<int, int>>& getElementos() {
            return elementos;
        }

        bool isEmpty() {
            return elementos.empty();
        }
};

std::pair<std::vector<int>, std::vector<int>> Dijkstra (Grafo G, int origen) {
    int V = G.getNumeroVertices();
    std::vector<int> distancias(V, INT_MAX);
    std::vector<int> previo(V, -1);
    std::vector<bool> visitado(V, false);
    ColaPrioridad colaPrioridad;

    distancias[origen] = 0;
    colaPrioridad.insertar(origen, 0);

    while (!colaPrioridad.isEmpty()) {
        int actual = colaPrioridad.extraerMinimo();
        visitado[actual] = true;

        for (int vecino : G.getAdyacentes(actual)) {
            auto [costo, disponibilidad] = G.getPeso(actual, vecino);
            if (disponibilidad && !visitado[vecino]) {
                int nuevaDistancia = distancias[actual] + costo;

                if (nuevaDistancia < distancias[vecino]) {
                    distancias[vecino] = nuevaDistancia;
                    previo[vecino] = actual;
                    colaPrioridad.insertarODecrementar(vecino, nuevaDistancia);
                }
            }
        }
    }

    return {distancias, previo};
}

std::vector<int> reconstruirCamino(const std::vector<int>& previo, int destino) { 
    std::vector<int> caminoDestino;
    for (int nodo = destino; nodo != -1; nodo = previo[nodo]) {
        caminoDestino.push_back(nodo);
    }
    std::reverse(caminoDestino.begin(), caminoDestino.end());
    return caminoDestino;
}

void mostrarMenu() {
    std::cout << "Menu:" << std::endl;
    std::cout << "1. Inicializar grafo" << std::endl;
    std::cout << "2. Agregar arista" << std::endl;
    std::cout << "3. Establecer disponibilidad de arista a false" << std::endl;
    std::cout << "4. Salir" << std::endl;
    std::cout << "Ingrese una opcion: ";
}

bool Numero(const std::string& linea) {
    if (linea.empty()) {
        return false;
    }

    int indice = 0;
    if (linea[0] == '+' || linea[0] == '-') {
        if (linea.size() == 1) {
            return false;
        }
        indice = 1;
    }

    for (int i = indice; i < linea.size(); ++i) {
        if (!isdigit(linea[i])) {
            return false;
        }
    }

    return true;
}

void llenarNumeroInt(int& n) {
	std::string dato;
	int num;
	bool correcto = false;

	do {
		std::cout << "---> ";
		std::cin >> dato;

		if (Numero(dato)) {
			correcto = true;
			num = atoi(dato.c_str()); // convierte de cadena a entero (puntero de caracteres)
		}

		else {
			correcto = false;
			std::cout << "El valor ingresado no es correcto." << std::endl;
		}
	} while (correcto == false);

	n = num;
}


int main() {
    Grafo* grafoActual = nullptr;
    int opcion;
    while (true) {
        mostrarMenu();
        std::cin >> opcion;
        if (opcion == 1) {
            if(grafoActual == nullptr){
                int numNodos = 5; 
                delete grafoActual; // Liberar la memoria del grafo anterior si existe
                grafoActual = new Grafo(numNodos);
                grafoActual->agregarArista(0, 1, 10);
                grafoActual->agregarArista(0, 3, 5);
                grafoActual->agregarArista(1, 2, 1);
                grafoActual->agregarArista(1, 3, 2);
                grafoActual->agregarArista(2, 4, 4);
                grafoActual->agregarArista(3, 1, 3);
                grafoActual->agregarArista(3, 2, 9);
                grafoActual->agregarArista(3, 4, 2);
                grafoActual->agregarArista(4, 0, 7);
                grafoActual->agregarArista(4, 2, 6);
                std::cout << "Grafo inicializado con " << numNodos << " nodos." << std::endl;
                std::cout << "Pulse cualquier botón para continuar..."<<std::endl;
                int origen = 0; 
                int destino = 2; 

                std::pair<std::vector<int>, std::vector<int>> resultado = Dijkstra(*grafoActual, origen);
                std::vector<int> distancias = resultado.first;
                std::vector<int> previo = resultado.second;
                std::vector<int> camino = reconstruirCamino(previo, destino);

                std::cout << "Origen: " << origen << "-> Destino: " << destino << std::endl;
                for (int nodo : camino) {
                    std::cout << nodo << " ";
                }
                std::cout <<"\nPeso mínimo del camino encontrado: "<<distancias[destino]<< std::endl;
                getch();
                system("cls"); // Clear the console screen
            }else{
                std::cout << "El grafo ya ha sido inicializado." << std::endl;
                getch();
                system("cls"); // Clear the console screen
            }
        } else if (opcion == 2) {
            if (grafoActual == nullptr) {
                std::cout << "Primero inicialice el grafo." << std::endl;
            } else {
                int u, v, peso;
                std::cout << "Ingrese el nodo origen: ";
                llenarNumeroInt(u);
                std::cout << "Ingrese el nodo destino: ";
                llenarNumeroInt(v);
                std::cout << "Ingrese el peso de la arista: ";
                llenarNumeroInt(peso);
                grafoActual->agregarArista(u, v, peso);
                std::cout << "Arista agregada de " << u << " a " << v << " con peso " << peso << "." << std::endl;
            }
        } else if (opcion == 3) {
            if (grafoActual == nullptr) {
                std::cout << "Primero inicialice el grafo." << std::endl;
            } else {
                int u, v;
                std::cout << "Ingrese el nodo origen: ";
                llenarNumeroInt(u);
                std::cout << "Ingrese el nodo destino: ";
                llenarNumeroInt(v);
                grafoActual->setDisponibilidad(u, v, false);
                std::cout << "Disponibilidad de la arista de " << u << " a " << v << " establecida a false." << std::endl;
            }
        } else if (opcion == 4) {
            break;
        } else {
            std::cout << "Opcion invalida. Intente de nuevo." << std::endl;
        }
    }
    
    delete grafoActual; // Liberar la memoria del grafo al final
    return 0;
}
