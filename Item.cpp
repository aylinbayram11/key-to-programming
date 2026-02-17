#include "Item.h"
#include "Held.h"
#include <iostream>
#include "Display.h"
using namespace std;

// --------------------------------------------
// Konstruktoren
// --------------------------------------------
Item::Item(string n, string t, int w)
	: name(n), typ(t), wert(w) {
}

Item::Item()
	: name(""), typ(""), wert(0) {
}

// --------------------------------------------
// Wendet das Item auf den Helden an
// Logik bleibt unverändert
// --------------------------------------------
void Item::anwenden(Held& ziel) {

	// Heilung: HP erhöhen, Debuffs entfernen
	if (typ == "Heilung") {
		ziel.hp += wert;
		if (ziel.hp > ziel.maxHp) ziel.hp = ziel.maxHp;

		Display::printLogSlow(ziel.name + " heilt sich um " + to_string(wert)
			+ " HP. Jetzt: " + to_string(ziel.hp) + "/" + to_string(ziel.maxHp) + " HP.\n Alle Debuffs verschwinden.");

		ziel.dotRunden = 0;// Debuff-Runden zurücksetzen
		ziel.angriffskraft = 1;   // Angriffskraft normalisieren
	}

	// Extra-Zug: Spieler darf sofort nochmal handeln
	else if (typ == "ExtraTurn") {
		ziel.extraZug = true;
		Display::printLogSlow("Du erhaelst einen extra Zug!\n");
	}
	// Permanenter Buff: dauerhafter Schadensbonus
	else if (typ == "PermBuff") {
		ziel.permBuff = wert / 100.0 + 1.0; // z.B. wert=20 → +20%
		Display::printLogSlow(" : dauerhaft +" + std::to_string(wert) + "% Schaden!\n");
	}
	// Debuff: Gegnerischer Angriff wird geschwächt
	else if (typ == "Debuff") {

		ziel.verteidigungAktiv = true;
		Display::printLogSlow("\033[38;5;202m" + ziel.name + " fuehlt sich gut vorbereitet – "
			+ "gegnerischer Angriff ist um 75% geschwaecht!\n \033[0m");
	}
}