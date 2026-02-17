#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Vorwaertsdeklarationen zur Vermeidung von Include-Zyklen
class Item;
class Held;

/**
 * Verwaltet die Items des Spielers.
 *
 * Das Inventar speichert Items, zeigt sie an
 * und erlaubt deren Nutzung im Spiel.
 */

class Inventar {
public:
    vector<Item> items; // Gespeicherte Items

    // Aktionen & Anzeige
    void fuegeItemHinzu(const Item& item);
    bool zeigeInventar(Held& held);
    void nutzeItem(int index, Held& held);
};