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

/**
 * Clase Rutas
 * 
 * Esta clase se utiliza para modelar y gestionar la red de tuberías de distribución de agua entre diferentes centros de agua.
 * Permite la creación de una red de tuberías, la consulta de conexiones entre centros, y la modificación de la disponibilidad de las tuberías.
 */
class Rutas {
    private:
        int centroAgua; // Número total de centros de agua en la red.
        std::vector<std::vector<int>> adyacentes; // Lista de adyacencia para representar las conexiones entre centros.
        std::map<std::pair<int, int>, std::pair<int, bool>> minutosParaLlegar; // Mapa que almacena el tiempo de viaje y la disponibilidad entre dos centros.

    public:
        /**
         * Constructor de la clase Rutas.
         * 
         * @param centroAgua Número total de centros de agua en la red.
         */
        Rutas(int centroAgua) {
            this->centroAgua = centroAgua;
            adyacentes = std::vector<std::vector<int>>(centroAgua);
        }

        /**
         * Obtiene el número total de centros de agua en la red.
         * 
         * @return Número de centros de agua.
         */
        int getNumeroCentrosAgua() {
            return centroAgua;
        }

        /**
         * Obtiene la lista de centros de agua adyacentes a un centro dado.
         * 
         * @param numeroCentroAgua El índice del centro de agua.
         * @return Vector de índices de centros adyacentes.
         */
        std::vector<int> getAdyacentes(int numeroCentroAgua) {
            return adyacentes[numeroCentroAgua];
        }

        /**
         * Obtiene el tiempo de viaje y la disponibilidad de la tubería entre dos centros de agua.
         * 
         * @param u Índice del centro de agua de origen.
         * @param v Índice del centro de agua de destino.
         * @return Par conteniendo el tiempo de viaje y la disponibilidad de la tubería.
         */
        std::pair<int, bool> getMinutosParaLlegarYDisponibilidad(int u, int v) {
            return minutosParaLlegar[{u, v}];
        }

        /**
         * Agrega o actualiza una tubería entre dos centros de agua.
         * 
         * @param u Índice del centro de agua de origen.
         * @param v Índice del centro de agua de destino.
         * @param peso Tiempo de viaje a través de la tubería.
         * @param disponibilidad Estado de disponibilidad de la tubería.
         */
        void agregarTuberia(int u, int v, int tiempoParaLlegar, bool disponibilidad = true) {
            adyacentes[u].push_back(v);
            minutosParaLlegar[{u, v}] = {tiempoParaLlegar, disponibilidad}; 
        }

        /**
         * Establece la disponibilidad de una tubería existente entre dos centros de agua.
         * 
         * @param u Índice del centro de agua de origen.
         * @param v Índice del centro de agua de destino.
         * @param disponibilidad Nuevo estado de disponibilidad para la tubería.
         */
        void establecerDisponibilidad(int u, int v, bool disponibilidad) {
            if (minutosParaLlegar.find({u, v}) != minutosParaLlegar.end()) {
                minutosParaLlegar[{u, v}].second = disponibilidad;
                std::cout << "Tubería " << u << " a " << v << " modificada." << std::endl;
            } else {
                std::cout << "La tuberia de " << u << " a " << v << " no existe." << std::endl;
            }
        }

};

/**
 * Clase ColaPrioridad
 *
 * Esta clase implementa una cola de prioridad utilizando un multiset de la STL de C++. 
 * La cola de prioridad es esencial para el algoritmo de Dijkstra, ya que permite extraer 
 * el elemento con la menor prioridad (o costo, en este caso en el menor tiempo de llegada) de manera eficiente.
 */
class ColaPrioridad {
    private:
        // Multiset para almacenar los elementos junto con sus prioridades
        std::multiset<std::pair<int, int>> elementos;

    public:
        // Constructor de la clase ColaPrioridad
        ColaPrioridad() {}

        /**
         * Inserta un elemento con una prioridad dada en la cola de prioridad.
         * 
         * @param elemento El elemento a insertar.
         * @param prioridad La prioridad del elemento.
         */
        void insertar(int elemento, int prioridad) {
            elementos.insert({prioridad, elemento});
        }

        /**
         * Extrae el elemento con la menor prioridad de la cola de prioridad.
         * 
         * @return El elemento con la menor prioridad.
         */
        int extraerMinimo() {
            int minimo = elementos.begin()->second; // Acceder al segundo elemento del par del primer elemento
            elementos.erase(elementos.begin()); // Eliminar el primer elemento
            return minimo;
        }

        /**
         * Inserta un nuevo elemento o decrementa la prioridad de un elemento existente.
         * 
         * @param elemento El elemento a insertar o cuyo prioridad se debe decrementar.
         * @param nuevaPrioridad La nueva prioridad del elemento.
         */
        void insertarODecrementar(int elemento, int nuevaPrioridad) {
            for (auto it = elementos.begin(); it != elementos.end(); ++it) {
                if (it->second == elemento) {
                    elementos.erase(it);
                    break;
                }
            }
            elementos.insert({nuevaPrioridad, elemento});
        }

        /**
         * Encuentra el índice de un elemento en la cola de prioridad.
         * 
         * @param elemento El elemento a encontrar.
         * @return El índice del elemento, o -1 si no se encuentra.
         */
        int encontrarElemento(int elemento) {
            for (auto it = elementos.begin(); it != elementos.end(); ++it) {
                if (it->second == elemento) {
                    return std::distance(elementos.begin(), it); // Índice
                }
            }
            return -1;
        }

        /**
         * Obtiene los elementos de la cola de prioridad.
         * 
         * @return El multiset de elementos y sus prioridades.
         */
        const std::multiset<std::pair<int, int>>& getElementos() {
            return elementos;
        }

        /**
         * Verifica si la cola de prioridad está vacía.
         * 
         * @return Verdadero si la cola está vacía, falso en caso contrario.
         */
        bool isEmpty() {
            return elementos.empty();
        }
};


/**
 * Implementación del algoritmo de Dijkstra para encontrar las rutas más cortas desde un centro de agua de origen
 * a todos los otros centros de agua en la red.
 * 
 * @param G Grafo que representa la red de tuberías de agua (instancia de la clase Rutas).
 * @param centroAguaOrigen Índice del centro de agua de origen desde el cual se calcularán las rutas más cortas.
 * @return Un par de vectores:
 *         - El primer vector contiene el tiempo acumulado mínimo para llegar a cada centro de agua desde el centro de agua de origen.
 *         - El segundo vector contiene el índice del centro de agua anterior para cada centro de agua en la ruta más corta.
 */
std::pair<std::vector<int>, std::vector<int>> Dijkstra(Rutas G, int centroAguaOrigen) {
    int numeroCentrosAgua = G.getNumeroCentrosAgua(); // Obtener el número total de centros de agua
    std::vector<int> minutosParaLlegarAcumulados(numeroCentrosAgua, INT_MAX); // Vector para almacenar el tiempo acumulado mínimo para llegar a cada centro de agua
    std::vector<int> centrosAguaPrevio(numeroCentrosAgua, -1); // Vector para almacenar el centro de agua previo en la ruta más corta
    std::vector<bool> centroAguaVisitado(numeroCentrosAgua, false); // Vector para marcar los centros de agua visitados
    ColaPrioridad colaPrioridad; // Cola de prioridad para gestionar los centros de agua según su tiempo acumulado mínimo

    minutosParaLlegarAcumulados[centroAguaOrigen] = 0; // Establecer el tiempo acumulado del centro de agua de origen a 0
    colaPrioridad.insertar(centroAguaOrigen, 0); // Insertar el centro de agua de origen en la cola de prioridad con un tiempo de llegada de 0

    // Mientras la cola de prioridad no esté vacía
    while (!colaPrioridad.isEmpty()) {
        int centroAguaActual = colaPrioridad.extraerMinimo(); // Extraer el centro de agua con el menor tiempo acumulado
        centroAguaVisitado[centroAguaActual] = true; // Marcar el centro de agua actual como visitado

        // Para cada centro de agua vecino del centro de agua actual
        for (int centroAguaVecino : G.getAdyacentes(centroAguaActual)) {
            auto [minutosParaLlegar, disponibilidad] = G.getMinutosParaLlegarYDisponibilidad(centroAguaActual, centroAguaVecino); // Obtener el tiempo de viaje y la disponibilidad de la tubería entre los centros de agua
            if (disponibilidad && !centroAguaVisitado[centroAguaVecino]) { // Si la tubería está disponible y el centro de agua vecino no ha sido visitado
                int nuevosMinutosParaLlegar = minutosParaLlegarAcumulados[centroAguaActual] + minutosParaLlegar; // Calcular el nuevo tiempo acumulado para llegar al centro de agua vecino

                // Si el nuevo tiempo acumulado es menor que el tiempo acumulado actual
                if (nuevosMinutosParaLlegar < minutosParaLlegarAcumulados[centroAguaVecino]) {
                    minutosParaLlegarAcumulados[centroAguaVecino] = nuevosMinutosParaLlegar; // Actualizar el tiempo acumulado mínimo
                    centrosAguaPrevio[centroAguaVecino] = centroAguaActual; // Actualizar el centro de agua previo en la ruta más corta
                    colaPrioridad.insertarODecrementar(centroAguaVecino, nuevosMinutosParaLlegar); // Insertar o actualizar la prioridad del centro de agua vecino en la cola de prioridad
                }
            }
        }
    }

    // Retornar el vector de tiempos acumulados y el vector de centros de agua previos
    return {minutosParaLlegarAcumulados, centrosAguaPrevio};
}

/**
 * Reconstruye la ruta más corta desde el centro de agua de origen hasta el centro de agua destino
 * utilizando el vector de centros de agua previos calculado por el algoritmo de Dijkstra.
 * 
 * @param centrosAguaPrevio Vector que contiene el índice del centro de agua anterior para cada centro de agua en la ruta más corta.
 * @param centroAguaDestino Índice del centro de agua destino al que se quiere llegar.
 * @return Un vector que contiene la secuencia de centros de agua desde el origen hasta el destino en la ruta más corta.
 */
std::vector<int> reconstruirRuta(const std::vector<int>& centrosAguaPrevio, int centroAguaDestino) {
    std::vector<int> rutaDestino; // Vector para almacenar la ruta desde el origen hasta el destino

    // Recorre el vector de centros de agua previos desde el destino hasta el origen
    for (int centroAguaActual = centroAguaDestino; centroAguaActual != -1; centroAguaActual = centrosAguaPrevio[centroAguaActual]) {
        rutaDestino.push_back(centroAguaActual); // Agrega el centro de agua actual a la ruta
    }

    // Invierte el vector para obtener la ruta desde el origen hasta el destino
    std::reverse(rutaDestino.begin(), rutaDestino.end());

    return rutaDestino; // Retorna la ruta reconstruida
}

void mostrarMenu() {
    std::cout << "Menu:" << std::endl;
    std::cout << "1. Inicializar rutas." << std::endl; // Inicializa las rutas con los centros de agua y tuberías iniciales
    std::cout << "2. Agregar tuberia." << std::endl; // Agrega una nueva tubería entre dos centros de agua
    std::cout << "3. Deshabilitar tuberia." << std::endl; // Deshabilita una tubería existente entre dos centros de agua
    std::cout << "4. Habilitar tubería." << std::endl; // Habilita una tubería existente entre dos centros de agua
    std::cout << "5. Ver rutas actuales y descargarlo." << std::endl; // Muestra las rutas actuales modificadas y las descarga a un archivo
    std::cout << "6. Calcular rutas optimas de cada centro de agua." << std::endl; // Calcula las rutas óptimas desde cada centro de agua a los demás
    std::cout << "7. Salir" << std::endl; // Sale del programa
    std::cout << "Ingrese una opcion: ";
}


/*Esta función nos permite interactuar con funciones dentro de un archivo .py
  para ser llamados dentro de este entorno,
  Funciona como un API de C++/Python*/
  
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

/**
 * Verifica si una cadena de caracteres representa un número entero válido.
 * 
 * @param linea La cadena de caracteres a verificar.
 * @return true si la cadena representa un número entero válido, false en caso contrario.
 */
bool Numero(std::string linea) {
    bool num = true; // Variable para indicar si la cadena es un número
    int longitud = linea.size(); // Longitud de la cadena

    // Verificar si la cadena está vacía
    if (longitud == 0) {
        num = false;
    } 
    // Verificar si la cadena tiene un solo carácter no numérico
    else if (longitud == 1 && !isdigit(linea[0])) {
        if (linea[0] == '-') { 
            num = false;
        }
    } 
    // Verificar si la cadena tiene múltiples caracteres
    else {
        int indice = 0;
        // Saltar el signo positivo
        if (linea[0] == '+') {
            indice = 1;
        }
        // Verificar que cada carácter sea un dígito
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

/**
 * Solicita al usuario que ingrese un número entero válido.
 * 
 * Esta función sigue solicitando al usuario hasta que se ingrese un número entero válido.
 * El número ingresado se almacena en la variable de referencia proporcionada.
 * 
 * @param n Referencia a una variable entera donde se almacenará el número ingresado.
 */
void llenarNumeroInt(int& n) {
    std::string dato; // Variable para almacenar la entrada del usuario
    int num; // Variable para almacenar el número convertido
    bool correcto = false; // Indicador de entrada válida

    do {
        std::cout << "---> "; // Mostrar el prompt de entrada
        std::cin >> dato; // Leer la entrada del usuario

        // Verificar si la entrada es un número válido
        if (Numero(dato)) {
            correcto = true; // Marcar la entrada como válida
            num = std::stoi(dato); // Convertir la entrada a entero
        } else {
            correcto = false; // Marcar la entrada como no válida
            std::cout << "El valor ingresado no es correcto." << std::endl; // Mostrar mensaje de error
        }
    } while (correcto == false); // Repetir hasta obtener una entrada válida

    n = num; // Asignar el número convertido a la variable de referencia
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

    Rutas* rutasActuales = nullptr;
    int opcion;

    if (pModule != nullptr) {
        while (true) {
            clearScreen();
            mostrarMenu();
            std::cin >> opcion;

            switch (opcion) {
                case 1: {
                    if (rutasActuales == nullptr) {
                        int numeroCentrosAgua = 5; 
                        rutasActuales = new Rutas(numeroCentrosAgua);
                        rutasActuales->agregarTuberia(0, 1, 10);
                        rutasActuales->agregarTuberia(0, 3, 5);
                        rutasActuales->agregarTuberia(1, 2, 1);
                        rutasActuales->agregarTuberia(1, 3, 2);
                        rutasActuales->agregarTuberia(2, 4, 4);
                        rutasActuales->agregarTuberia(3, 1, 3);
                        rutasActuales->agregarTuberia(3, 2, 9);
                        rutasActuales->agregarTuberia(3, 4, 2);
                        rutasActuales->agregarTuberia(4, 0, 7);
                        rutasActuales->agregarTuberia(4, 2, 6);
                        std::cout << "Rutas inicializadas con " << numeroCentrosAgua << " centros de agua." << std::endl;

                        // Llamado de la función inicializar_grafo en Python
                        callPythonFunction(pModule, "inicializar_rutas", nullptr);
                    } else {
                        std::cout << "Las rutas ya han sido inicializadas." << std::endl;
                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 2: {
                    if (rutasActuales == nullptr) {
                        std::cout << "Primero inicialice las rutas." << std::endl;
                    } else {
                        int u, v, peso;
                        std::cout << "Ingrese el centro de agua origen: ";
                        llenarNumeroInt(u);
                        std::cout << "Ingrese el centro de agua destino: ";
                        llenarNumeroInt(v);
                        std::cout << "Ingrese los minutos que tarda en llegar de un punto a otro: ";
                        llenarNumeroInt(peso);
                        rutasActuales->agregarTuberia(u, v, peso);
                        callPythonFunction(pModule, "agregar_tuberia", PyTuple_Pack(3, PyLong_FromLong(u), PyLong_FromLong(v), PyLong_FromLong(peso)));
                        std::cout << "TUbería agregada de " << u << " a " << v << " con minutos entre los centros de " << peso << "." << std::endl;
                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 3: {
                    if (rutasActuales == nullptr) {
                        std::cout << "Primero inicialice las rutas." << std::endl;
                    } else {
                        int u, v;
                        std::cout << "Ingrese el centro de agua origen: ";
                        llenarNumeroInt(u);
                        std::cout << "Ingrese el centro de agua destino: ";
                        llenarNumeroInt(v);
                        rutasActuales->establecerDisponibilidad(u, v, false);
                        callPythonFunction(pModule, "establecer_no_disponibilidad", PyTuple_Pack(2, PyLong_FromLong(u), PyLong_FromLong(v)));
                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 4:{
                    if (rutasActuales == nullptr) {
                        std::cout << "Primero inicialice las rutas." << std::endl;
                    } else {
                        int u, v;
                        std::cout << "Ingrese el centro de agua origen: ";
                        llenarNumeroInt(u);
                        std::cout << "Ingrese el centro de agua destino: ";
                        llenarNumeroInt(v);
                        rutasActuales->establecerDisponibilidad(u, v, true);
                        callPythonFunction(pModule, "establecer_disponibilidad", PyTuple_Pack(2, PyLong_FromLong(u), PyLong_FromLong(v)));
                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 5: {
                    if (rutasActuales == nullptr) {
                        std::cout << "Primero inicialice las rutas." << std::endl;
                    } else{
                        callPythonFunction(pModule, "mostrar_rutas", nullptr);
                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 6: {
                    if (rutasActuales == nullptr) {
                        std::cout << "Primero inicialice las rutas." << std::endl;
                    } else {
                        /*Esta opción nos permite hacer el benchmarking de acuerdo a los centros de agua actuales
                          y su tiempo óptimo en llegar a los demas centros de agua, en minutos. Esto nos permite
                          comparar cual centro de agua es factible comenzar a abastecer agua, para que llegue a los
                          demás centros y no dejar de abastecer aguar a las zonas aledañas. */
                        int origenOptimoIndex = -1;
                        int tiempoMenor = INT_MAX;
                        int V = rutasActuales->getNumeroCentrosAgua();
                        for (int centrodeAguaOrigen = 0; centrodeAguaOrigen < V; ++centrodeAguaOrigen) {
                            int minutosAcumuladosTotales = 0;
                            std::pair<std::vector<int>, std::vector<int>> resultado = Dijkstra(*rutasActuales, centrodeAguaOrigen);
                            std::vector<int> distancias = resultado.first;

                            std::cout << "Distancias desde el pozo/centro de agua " << centrodeAguaOrigen << ":" << std::endl;
                            for (int destino = 0; destino < V; ++destino) {
                                if (distancias[destino] == INT_MAX) {
                                    std::cout << "No hay camino a " << destino << std::endl;
                                } else {
                                    std::cout << "Distancia a pozo/centro de agua " << destino << ": " << distancias[destino] << std::endl;
                                }
                                minutosAcumuladosTotales += distancias[destino];
                            }

                            if(tiempoMenor > minutosAcumuladosTotales){
                                tiempoMenor = minutosAcumuladosTotales;
                                origenOptimoIndex = centrodeAguaOrigen;
                            }

                            std::cout << "Distancia total desde el pozo/centro de agua " << centrodeAguaOrigen << ": " << minutosAcumuladosTotales << std::endl;
                            std::cout << std::endl;
                        }

                        std::pair<std::vector<int>, std::vector<int>> resultado = Dijkstra(*rutasActuales, origenOptimoIndex);
                        std::vector<int> distancias = resultado.first;
                        std::vector<int> previo = resultado.second;

                        std::cout << "Origen optimo: " << origenOptimoIndex << std::endl;

                    }
                    std::cout << "Presione una tecla para continuar" << std::endl;
                    std::cin.ignore();
                    std::cin.get();
                    break;
                }
                case 7: {
                    delete rutasActuales; // Liberar la memoria del grafo al final
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
