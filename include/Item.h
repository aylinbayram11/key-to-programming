#pragma once
#include <string>
using namespace std;

class Held; // Vorwaertsdeklaration zur Vermeidung von Include-Zyklen

/**
 * Repräsentiert ein benutzbares Item im Spiel.
 *
 * Items koennen z. B. heilen, Buffs geben oder
 * spezielle Effekte ausloesen.
 */

class Item {
public:

    //Basisdaten
    string name;
    string typ;   // z. B. "Heilung", "Buff"
    int wert;

    //Konstruktoren
    Item(string n, string t, int w);
    Item();  

    //Aktion
    void anwenden(Held& ziel);
};