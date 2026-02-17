#include "Inventar.h"
#include "Held.h"
#include "Item.h"
#include "Display.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

// --------------------------------------------
// Fügt ein Item dem Inventar hinzu
// --------------------------------------------
void Inventar::fuegeItemHinzu(const Item& item) {
	items.push_back(item);
	Display::printLog(item.name + " wurde ins Inventar gelegt.\n");
}

// ---------------------------------------------
// Nutzt und entfernt ein Item aus deem Inventar
// ---------------------------------------------
void Inventar::nutzeItem(int index, Held& ziel) {
	if (index < 0 || index >= items.size()) {
		Display::printLog("Ungueltiger Index!\n");
		return;
	}
	items[index].anwenden(ziel);
	items.erase(items.begin() + index); // nach Nutzung entfernen
}

// ---------------------------------------------
// Zeigt das Inventar an und erlaubt die Nutzung von Items
// Spezielle Logik, wenn der Spieler den geheimen Schlüssel hat
// ---------------------------------------------
bool Inventar::zeigeInventar(Held& held) {

	// Ab 4 besiegten Gegnern wird das spezielle Inventar-Menü angezeigt
	if (held.besiegteGegner >= 4) {


		while (true) {
			Display::clearScreen();
			Display::printCenteredQuick("------Inventar-------\n");

			if (items.empty()) {
				Display::printCenteredQuick("Inventar ist leer.\n");
			}
			else {
				for (int i = 0; i < items.size(); i++) {
					Display::printCenteredQuick(to_string(i + 1) + ") " + items[i].name + " (" + items[i].typ + ")\n");
				}
			}

			Display::printCenteredQuick("0: Zurueck\n");
			int wahl;
			Display::printCenteredQuick("\nAuswahl: ");
			cin >> wahl;

			// Eingabe validieren
			if (cin.fail())
			{
				cin.clear();             // Fehlerzustand zuruecksetzen
				cin.ignore(10000, '\n'); // falsche Eingabe verwerfen
				Display::printCenteredQuick("Ungueltige Eingabe! Bitte Zahl eingeben.\n");
				std::this_thread::sleep_for(std::chrono::milliseconds(3000));
				continue;                // Schleife erneut
			}

			if (wahl == 0) {
				Display::clearScreen();
				Display::printCentered("Jetzt zurueck zu gehen bringt dir nichts. Du musst den Schluessel ausprobieren.\n");
				continue;
			}

			// Benutzen eines Items außer dem geheimen Schlüssel
			if (!items.empty() && wahl > 0 && (size_t)wahl <= items.size() && items[wahl - 1].name != "Mysterioeser Schluessel") {
				Display::clearScreen();
				Display::printCentered("Du benutzt " + items[wahl - 1].name + " !\n Es scheint dir in dieser Situation nicht zu helfen..");
				continue;
			}
			// Benutzen des geheimen Schlüssels
			if (!items.empty() && wahl > 0 && (size_t)wahl <= items.size() && items[wahl - 1].name == "Mysterioeser Schluessel") {
				Display::clearScreen();
				Display::printCentered("Du benutzt " + items[wahl - 1].name + " !\n");
				return true; // Schlüssel wurde benutzt
			}

			if (wahl >= 0 && (size_t)wahl >= items.size()) {
				Display::printCenteredQuick("Ungueltige Eingabe! Gueltige Zahl auswaehlen.\n");
				std::this_thread::sleep_for(std::chrono::milliseconds(3000));
				continue;
			}
		}
	}

	// Standard-Inventar-Menü für Spieler
	else {
		Display::resetLog();
		Display::printLog("------Inventar-------\n");
		if (items.empty()) {
			Display::printLog("Inventar ist leer.\n");
		}
		else {
			for (int i = 0; i < items.size(); i++) {
				Display::printLog(to_string(i + 1) + ") " + items[i].name + " (" + items[i].typ + ")\n");
			}
		}

		Display::printLog("0: Zurueck\n");

		while (true) {
			int wahl;
			Display::printLog("Auswahl: ");
			cin >> wahl;
			if (cin.fail())
			{
				cin.clear();             // Fehlerzustand zuruecksetzen
				cin.ignore(10000, '\n'); // falsche Eingabe verwerfen
				Display::printLog("Ungueltige Eingabe! Bitte Zahl eingeben.\n");
				continue;                // Schleife erneut
			}
			if (wahl == 0) {
				return false; // Zurück ohne Item-Nutzung
			}
			if (!items.empty() && wahl > 0 && (size_t)wahl <= items.size()) {
				Display::printLogSlow("Du benutzt " + items[wahl - 1].name + " !\n");
				nutzeItem((wahl - 1), held);
				return true;
			}
			Display::printLog("Ungueltige Eingabe! Gueltige Zahl auswaehlen.\n");
		}

	}

}