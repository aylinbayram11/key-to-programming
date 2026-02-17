#pragma once
#include <vector>
#include <string>
#include "Held.h"
#include "StoryElement.h"
using namespace std;
// Vorwaertsdeklarationen zur Vermeidung von Include-Zyklen
class Held; 
class Gegner;  

/**
 * Zentrale Spielklasse.
 *
 * Verwaltert Spiellogik, Story-Fortschritt,
 * Kaempfe, Game-Over und das Spielende.
 */

class Spiel {
    Held spieler; // Der Spieler / Held
    vector<Gegner> gegnerListe; // Alle Gegner im Spiel
    vector<StoryElement> story; // Story-Abfolge
   

public:
    

    Spiel(string spielerName); // Konstruktor

    // Aktionen
    void start();
    void kampf(Held& spieler, int gegner);
    void spieleStory();
    void gameOverScreen();
    void schluss();
};