
#include "Attacke.h"
#include "Held.h"
#include "Gegner.h"
#include "Display.h"
#include <iostream>
using namespace std;

// --------------------------------------------
// Konstruktoren
// --------------------------------------------
Attacke::Attacke(string n, int s, string e)
	: name(n), schaden(s), effekt(e) {
}

Attacke::Attacke()
	: name(""), schaden(0), effekt("") {
}

// --------------------------------------------
// Angriff von Gegner auf Held
// --------------------------------------------
void Attacke::ausfuehren(Gegner& angreifer, Held& ziel)
{
	// Verteidigung berücksichtigen

	if (ziel.verteidigungAktiv == true) {
		angreifer.angriffskraft -= 0.5;
		ziel.verteidigungAktiv = false;
	}

	// Standard-Schaden anwenden

	int hpLost = schaden * angreifer.angriffskraft;
	ziel.hp -= hpLost;
	Display::printLogSlow("\033[38;5;124m" + name + "\033[0m trifft " + ziel.name + "!\n");
	if (angreifer.angriffskraft < 1) { // Angriffskraft anpassen, falls geschwächt
		Display::printLog("\033[36m" + angreifer.name + "s Angriff war geschwaecht.\n\033[0m");
		angreifer.angriffskraft = 1;
	}
	if (ziel.hp < 0) ziel.hp = 0; // keine negativen HP
	Display::printLog("-> " + ziel.name + " verliert \033[38;5;124m" + to_string(hpLost) + " HP\033[0m");


	// Zusaetzliche Effekte
	if (effekt == "Debuff") {
		Display::printLogSlow("\033[38;5;202m" + ziel.name + " fuehlt sich unsicher - "
			+ "naechster Angriff ist geschwaecht!\n \033[0m");
		ziel.boostAktiv = false;   // Beispiel: Motivation wird deaktiviert
		ziel.angriffskraft -= 0.25;
	}
	else if (effekt == "Dot") { // Damage over Time
		Display::printLog("-> " + ziel.name + " wird weiter geschwaecht \033[38;5;202m(tickender Schaden[-" + to_string(schaden) + "] fuer 3 Runden)\033[0m !");
		ziel.dotRunden = 3;
		ziel.dotSchaden = schaden;
	}

}

// --------------------------------------------
// Angriff von Held auf Gegner
// --------------------------------------------
void Attacke::ausfuehren(Held& angreifer, Gegner& ziel)
{
	Display::resetLog();
	Display::printLogSlow(angreifer.name + " setzt " + name + " ein!\n");
	Display::resetLog();

	// Temporaeren Boost pruefen
	if (angreifer.boostAktiv == true) {
		angreifer.angriffskraft = 1.5; //Schaden um 50% erhoeht
		angreifer.boostAktiv = false;
	}

	// Basis-Schaden
	if (schaden > 0) {
		int hpLost = schaden * angreifer.angriffskraft * angreifer.permBuff;
		ziel.hp -= hpLost;
		if (ziel.hp < 0) ziel.hp = 0; // kein negativer HP

		Display::printLogSlow(name + " trifft " + ziel.name + " ! ( -" + to_string(hpLost) + " HP )\n");

		// Angriffskraft prüfen
		if (angreifer.angriffskraft < 1) {
			Display::printLogSlow("\033[31m" + angreifer.name + "s Angriff war geschwaecht.\033[0m");
			angreifer.angriffskraft = 1;
		}
		if (angreifer.angriffskraft > 1) {
			Display::printLogSlow("\033[36m" + angreifer.name + "s Angriff war besonders stark.\033[0m");
			angreifer.angriffskraft = 1;
		}

		// Permanente Buff-Infos
		if (angreifer.permBuff > 1.0) {
			Display::printLogSlow("\033[36m" + angreifer.name + "'s permanente Staerke (+"
				+ std::to_string(int((angreifer.permBuff - 1.0) * 100)) + "%) wirkt!\033[0m");
		}


	}

	// Keine direkten HP-Schaden, nur Effekte
	if (schaden == 0)
	{
		if (effekt == "Shield") {
			Display::printLogSlow("\033[36m" + angreifer.name + " fuehlt sich motiviert!\n\033[0m");
			angreifer.hp += 20;
			if (angreifer.hp >= angreifer.maxHp) angreifer.hp = angreifer.maxHp;
			Display::printHUD(angreifer, ziel, ziel.id);
			Display::printLogSlow("-> " + angreifer.name + "s HP regenerieren sich etwas \033[36m(+20HP)\033[0m");
			Display::printLogSlow("-> Naechster Angriff von " + ziel.name + "\033[36m 50% schwaecher\033[0m");
			angreifer.verteidigungAktiv = true;
		}
		else if (effekt == "Buff") { // Damage over Time
			Display::printLogSlow("\033[36mTeamarbeit aktiviert!\033[0m");
			Display::printLogSlow("-> Naechster Angriff von " + angreifer.name + "\033[36m 50% staerker\033[0m");
			angreifer.boostAktiv = true;

		}

	}

}

