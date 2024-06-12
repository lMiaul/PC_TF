#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <queue>
#include <algorithm>
#include <string>
#include <climits>
#include <C:\Users\mauri\AppData\Local\Programs\Python\Python311\include\Python.h> // Asegúrate de que la ruta de la cabecera sea correcta

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

        std::vector<int> getAdyacentes(int nodo) {
            return adyacentes[nodo];
        }

        std::pair<int, bool> getPesoYDisponibilidad(int u, int v) {
            return pesos[{u, v}];
        }

        void agregarArista(int u, int v, int peso, bool disponibilidad = true) {
            adyacentes[u].push_back(v);
            pesos[{u, v}] = {peso, disponibilidad}; 
        }

        void setDisponibilidad(int u, int v, bool disponibilidad) {
            if (pesos.find({u, v}) != pesos.end()) {
                pesos[{u, v}].second = disponibilidad;
                std::cout << "Disponibilidad de la arista de " << u << " a " << v << " establecida a false." << std::endl;
            } else {
                std::cout << "La tuberia de " << u << " a " << v << " no existe." << std::endl;
            }
        }

};

class ColaPrioridad {
    private:
        std::multiset<std::pair<int, int>> elementos;
    public:
        ColaPrioridad() {}

        void insertar(int elemento, int prioridad) {
            elementos.insert({prioridad, elemento});
        }

        int extraerMinimo() {
            int minimo = elementos.begin()->second; //Acceder al segundo elemento del pair del primer elemento
            elementos.erase(elementos.begin()); //Eliminar el primer elemento  
            return minimo;
        }

        void insertarODecrementar(int elemento, int nuevaPrioridad) {
            for(auto it = elementos.begin(); it != elementos.end(); ++it) {
                if(it->second == elemento) {
                    elementos.erase(it);
                    break;
                }
            }
            elementos.insert({nuevaPrioridad, elemento});
        }

        int encontrarElemento(int elemento) {
            for(auto it = elementos.begin(); it != elementos.end(); ++it) {
                if(it->second == elemento) {
                    return std::distance(elementos.begin(), it); //indice
                }
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

std::pair<std::vector<int>, std::vector<int>> Dijkstra(Grafo G, int origen) {
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
            auto [costo, disponibilidad] = G.getPesoYDisponibilidad(actual, vecino);
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
    std::cout << "1. Inicializar mapa." << std::endl;
    std::cout << "2. Agregar arista." << std::endl;
    std::cout << "3. Establecer disponibilidad de tuberia a No Disponible." << std::endl;
    std::cout << "4. Ver mapa actual." << std::endl;
    std::cout << "5. Calcular rutas optimas de cada pozo/centro de agua." << std::endl;
    std::cout << "6. Salir" << std::endl;
    std::cout << "Ingrese una opcion: ";
}

void callPythonFunction(PyObject* pModule, const std::string& funcName, PyObject* args) {
    PyObject* pFunc = PyObject_GetAttrString(pModule, funcName.c_str());
    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pValue = PyObject_CallObject(pFunc, args);
        if (pValue != nullptr) {
            const char* result = PyUnicode_AsUTF8(pValue);
            std::cout << result << std::endl;
            Py_DECREF(pValue);
        } else {
            PyErr_Print();
            std::cerr << "Llamada a la función " << funcName << " fallida" << std::endl;
        }
        Py_DECREF(pFunc);
    } else {
        if (PyErr_Occurred())
            PyErr_Print();
        std::cerr << "No se pudo encontrar la función " << funcName << std::endl;
    }
}

bool Numero(std::string linea) {
    bool num = true;
    int longitud = linea.size();

    if (longitud == 0) {
        num = false;
    } else if (longitud == 1 && !isdigit(linea[0])) {
        if (linea[0] == '-') { 
            num = false;
        }
    } else {
        int indice = 0;
        if (linea[0] == '+') {
            indice = 1;
        }
        while (indice < longitud) {
            if (!isdigit(linea[indice])) {
                num = false;
                break;
            }
            indice++;
        }
    }
    return num;
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
            num = std::stoi(dato);
        } else {
            correcto = false;
            std::cout << "El valor ingresado no es correcto." << std::endl;
        }
    } while (correcto == false);

    n = num;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    // Inicializar el intérprete de Python
    Py_Initialize();

    // Cargar el script de Python
    PyObject* pName = PyUnicode_DecodeFSDefault("graph_script");
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    Grafo* grafoActual = nullptr;
    int opcion;

    if (pModule != nullptr) {
        while (true) {
            clearScreen();
            mostrarMenu();
            std::cin >> opcion;

            switch (opcion) {
                case 1: {
                    if (grafoActual == nullptr) {
                        int numNodos = 5; 
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

                        // Llamado de la función inicializar_grafo en Python
                        callPythonFunction(pModule, "inicializar_grafo", nullptr);

                        int origen = 0;
                        int destino = 2;

                        std::pair<std::vector<int>, std::vector<int>> resultado = Dijkstra(*grafoActual, origen);
                        std::vector<int> distancias = resultado.first;
                        std::vector<int> previo = resultado.second;
                        std::vector<int> camino = reconstruirCamino(previo, destino);

                        std::cout << "Origen: " << origen << " -> Destino: " << destino << std::endl;
                        for (int nodo : camino) {
                            std::cout << nodo << " ";
                        }
                        std::cout << "\nPeso mínimo del camino encontrado: " << distancias[destino] << std::endl;
                    } else {
                        std::cout << "El grafo ya ha sido inicializado." << std::endl;
                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 2: {
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
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 3: {
                    if (grafoActual == nullptr) {
                        std::cout << "Primero inicialice el grafo." << std::endl;
                    } else {
                        int u, v;
                        std::cout << "Ingrese el nodo origen: ";
                        llenarNumeroInt(u);
                        std::cout << "Ingrese el nodo destino: ";
                        llenarNumeroInt(v);
                        grafoActual->setDisponibilidad(u, v, false);
                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 4: {
                    if (grafoActual == nullptr) {
                        std::cout << "Primero inicialice el mapa." << std::endl;
                    } else{
                        callPythonFunction(pModule, "mostrar_grafo", nullptr);
                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 5: {
                    if (grafoActual == nullptr) {
                        std::cout << "Primero inicialice el grafo." << std::endl;
                    } else {
                        int origenOptimoIndex = -1;
                        int tiempoMenor = INT_MAX;
                        int V = grafoActual->getNumeroVertices();
                        for (int origen = 0; origen < V; ++origen) {
                            int distanciaTotal = 0;
                            std::pair<std::vector<int>, std::vector<int>> resultado = Dijkstra(*grafoActual, origen);
                            std::vector<int> distancias = resultado.first;

                            std::cout << "Distancias desde el pozo/centro de agua " << origen << ":" << std::endl;
                            for (int destino = 0; destino < V; ++destino) {
                                if (distancias[destino] == INT_MAX) {
                                    std::cout << "No hay camino a " << destino << std::endl;
                                } else {
                                    std::cout << "Distancia a pozo/centro de agua " << destino << ": " << distancias[destino] << std::endl;
                                }
                                distanciaTotal += distancias[destino];
                            }

                            if(tiempoMenor > distanciaTotal){
                                tiempoMenor = distanciaTotal;
                                origenOptimoIndex = origen;
                            }

                            std::cout << "Distancia total desde el pozo/centro de agua " << origen << ": " << distanciaTotal << std::endl;
                            std::cout << std::endl;
                        }

                        std::pair<std::vector<int>, std::vector<int>> resultado = Dijkstra(*grafoActual, origenOptimoIndex);
                        std::vector<int> distancias = resultado.first;
                        std::vector<int> previo = resultado.second;
                        std::vector<int> camino = reconstruirCamino(previo, origenOptimoIndex);

                        std::cout << "Origen optimo: " << origenOptimoIndex << std::endl;
                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 6: {
                    delete grafoActual; // Liberar la memoria del grafo al final
                    Py_DECREF(pModule);
                    Py_Finalize();
                    return 0;
                }
                default: {
                    std::cout << "Opcion invalida. Intente de nuevo." << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
            }
        }
    } else {
        PyErr_Print();
        std::cerr << "No se pudo cargar el módulo graph_script" << std::endl;
        std::cin.ignore();
        std::cin.get();
    }

    // Finalizar el intérprete de Python
    Py_Finalize();
    return 0;
}
