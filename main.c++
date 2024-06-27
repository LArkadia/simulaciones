#include "graficos/Pantalla.hpp"
#include <map>
#include <random>
#include <cmath>
#include <utility>
/*
Intrucción de compilación:  g++ main.c++ -lSDL2 -lSDL2_image -lSDL2_ttf && ./a.out
*/
//Parametros de simulacion
const float velocidad_sonido    = 300;    // m/s
const float Tasa_muestreo       = 100;    // muestras/seg
const float unidades_metros     = 0.001;   //cm
const float intensidad_emision  = 5;    //db
const float max_radio           = 1000;
//Prototipo funciones
void eventos(SDL_Event& evento_actual);
std::vector<int> generarSennalAudio();
typedef std::pair<double, double> Point;
std::vector<Point> generate_hyperbola_points(Point F1, Point F2, double difference, int num_points ); 
void plot_hyperbola(int mic1, int mic2, float delta_d, int num_points,Color& colorr); 

Pantalla Ventana;
std::map<std::string,Color*> colores;
    std::vector<std::string> codcolor;
std::vector<std::vector<int>> microfono{
    {600,400},
    {500,500},
    {700,500}
};

int main(){
    

    
    Ventana.crear_ventana("Simulacion de microfonos",1200,800);
    Ventana.crear_renderizador(SDL_RENDERER_ACCELERATED);

    colores["Verde"] =  new Color(0,155,0);
    colores["Azul"] =  new Color(30,80,155);
    colores["Rojo"] =  new Color(155,0,0);
    colores["Ambar"] =  new Color(255,191,0);
    colores["Rosa"] =  new Color(155,100,100);

    codcolor.push_back("Verde");
    codcolor.push_back("Azul");
    codcolor.push_back("Rojo");
    codcolor.push_back("Ambar");

    while (Ventana.manejar_eventos(eventos))
    {
        
        Ventana.dibujar_circunferencia(microfono[0][0],microfono[0][1],10,*colores["Verde"]);
        Ventana.dibujar_circunferencia(microfono[1][0],microfono[1][1],10,*colores["Azul"]);
        Ventana.dibujar_circunferencia(microfono[2][0],microfono[2][1],10,*colores["Ambar"]);

        Ventana.presentar_renderizador();
        //SDL_Delay(800);
        //Ventana.limpiar();
    }
}


void eventos(SDL_Event& evento_actual){
        std::vector<std::pair<int, int>> pares_microfonos;
        int x;
        int y;
        float diferencia;
        float color_count = 0;
    switch (evento_actual.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        switch (evento_actual.button.button)
        {
        case SDL_BUTTON_LEFT:
        x = evento_actual.button.x;
        y = evento_actual.button.y;

        for (int i = 0; i < microfono.size(); i++)
        {
            for (int u = 0; u < microfono.size(); u++)
            {
                if (u >= i ){
                        color_count ++;
                        float dx1 = x - microfono[i][0];
                        float dy1 = y - microfono[i][1];
                        float distancia1 = sqrt(dx1 * dx1 + dy1 * dy1);

                        float dx2 = x - microfono[u][0];
                        float dy2 = y - microfono[u][1];
                        float distancia2 = sqrt(dx2 * dx2 + dy2 * dy2);
                    diferencia = distancia2-distancia1;
                    plot_hyperbola(i,u,diferencia,300,*colores["Rosa"]);                   
                }
            }
            
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
void plot_hyperbola(int mic1, int mic2, float delta_d, int num_points,Color& colorr) {
    Point foco1 = {microfono[mic1][0],microfono[mic1][1]} ;
    Point foco2 = {microfono[mic2][0],microfono[mic2][1]} ;
    //Ventana.dibujar_circunferencia(x1,y1,20,*colores["Verde"]);
    //Ventana.dibujar_circunferencia(x2,y2,15,*colores["Azul"]);
    std::vector<Point> Hyperbola;
    
        Hyperbola = generate_hyperbola_points(foco2,foco1,delta_d,100);
    
    
    
    //

    for (int i = 0; i < Hyperbola.size(); i++)
    {
        Ventana.dibujar_circunferencia(Hyperbola[i].first,Hyperbola[i].second,1,colorr);
    }
    
    

}

std::vector<Point> generate_hyperbola_points(Point F1, Point F2, double difference, int num_points = 100) {
    std::vector<Point> points;
    
    // Calcular el centro
    Point center((F1.first + F2.first) / 2, (F1.second + F2.second) / 2);
    
    // Calcular la orientación de la hipérbola
    double angle = atan2(F2.second - F1.second, F2.first - F1.first);
    
    // Semi-ejes de la hipérbola
    double a = difference / 2; // Semi-eje mayor (distancia al vértice)
    double c = sqrt(pow(F1.first - center.first, 2) + pow(F1.second - center.second, 2));
    double b = sqrt(c * c - a * a); // Semi-eje menor calculado por la relación c^2 = a^2 + b^2
    
    // Generar puntos sobre la hipérbola
    for (int i = -num_points; i < num_points; ++i) {
        double t = i * 0.1;
        double x = a * cosh(t);
        double y = b * sinh(t);
        
        // Rotar los puntos
        double rotatedX = center.first + x * cos(angle) - y * sin(angle);
        double rotatedY = center.second + x * sin(angle) + y * cos(angle);
        
        points.push_back(Point(rotatedX, rotatedY));
    }
    
    return points;
}