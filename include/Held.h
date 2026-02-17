#pragma once
#include <string>
#include <vector>
#include "Inventar.h"
using namespace std;

// Vorwaertsdeklarationen zur Vermeidung von Include-Zyklen
class Gegner;
class Attacke;  

/**
 * Repraesentiert den Spieler / Helden.
 *
 * Der Held besitzt Lebenspunkte, Attacken, Status-Effekte
 * sowie ein Inventar fuer Items.
 */
class Held {
public:
    // Basisdaten
    string name;
    int hp;
    int maxHp;

    // Status-Effekte
    bool verteidigungAktiv;
    bool boostAktiv;
    int dotSchaden;
    int dotRunden;
    bool extraZug = false;

    // Kampfwerte
    float angriffskraft;
    double permBuff = 1.0;

    // Inhalte
    vector<Attacke> attackenListe;
    Inventar inventory;
    int besiegteGegner;

    // Konstruktoren
    Held(string n);
    Held();

    // Aktionen & Anzeige
    void zeigeAttacken();
    void angreifen(Held& held, Gegner& gegner, Attacke attacke);
    void heilen(int menge);
    void resetStats();
};