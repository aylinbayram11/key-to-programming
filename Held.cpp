#include "Held.h"
#include "Item.h"
#include "Gegner.h"
#include "Attacke.h"
#include "Inventar.h"
#include "Display.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// --------------------------------------------
// Konstruktor
// --------------------------------------------
Held::Held(string n)
{
	name = n;
	maxHp = 100;
	hp = maxHp;

	angriffskraft = 1;
	verteidigungAktiv = false;
	boostAktiv = false;
	dotRunden = 0;
	dotSchaden = 0;
	permBuff = 1.0;
	extraZug = false;

	inventory = {};       // Leeres Inventar
	besiegteGegner = 0;   // Keine Gegner besiegt am Anfang

	// Standard-Attacken des Helden
	Attacke selbstdisziplin("Selbstdisziplin", 20, "");
	Attacke kreativitaet("Kreativitaet", 30, "");
	Attacke motivation("Motivation", 0, "Shield");
	Attacke teamarbeit("Teamarbeit", 0, "Buff");
	attackenListe.push_back(selbstdisziplin);
	attackenListe.push_back(kreativitaet);
	attackenListe.push_back(motivation);
	attackenListe.push_back(teamarbeit);
}

// --------------------------------------------
// Zeigt alle Attacken des Helden an
// --------------------------------------------
void Held::zeigeAttacken()
{
	if (attackenListe.empty()) {
		cout << "Error\n";
		return;
	}
	Display::printLog("Attacken:");

	for (int i = 0; i < attackenListe.size(); i++) {

		if (attackenListe[i].effekt == "")
		{
			Display::printLog(to_string(i + 1) + ": " + attackenListe[i].name + " (" + to_string(attackenListe[i].schaden) + " dmg)");
		}
		else
		{
			Display::printLog(to_string(i + 1) + ": " + attackenListe[i].name + " (" + attackenListe[i].effekt + ")");
		}

	}
	Display::printLog("5: Items\n");
}

// --------------------------------------------
// Führt eine Attacke gegen einen Gegner aus
// --------------------------------------------
void Held::angreifen(Held& held, Gegner& gegner, Attacke attacke)
{
	attacke.ausfuehren(held, gegner);
}

// --------------------------------------------
// Heilt den Helden um eine bestimmte Menge
// --------------------------------------------
void Held::heilen(int menge)
{
	hp += menge;
	if (hp > maxHp)
	{
		hp = maxHp; // HP nicht über Maximum hinaus erhöhen
	}
	cout << name << " heilt sich um " << menge << " Punkte. "
		<< "Aktuelle HP: " << hp << " / " << maxHp << "\n";
}
