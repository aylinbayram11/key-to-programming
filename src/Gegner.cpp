#include "Gegner.h"
#include "Held.h"
#include "Item.h"
#include "Attacke.h"
#include "Display.h"
#include "Inventar.h"
#include <iostream>
#include <string>
using namespace std;

// --------------------------------------------
// Konstruktor
// --------------------------------------------
Gegner::Gegner(string n, int GegnerId, int mHp, vector<Attacke> atk, Item d)
{
	name = n;
	maxHp = mHp;
	hp = mHp;
	attacken = atk;
	drop = d;
	angriffskraft = 1;
	id = GegnerId;
}

// --------------------------------------------
// Alle Attacken des Gegners anzeigen
// --------------------------------------------
void Gegner::zeigeAttacken()
{
	if (attacken.empty()) {
		cout << "Error\n";
		return;
	}
	Display::printLogSlow("" + name + " waehlt aus seinen Attacken: \n");
	for (int i = 0; i < attacken.size(); i++) {
		if (attacken[i].effekt == "")
		{
			Display::printLog(to_string(i + 1) + ": " + attacken[i].name + " (" + to_string(attacken[i].schaden) + " dmg)\n");
		}
		else
		{
			Display::printLog(to_string(i + 1) + ": " + attacken[i].name + " (" + attacken[i].effekt + ")\n");
		}
	}
}

// --------------------------------------------
// Gegnerstatus anzeigen (HP-Leiste)
// --------------------------------------------
void Gegner::zeigeStatus() {
	cout << "---------------------------------\n";
	cout << "Gegner: " << name << "\n";

	// Laenge der Lebensleiste
	int barWidth = 20;
	int filled = (hp * barWidth) / maxHp;

	cout << "[";
	for (int i = 0; i < barWidth; i++) {
		if (i < filled) cout << "#";   // gefuellter Teil
		else cout << " ";              // leerer Teil
	}
	cout << "] " << "HP: " << hp << " / " << maxHp << "\n";
	cout << "---------------------------------\n";
}

// --------------------------------------------
// Zufälliger Angriff des Gegners
// --------------------------------------------
void Gegner::zufallsAngriff(Gegner& gegner, Held& held)
{
	if (attacken.empty()) return; // Sicherheit

	if (held.dotRunden > 0) {
		Attacke dotAngriff;
		for (int i = 0; i < attacken.size(); i++) {
			if (attacken[i].effekt == "Dot")
			{
				dotAngriff = attacken[i];
				attacken.erase(attacken.begin() + i);
				int index = rand() % attacken.size();  // Zufallsindex
				Attacke& atk = attacken[index];        // Referenz zur Attacke
				atk.ausfuehren(gegner, held);
				attacken.insert(attacken.begin() + i, dotAngriff);
			}
		}

	}
	else
	{

		int index = rand() % attacken.size();  // Zufallsindex
		Attacke& atk = attacken[index];        // Referenz zur Attacke
		atk.ausfuehren(gegner, held);
	}
}
