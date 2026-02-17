#pragma once
#include <string>
/**
 * Repräsentiert ein Story-Element im Spiel.
 *
 * Jedes Element kann Text enthalten, optional einen Kampf starten
 * und optional ein Item mit ASCII-Darstellung zeigen.
 */
struct StoryElement {
    std::string text;
    bool kampfStart; // True, wenn an diesem Punkt ein Kampf startet
    int gegnerIndex; // Index des Gegners in der Gegnerliste
    std::string itemASCII;
};