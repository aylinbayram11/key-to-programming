#pragma once
#include <iostream>
#include <vector>
#include "StoryElement.h"

// Vorwaertsdeklarationen zur Vermeidung von Include-Zyklen
class Held;
class Gegner;

/**
 * Zentrale Klasse fuer alle Konsolenausgaben.
 *
 * Kuemmert sich um:
 *  - HUD-Anzeige
 *  - Log-Ausgaben (normal & langsam)
 *  - Zentrierte Texte
 *  - Boxen-Ausgaben
 *  - Bildschirmsteuerung (Clear, Cursor)
 */

class Display {

public:
    // HUD & Gegner
    static void printHUD(Held& held, Gegner& gegner, int gegnerNum);
    static void printGegner(int gegnerNum);

    // Log-Ausgaben
    static void printLog(const std::string& text);
    static void printLogSlow(const std::string& text);
    static void resetLog();

    // Zentrierte Ausgaben
    static void printCentered(const std::string& text);
    static void printCenteredQuick(const std::string& text);

    // Box-Ausgaben (center oder log)
    static void printBoxed(const std::string& centerOrLog, const std::string& text);

    // Bildschirm
    static void clearScreen();

private:
    // Aktuelle Zeilenpositionen
    static int logLine;        // aktuelle Log-Zeile
    static int centeredLine;   // aktuelle zentrierte Zeile

    // Hilfsfunktion
    static int getConsoleWidth();
};