#pragma once
#include <string>
using namespace std;

// Vorwaertsdeklarationen, um Include-Zyklen zu vermeiden
class Held;    
class Gegner;  

/*
 * Repraesentiert eine Attacke, die entweder von einem Helden
 * oder einem Gegner ausgefuehrt werden kann.
 */

class Attacke {
public:
    // Daten
    string name; 
    int schaden; // Basis-Schaden (0 = Spezialeffekt)
    string effekt; // Zusatz-Effekt (z.B. Buff, Debuff, Dot)

    // Konstruktoren
    Attacke(string n, int s, string e = ""); 
    Attacke();

    // Methoden
    void ausfuehren(Gegner& angreifer, Held& ziel); // Angriff: Gegner greift Held an
    void ausfuehren(Held& angreifer, Gegner& ziel); // Angriff: Held greift Gegner an
};
