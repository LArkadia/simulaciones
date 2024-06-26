#include "graficos/Pantalla.hpp"
#include <map>
#include <random>
/*
Intrucción de compilación:  g++ main.c++ -lSDL2 -lSDL2_image -lSDL2_ttf && ./a.out
*/
//Parametros de simulacion
const float Velocidad_sonido    = 300;    // m/s
const float Tasa_muestreo       = 100;    // muestras/seg
const float unidades_metros     = 0.001;   //cm
const float intensidad_emision  = 5;    //db
const float max_radio           = 1000;
//Prototipo funciones
void eventos(SDL_Event& evento_actual);
std::vector<int> generarSennalAudio();

Pantalla Ventana;
std::map<std::string,Color*> colores;
std::vector<std::vector<int>> microfono{
    {600,300},
    {400,500},
    {800,500}
};

int main(){
    

    
    Ventana.crear_ventana("Simulacion de microfonos",1200,800);
    Ventana.crear_renderizador(SDL_RENDERER_ACCELERATED);

    colores["Verde"] =  new Color(0,155,0);
    colores["Azul"] =  new Color(30,80,155);
    colores["Rojo"] =  new Color(155,0,0);
    colores["Ambar"] =  new Color(255,191,0);

    while (Ventana.manejar_eventos(eventos))
    {
        
        Ventana.dibujar_circunferencia(microfono[0][0],microfono[0][1],10,*colores["Verde"]);
        Ventana.dibujar_circunferencia(microfono[1][0],microfono[1][1],10,*colores["Azul"]);
        Ventana.dibujar_circunferencia(microfono[2][0],microfono[2][1],10,*colores["Ambar"]);

        Ventana.presentar_renderizador();
        SDL_Delay(800);
        Ventana.limpiar();
    }
}

            int x;
            int y;
void eventos(SDL_Event& evento_actual){
    switch (evento_actual.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        switch (evento_actual.button.button)
        {
        case SDL_BUTTON_LEFT:
            x = evento_actual.button.x;
            y = evento_actual.button.y;


            for (auto& mic : microfono) {
                float dx = x - mic[0];
                float dy = y - mic[1];
                float distancia = sqrt(dx * dx + dy * dy);  // Calcula la distancia euclidiana

                // Intensidad de la señal basada en la ley del inverso del cuadrado
                float intensidad = intensidad_emision / (distancia * distancia);  
                if (intensidad < 1e-7) { // Ajustar umbral según sea necesario para evitar divisiones por cero
                    intensidad = 1e-7;
                }
                
                // El radio del círculo es inversamente proporcional a la intensidad
                float radio = sqrt(intensidad_emision) / sqrt(intensidad);
                if (radio > max_radio) {
                    radio = max_radio;  // Limitar el tamaño máximo del círculo
                }

                // Dibuja un círculo en la posición del micrófono con el radio calculado
                Ventana.dibujar_circunferencia(mic[0], mic[1], radio, *colores["Rojo"]);
            }
            
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }
}
std::vector<int> generarSennalAudio() {
    std::vector<int> senal(100);  // Vector para almacenar los 100 valores
    std::random_device rd;  // Dispositivo para generar números aleatorios
    std::mt19937 gen(rd());  // Generador de números (Mersenne Twister)
    std::uniform_int_distribution<> distrib(-32768, 32767);  // Distribución para simular la amplitud de audio en 16 bits

    // Llenar el vector con números aleatorios
    for (int &valor : senal) {
        valor = distrib(gen);
    }

    return senal;
}