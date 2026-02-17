#include "Spiel.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include "Held.h"
#include "Item.h"
#include "Gegner.h"
#include "Attacke.h"
#include "Inventar.h"
#include "Display.h"
#include <chrono>
#include <thread>

// --------------------------------------------
// Globale Variable
// --------------------------------------------
bool spielLaueft = true;

// --------------------------------------------
// Konstruktor
// --------------------------------------------
Spiel::Spiel(string spielerName) : spieler(spielerName)   // <--- so!
{
	// ---------------- Gegner 1: Prokrastination ----------------
	vector<Attacke> procrastAtk = {
		Attacke("YouTube-Video", 10),
		Attacke("Serien-Marathon", 2, "Dot")
	};
	Item postIt("Motivations Post it", "ExtraTurn", 100);
	Gegner prokrastination("Prokrastination", 0, 80, procrastAtk, postIt);

	// ---------------- Gegner 2: Zweifel ----------------
	vector<Attacke> zweifelAtk = {
	   Attacke("'Du bist nicht gut genug!'", 15),
	   Attacke("Unsicherheit", 0 , "Debuff")
	};
	Item amulett("Selbstvertrauens-Amulett", "PermBuff", 25);
	Gegner zweifel("Zweifel", 1, 90, zweifelAtk, amulett);

	// ---------------- Gegner 3: Zeitdruck ----------------
	vector<Attacke> zeitdruckAtk = {
	   Attacke("Tickende Uhr", 7 , "Dot"),
	   Attacke("Deadline-Stress", 25)
	};
	Item kaffee("Kaffee", "Heilung", 100);
	Gegner zeitdruck("Zeitdruck", 2, 100, zeitdruckAtk, kaffee);

	// ---------------- Gegner 4: Komplexitaet ----------------
	vector<Attacke> komplexAtk = {
	   Attacke("Ueberforderung", 10 , "Debuff"),
	   Attacke("Endlose Aufgabenliste", 10, "Dot")
	};
	Item schluessel("Mysterioeser Schluessel", "???", 0);
	Gegner komplexitaet("Komplexitaet", 3, 90, komplexAtk, schluessel);

	// Gegnerliste zusammenstellen
	gegnerListe = { prokrastination,zweifel,zeitdruck,komplexitaet };

	// ---------------- Story-Elemente ----------------
	story = {
		// Einstieg
		{"Du sitzt an deinem Schreibtisch, bereit fuer dein grosses Abenteuer in der Welt der Informatik.\n"
		 "Der Bildschirm leuchtet, Tastatur und Maus liegen vor dir wie treue Gefaehrten.",
		false, -1, ""
		},

		{"Doch schon ahnst du: Auf dem Weg zum Tor der Informatik warten unerwartete Huerden, die deinen Mut,\ndeine Kreativitaet und dein Durchhaltevermoegen testen werden.\n"
		 "Jede Herausforderung ist ein kleines Abenteuer fuer sich.\nNur wer clever und entschlossen ist, wird es meistern.",
		 false, -1, ""
		},


		// Gegner 1: Prokrastination
		{"Ploetzlich hoerst du ein leises Rascheln. Du blickst auf und siehst Prokrastination vor dir!\n"
		 "Er liegt gemuetlich in einer kuscheligen Decke eingewickelt, ein Haufen Serien-DVDs auf dem Schoss,\n"
		 "ein Tablet in der Hand, das ununterbrochen neue YouTube-Videos abspielt. Sein Blick fordert dich heraus:\n"
		 "'Komm schon, setz dich neben mich und chill eine Runde..Du willst doch nicht gleich alles richtig machen, oder?' \n"
		 "Sein Auftritt ist laessig, aber hinter dieser Gemuetlichkeit lauert die Gefahr: Wenn du dich ablenken laesst,\n"
		 "verlierst du wertvolle Zeit auf deinem Weg zum Tor der Informatik.",
		 true, 0, ""},

		// Nach Prokrastination
		{"Du hast Prokrastination erfolgreich besiegt! Mit einem letzten Schwung wirfst du die Serien-DVDs zur Seite.\n"
		 "Vor dir liegt ein Motivations-Post-it, das hell aufleuchtet. Es fluestert: 'Jetzt nicht aufgeben, weiter kaempfen!'\n",
		 false, -1,
		 "  _____\n"
		 " |     |\n"
		 " | GO! |\n"
		 " |_____|\n"},

		{
		 "Wenn du es clever einsetzt, bekommst du einen Extra-Zug im Kampf...ueberlege gut, wann der richtige Moment ist.\n"
		 "Dieses kleine Item koennte den Unterschied zwischen Sieg und Niederlage ausmachen. Nutze deine Ressourcen weise!",
		 false, -1,
		 "  _____\n"
		 " |     |\n"
		 " | GO! |\n"
		 " |_____|\n"},

		// Gegner 2: Zweifel
		{"Kaum hast du durchgeatmet, schleicht sich ein leises Murmeln in deinen Kopf - Zweifel!\n"
		 "Mit fiesen Spruechen wie 'Bist du wirklich gut genug?' oder 'Vielleicht solltest du noch warten' versucht er,\n"
		 "deine Entschlossenheit zu erschuettern. Es ist ein Gegner, der deine eigenen Gedanken gegen dich einsetzt.\n"
		 "Wirst du ihm widerstehen und weiter deinen Weg gehen?",
		 true, 1, ""},

		// Nach Zweifel
		{"Zweifel liegt besiegt am Boden, ueberwunden durch deinen Mut und deine Entschlossenheit.\n"
		 "Er hinterlaesst ein Selbstvertrauens-Amulett. Dieses Amulett verstaerkt permanent deine Angriffskraft im Kampf.\n"
		 "Trage es weise, es wird dir helfen, zukuenftige Herausforderungen zu meistern.",
		 false, -1,
		 "  \\    /\n"
		 "   \\/\\/ \n"
		 "   /  \\ \n"
		 "   \\__/ \n"
		 },

		// Gegner 3: Zeitdruck
		{"Noch immer am Durchatmen, hoerst du ein unaufhoerliches Ticken...Zeitdruck steht vor dir!\n"
		 "Deadlines, Abgabetermine und tickende Uhren setzen dir zu. Jeder Moment zaehlt.\n"
		 "Kannst du trotz des Drucks die Ruhe bewahren und deine naechsten Schritte planen?",
		 true, 2, ""},

		// Nach Zeitdruck
		{"Zeitdruck wurde gemeistert. Dein Herz beruhigt sich.\n"
		 "Er hinterlaesst einen frisch gebruehten Kaffee. Sobald du ihn trinkst, werden deine HP sofort vollstaendig aufgefuellt.\n"
		 "Perfekt, um fuer den naechsten Kampf topfit zu sein und weiterzumachen.",
		 false, -1,
		 "   ( (\n"
		 "    ) )\n"
		 "  .....\n"
		 "  |   |\n"
		 "   `-'\n"},

		// Gegner 4: Komplexitaet
		{"Die groesste Herausforderung erhebt sich nun vor dir: Komplexitaet.\n"
		 "Unuebersichtliche Aufgaben, verworrene Logik und endlose Problemstellungen fordern deine volle Konzentration.\n"
		 "Nur wer clever und vorbereitet ist, kann diesen Gegner bezwingen.",
		 true, 3, ""},

		// Nach Komplexitaet
		{"Komplexitaet liegt besiegt am Boden. Vor dir liegt ein mysterioeser Schluessel, der geheimnisvoll im Licht der Konsole schimmert.\n"
		 "Wofuer mag dieser Schluessel wohl sein? Ein altes Schloss? Eine versteckte Schatzkammer?...",
		 false, -1,
		 "       __        \n"
		"      /o \\=======|> \n"
		"      \\__/       \n"}
	};
}

// --------------------------------------------
// Ende der Story: Animation Tor & Schluessel
// --------------------------------------------
void Spiel::schluss() {
	// Tor und Schluessel in ASCII
	const string tor[] = {
		"    _________    ",
		"   |         |   ",
		"   |         |   ",
		"   |         |   ",
		"   |         |   ",
		"   |         |   ",
		"   |         |   ",
		"   |_________|   "
	};

	const string schluessel[] = {
		"       __        ",
		"      /o \\=======|> ",
		"      \\__/       "
	};

	// Bildschirm vorbereiten

	Display::clearScreen();
	Display::printBoxed("c", " Neugierig und voller Vorfreude machst du dich auf den Weg.\n Nach langem Wandern erhebt sich vor dir ploetzlich ein riesiges,\n majestaetisches Tor, das den Weg versperrt.");

	Display::clearScreen();
	Display::printBoxed("c", "''Koennte dieser geheimnisvolle Schluessel passen..?''\n  denkst du, waehrend deine Hand zum Inventar wandert.");

	if (spieler.inventory.zeigeInventar(spieler)) {
		// Animation: Schluessel bewegt sich Richtung Tor
		for (int i = 0; i <= 10; ++i) {
			Display::clearScreen();
			// Tor ausgeben
			for (auto& line : tor) cout << "                         " << line << "\n";

			// Schluesselposition
			for (int j = 0; j < 3; ++j) {
				cout << string(i, ' ') << schluessel[j] << "\n";
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		// Animation: Tor oeffnet sich
		for (int step = 0; step <= 25; ++step) {
			Display::clearScreen();

			cout << "                         ";
			for (int i = 0; i < step; ++i) cout << " ";
			cout << "_______";
			for (int i = 0; i < step; ++i) cout << " ";
			cout << "\n";

			cout << "                         |" << string(step, ' ') << "       " << string(step, ' ') << "|\n";
			cout << "                         |" << string(step, ' ') << "       " << string(step, ' ') << "|\n";
			cout << "                         |" << string(step, ' ') << "       " << string(step, ' ') << "|\n";
			cout << "                         |" << string(step, ' ') << "       " << string(step, ' ') << "|\n";
			cout << "                         |" << string(step, ' ') << "       " << string(step, ' ') << "|\n";
			cout << "                         |" << string(step, ' ') << "       " << string(step, ' ') << "|\n";
			cout << "                         |" << string(step, ' ') << "_______" << string(step, ' ') << "|\n";

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		// Endbild
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		Display::clearScreen();

		Display::printCentered("Mit einem leisen Klicken passt der Schluessel perfekt ins Schloss.\n");
		Display::printCentered("Das Tor oeffnet sich langsam, quietschend und funkelnd..");
		Display::printCentered("Die Welt der Informatik liegt nun vor dir.");
		Display::clearScreen();
		Display::printCentered("Durch deine besiegten Huerden, deinen Mut und deine Cleverness hast du dir den Weg zu neuen Abenteuern geoeffnet.\n");
		Display::printCentered("Glueckwunsch, tapferer Held! Dein Abenteuer geht weiter...moege der Code mit dir sein.");
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		exit(0);
	}
}

// --------------------------------------------
// Game Over Screen
// --------------------------------------------
void Spiel::gameOverScreen() {
	Display::clearScreen();
	Display::resetLog();

	// kleine Pause fuer Effekt
	Display::printCentered("...");
	Display::clearScreen();

	// Szene beschreiben
	Display::printCentered("Dein Held sinkt erschoepft auf die Tastatur.");

	Display::printCentered("Ein leises Summen des Monitors bleibt als einziges Geraeusch zurueck...");

	Display::printCentered("Anscheinend warst du doch noch nicht ganz bereit fuer die Huerden dieses Abenteuers.");

	Display::printCentered("Aber hey...");

	Display::printCentered("dein Wille bleibt im Speicher der Informatik bestehen.");

	Display::clearScreen();
	Display::printCenteredQuick("\n-------------------------------------------");
	Display::printCenteredQuick("              G A M E   O V E R            ");
	Display::printCenteredQuick("-------------------------------------------\n");

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	// Auswahlmenue
	int choice = 0;
	while (true) {
		Display::printCenteredQuick("Was moechtest du tun?");
		Display::printCenteredQuick("[1] Neustart");
		Display::printCenteredQuick("[2] Beenden");
		std::cout << "\n> ";
		std::cin >> choice;


		if (std::cin.fail() || (choice != 1 && choice != 2)) {

			std::cin.ignore(10000, '\n');
			std::cin.clear();
			Display::printLog("Ungueltige Eingabe. Bitte 1 oder 2 waehlen.\n");
			continue;
		}
		break;
	}

	if (choice == 1) {
		Display::clearScreen();
		Display::printCentered("Ein neuer Versuch beginnt... der Code kompiliert wieder!");
		Display::printCentered("Druecke Enter um fortzufahren...");
		// Held wiederherstellen
		spieler.hp = spieler.maxHp;
		spieler.boostAktiv = false;
		spieler.verteidigungAktiv = false;
		spieler.dotRunden = 0;

		// Gegner zuruecksetzen
		for (auto& g : gegnerListe) {
			g.hp = g.maxHp;
		}
		spielLaueft = true;
		Display::clearScreen();
		spieleStory(); // Neustart der Story
	}
	else {
		Display::clearScreen();
		Display::printCentered("Du schaltest den Monitor aus...");
		Display::printCentered("und weisst: Du wirst zurueckkehren.");
		Display::printCentered("\nBis bald, tapferer Informatikheld!");
		exit(0);
	}
}


// --------------------------------------------
// Kampf gegen Gegner
// --------------------------------------------
void Spiel::kampf(Held& spieler, int gegner)
{
	Display::clearScreen();
	Display::printCentered("Der Kampf gegen " + gegnerListe[gegner].name + " beginnt!!!\n");
	int extraZuege = 0;

	while (spieler.hp > 0 && gegnerListe[gegner].hp > 0) {
		Display::resetLog();
		Display::printHUD(spieler, gegnerListe[gegner], gegner);
		Display::printLog("Du bist dran! \n");


		bool zugBeendet = false;
		while (!zugBeendet) {
			Display::resetLog();
			spieler.zeigeAttacken();

			int wahl;
			while (true) {
				Display::printLog("Auswahl: ");
				cin >> wahl;
				// Pruefen, ob die Eingabe gueltig ist
				if (cin.fail())
				{
					cin.clear();             // Fehlerzustand zuruecksetzen
					cin.ignore(10000, '\n'); // falsche Eingabe verwerfen
					Display::printLog("Ungueltige Eingabe! Bitte Zahl eingeben.\n");
					continue;                // Schleife erneut
				}

				if (wahl >= 1 && wahl <= 5) {
					break;  // gueltig → Schleife verlassen
				}
				else {
					Display::printLog("Ungueltige Zahl! Bitte zwischen 1 und 5 waehlen.\n");
				}
			}

			switch (wahl) {
			case 1: spieler.angreifen(spieler, gegnerListe[gegner], spieler.attackenListe[0]); zugBeendet = true; break;
			case 2: spieler.angreifen(spieler, gegnerListe[gegner], spieler.attackenListe[1]); zugBeendet = true; break;
			case 3: spieler.angreifen(spieler, gegnerListe[gegner], spieler.attackenListe[2]); zugBeendet = true; break;
			case 4: spieler.angreifen(spieler, gegnerListe[gegner], spieler.attackenListe[3]); zugBeendet = true; break;
			case 5: if (spieler.inventory.zeigeInventar(spieler)) {
				zugBeendet = true;
			}
				  else {
				extraZuege++;
			} break;
			default: Display::printLog("Ungueltige Eingabe!\n"); break;
			}

			Display::printHUD(spieler, gegnerListe[gegner], gegner);

			if (spieler.extraZug) {

				spieler.extraZug = false;
				extraZuege = 2;
			}
			if (extraZuege > 0) {
				extraZuege--;   // einen zusätzlichen Zug verbrauchen
				zugBeendet = false;
			}
			else {
				zugBeendet = true;  // kein Extra-Zug mehr → Zug beenden
			}
		}


		// Gegner tot → Item drop
		if (gegnerListe[gegner].hp <= 0) {
			spieler.dotRunden = 0;
			spieler.inventory.fuegeItemHinzu(gegnerListe[gegner].drop);
			Display::clearScreen();
			Display::printCentered(gegnerListe[gegner].name + " wurde besiegt!");
			Display::printCenteredQuick(gegnerListe[gegner].drop.name);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			spieler.besiegteGegner++;
			return;
		}

		// Gegnerzug
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		Display::clearScreen();
		Display::printHUD(spieler, gegnerListe[gegner], gegner);
		Display::resetLog();
		Display::printLogSlow("\033[38;5;124m" + gegnerListe[gegner].name + "\033[0m ist am Zug!\n");
		Display::resetLog();
		gegnerListe[gegner].zeigeAttacken();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		Display::resetLog();
		gegnerListe[gegner].zufallsAngriff(gegnerListe[gegner], spieler);
		Display::printHUD(spieler, gegnerListe[gegner], gegner);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// Dot Schaden
		if (spieler.dotRunden > 0) {
			Attacke dotAngriff;
			for (int i = 0; i < gegnerListe[gegner].attacken.size(); i++) {
				if (gegnerListe[gegner].attacken[i].effekt == "Dot")
				{
					dotAngriff = gegnerListe[gegner].attacken[i];
				}
			}
			spieler.hp -= spieler.dotSchaden;
			if (spieler.hp < 0) spieler.hp = 0;
			Display::resetLog();
			Display::printHUD(spieler, gegnerListe[gegner], gegner);
			Display::printLogSlow(spieler.name + " erleidet \033[38;5;124m" + to_string(spieler.dotSchaden)
				+ " \033[0m Schaden durch \033[38;5;202m" + dotAngriff.name + "\033[0m!(HP: " + to_string(spieler.hp) + " / " + to_string(spieler.maxHp) + ")\n\033[0m");
			spieler.dotRunden--;
		}

		// Spieler tot → Game Over
		if (spieler.hp <= 0) {
			spielLaueft = false;
			gameOverScreen();
			return;
		}
	}

}

// --------------------------------------------
// Story abspielen
// --------------------------------------------
void Spiel::spieleStory() {
	for (const auto& elem : story) {

		if (!spielLaueft)
		{
			return;
		}

		// Text anzeigen
		Display::clearScreen();
		if (elem.itemASCII != "")
		{
			Display::printCenteredQuick(elem.itemASCII + "\n");
		}

		Display::printBoxed("c", elem.text);

		if (spieler.besiegteGegner == 4) 
		{
			schluss();
			return;
		}

		// Kampf starten, falls vorgesehen
		if (elem.kampfStart && elem.gegnerIndex >= 0 && elem.gegnerIndex < gegnerListe.size()) {
			kampf(spieler, elem.gegnerIndex);
		}

	}

}

// --------------------------------------------
// Spiel starten
// --------------------------------------------
void Spiel::start()
{
	Display::printCenteredQuick("Wie lautet dein Name? ");
	string name;
	cin >> name;
	spieler.name = name;

	Display::clearScreen();
	Display::printCentered("Hello " + spieler.name + "!");
	Display::printCentered("Dein Abenteuer beginnt nun..");
	spieleStory();

}
