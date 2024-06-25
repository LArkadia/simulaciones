#include "graficos/Pantalla.hpp"
#include <map>

int main(){
    Pantalla Ventana;
    std::map<std::string,Color*> colores;
    Ventana.crear_ventana("Simulacion de microfonos",600,400);
    Ventana.crear_renderizador(SDL_RENDERER_ACCELERATED);

    colores["Verde"] =  new Color(0,155,0);
    colores["Azul"] =  new Color(30,80,155);
    colores["Rojo"] =  new Color(155,0,0);
    colores["Ambar"] =  new Color(255,191,0);

    while (Ventana.manejar_eventos())
    {
        Ventana.dibujar_circunferencia(300,200,10,*colores["Verde"]);
        Ventana.dibujar_circunferencia(100,300,10,*colores["Azul"]);
        Ventana.dibujar_circunferencia(500,300,10,*colores["Ambar"]);

        Ventana.presentar_renderizador();
    }
    

}