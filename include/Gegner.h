#pragma once
#include <string>
#include <vector>
#include "Item.h"
using namespace std;

// Vorwaertsdeklarationen zur Vermeidung von Include-Zyklen
class Attacke;  
class Item;     
class Held;     

/**
 * Repraesentiert einen Gegner im Spiel.
 *
 * Ein Gegner besitzt:
 *  - Lebenspunkte
 *  - eine Liste von Attacken
 *  - ein Item, das er beim Besiegen fallen laesst
 */

class Gegner {
public:
    //Grunddaten
    string name;
    int hp;
    int maxHp;
    int id;

    //Kampfattribute
    float angriffskraft;

    //Inhalte
    vector<Attacke> attacken;
    Item drop;
    
    //Kontruktorn
    Gegner(string n, int GegnerId, int mHp, vector<Attacke> atk, Item d);
    Gegner();  // Standardkonstruktor, falls noetig

    //Anzeige & Aktionen
    void zeigeStatus();
    void zufallsAngriff(Gegner& gegner, Held& held);
    void zeigeAttacken();
};