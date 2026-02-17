#include <iostream>
#include <Windows.h>
#include "Display.h"
#include "Held.h"
#include "Gegner.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>      // _kbhit(), _getch()
using namespace std;

// --------------------------------------------
// Statik-Variablen initialisieren
// --------------------------------------------
int Display::logLine = 10;
int Display::centeredLine = 10;

// --------------------------------------------
// ASCII-Figuren der Gegner
// --------------------------------------------
vector<string> asciiProkastination = {
	" [===]  ",
	"  -|-  ",
	"  / \\  ",
	"Prokastination"
};

vector<string> asciiKomplexitaet = {
	" \\O_O/",
	"  |X| ",
	" / | \\",
	"Komplex"
};

vector<string> asciiZweifel = {
	"  @   ",
	" <|>  ",
	" / \\  ",
	"Zweifel"
};

vector<string> asciiZeitdruck = {
	"  @@@ ",
	" @-!-@",
	"  @@@ ",
	"  | | ",
	"Druck"
};
// Alle Gegner-Figuren zusammenfassen
vector<vector<string>> asciiGegner = { asciiProkastination, asciiZweifel, asciiZeitdruck, asciiKomplexitaet };

// Held-Figur
vector<string> asciiHeld = {
	"  O   ",
	" /|\\  ",
	" / \\  ",
	"Held"
};

// --------------------------------------------
// ASCII-Darstellungen der Items
// --------------------------------------------
vector<string> asciiPostIt = {
	"  _______  ",
	" |       | ",
	" | GO!!  | ",
	" |_______| "
};

vector<string> asciiAmulett = {
	"   /\\   ",
	"  /  \\  ",
	"  \\__/  ",
	"   ||   "
};

vector<string> asciiSchluessel = {
	"   __   ",
	"  / o\\  ",
	"  \\__/  ",
	"   ||   "
};

vector<string> asciiKaffee = {
	"  (  )  ",
	"   )(   ",
	"  [__] )",
	"  |__|  "
};

// Alle Item-Figuren zusammenfassen
vector<vector<string>> asciiItems = {
	asciiPostIt,
	asciiAmulett,
	asciiKaffee,
	asciiSchluessel
};

// ============================================
// LOG-FUNKTIONEN
// ============================================

void Display::printLog(const std::string& text) {
	vector<string> lines;
	stringstream ss(text);
	string line;
	while (getline(ss, line, '\n')) {
		lines.push_back(line);
	}
	for (size_t i = 0; i < lines.size(); i++) {
		cout << "\033[" << logLine << ";" << "1H" << lines[i];
	}
	logLine += static_cast<int>(lines.size());
	cout.flush();
}

//Wartet auf ENTER nach Textausgabe
void Display::printLogSlow(const std::string& text) {

	vector<string> lines;
	stringstream ss(text);
	string line;

	while (getline(ss, line, '\n')) {
		lines.push_back(line);
	}
	bool skipAnimation = false;

	for (size_t i = 0; i < lines.size(); ++i) {
		cout << "\033[" << logLine << ";1H";
		for (char c : lines[i]) {
			if (_kbhit()) {  // wenn ENTER gedrückt → skip
				char ch = _getch();
				if (ch == '\r') skipAnimation = true;
			}

			cout << c << flush;
			if (!skipAnimation && c != ' ' && c != '\n')
				this_thread::sleep_for(chrono::milliseconds(25));

		}
		logLine++;
	}

	// Jetzt wirklich auf ENTER warten
	while (true) {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == '\r') break;
		}
		this_thread::sleep_for(chrono::milliseconds(20));
	}

	cout.flush();
}

void Display::resetLog() {

	int startLine = 10;
	int endLine = 30;

	for (int i = startLine; i <= endLine; i++) {
		cout << "\033[" << i << ";1H\033[2K";
	}
	logLine = 10;
}

// ============================================
// HILFSFUNKTIONEN
// ============================================

int Display::getConsoleWidth() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	static int width = 80; // Fallback
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	}
	return width;
}

// ============================================
// TEXT ZENTRIERT DRUCKEN
// ============================================

void Display::printCentered(const std::string& text)//Wartet auf ENTER nach Textausgabe
{
	// Konsolenbreite
	static int consoleWidth = Display::getConsoleWidth();
	// Text in Zeilen splitten
	vector<string> lines;
	stringstream ss(text);
	string line;
	while (getline(ss, line, '\n')) {
		lines.push_back(line);
	}
	// Breite berechnen anhand laengster Zeile
	size_t maxLen = 0;
	for (auto& l : lines) {
		maxLen = max(maxLen, l.size());
	}
	int col = (consoleWidth - maxLen) / 2;
	if (col < 1) col = 1;

	// Cursor in die Zeile setzen + Spalte (col)
	cout << "\033[" << (centeredLine) << ";" << col << "H";

	bool skipAnimation = false;

	// Tipp-Animation mit Überspringen per ENTER
	for (size_t i = 0; i < lines.size(); ++i) {
		cout << "\033[" << (centeredLine + 1 + i) << ";" << col << "H";
		for (char c : lines[i]) {
			if (_kbhit()) {  // wenn ENTER gedrückt → skip
				char ch = _getch();
				if (ch == '\r') skipAnimation = true;
			}

			cout << c << flush;
			if (!skipAnimation && c != ' ' && c != '\n')
				this_thread::sleep_for(chrono::milliseconds(25));
		}
	}
	// Jetzt wirklich auf ENTER warten
	while (true) {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == '\r') break;
		}
		this_thread::sleep_for(chrono::milliseconds(20));
	}

	centeredLine += static_cast<int>(lines.size() + 1);
	cout.flush();
}

void Display::printCenteredQuick(const std::string& text)
{
	// Konsolenbreite
	static int consoleWidth = Display::getConsoleWidth();
	// Text in Zeilen splitten
	vector<string> lines;
	stringstream ss(text);
	string line;
	while (getline(ss, line, '\n')) {
		lines.push_back(line);
	}
	// Breite berechnen anhand laengster Zeile
	size_t maxLen = 0; for (auto& l : lines) { maxLen = max(maxLen, l.size()); }
	int boxWidth = static_cast<int>(maxLen) + 4;
	int col = (consoleWidth - boxWidth) / 2;
	if (col < 1) col = 1;

	// Cursor in die Zeile setzen + Spalte (col) 
	cout << "\033[" << (centeredLine) << ";" << col << "H";
	//Text drucken
	for (size_t i = 0; i < lines.size(); i++) {
		cout << "\033[" << (centeredLine + 1 + i) << ";" << col << "H" << lines[i];
	}
	centeredLine += static_cast<int>(lines.size() + 1); cout.flush();
}

// ============================================
// TEXT IN BOX MIT ANIMATION
// ============================================

void Display::printBoxed(const std::string& centerOrLog, const std::string& text) {
	static int consoleWidth = Display::getConsoleWidth();

	// Textzeilen vorbereiten
	vector<string> lines;
	stringstream ss(text);
	string line;
	while (getline(ss, line, '\n')) {
		lines.push_back(line);
	}

	// Breite der Box berechnen
	size_t maxLen = 0;
	for (auto& l : lines) maxLen = max(maxLen, l.size());
	int boxWidth = static_cast<int>(maxLen) + 4;
	int col = (consoleWidth - boxWidth) / 2;
	if (col < 1) col = 1;

	int startLine = (centerOrLog == "c") ? centeredLine : logLine;

	// Box-Rahmen zeichnen
	cout << "\033[" << startLine << ";" << col << "H"
		<< "+" << string(boxWidth - 2, '-') << "+";
	for (size_t i = 0; i < lines.size(); ++i) {
		cout << "\033[" << (startLine + 1 + i) << ";" << col << "H"
			<< "| " << string(maxLen, ' ') << " |";
	}
	cout << "\033[" << (startLine + 1 + lines.size()) << ";" << col << "H"
		<< "+" << string(boxWidth - 2, '-') << "+";
	cout.flush();

	bool skipAnimation = false;

	// Tipp-Animation mit Überspringen per ENTER
	for (size_t i = 0; i < lines.size(); ++i) {
		cout << "\033[" << (startLine + 1 + i) << ";" << (col + 2) << "H";
		for (char c : lines[i]) {
			if (_kbhit()) {  // wenn ENTER gedrückt → skip
				char ch = _getch();
				if (ch == '\r') skipAnimation = true;
			}

			cout << c << flush;
			if (!skipAnimation && c != ' ' && c != '\n')
				this_thread::sleep_for(chrono::milliseconds(25));
		}
	}

	// Cursor unterhalb der Box positionieren
	int nextLine = startLine + static_cast<int>(lines.size()) + 2;
	cout << "\033[" << nextLine << ";" << col << "H"
		<< "[Druecke ENTER um fortzufahren...]";
	cout.flush();

	// Jetzt wirklich auf ENTER warten
	while (true) {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == '\r') break;
		}
		this_thread::sleep_for(chrono::milliseconds(20));
	}

	// Zeile danach aufräumen
	cout << "\033[" << nextLine << ";" << col << "H"
		<< string(boxWidth, ' ') << flush;

	if (centerOrLog == "c")
		centeredLine = nextLine + 1;
	else
		logLine = nextLine + 1;
}

// ============================================
// HUD & Gegneranzeige
// ============================================

void Display::printHUD(Held& held, Gegner& gegner, int gegnerNum) {

	cout << "\033[1;1H"; // Cursor: Zeile 1, Spalte 1
	cout << "\033[2K";   // Zeile 1 loeschen
	vector<string>& gegnerFigur = asciiGegner[gegnerNum];
	for (size_t i = 0; i < asciiHeld.size() - 1; i++) {
		cout << asciiHeld[i] << "                                                           \033[38;5;124m" << gegnerFigur[i] << "\033[0m" << endl;
	}


	cout << held.name << "  HP: [" << string(held.hp * 20 / held.maxHp, '#') << string(20 - held.hp * 20 / held.maxHp, ' ') << "] "
		<< held.hp << "/" << held.maxHp;

	//cout << "\n\033[2K"; // Zeile 2 loeschen
	cout << "                       " << gegner.name << " HP: [" << string(gegner.hp * 20 / gegner.maxHp, '#') << string(20 - gegner.hp * 20 / gegner.maxHp, ' ') << "] "
		<< gegner.hp << "/" << gegner.maxHp << "\n";

	cout.flush();
	cout << "\033[" << logLine << ";1H";

}

void Display::printGegner(int gegnerNum) {
	vector<string>& gegnerFigur = asciiGegner[gegnerNum];
	for (size_t i = 0; i < asciiGegner[gegnerNum].size() - 1; i++) {
		printCentered(gegnerFigur[i] + "\n");
	}
}

// ============================================
// Bildschirm löschen
// ============================================

void Display::clearScreen() {
	std::cout << "\033[2J"; // Bildschirm loeschen
	std::cout << "\033[1;1H"; // Cursor auf Zeile 1, Spalte 1 setzen
	std::cout.flush();

	// Log & Centered Line zuruecksetzen
	logLine = 10;
	centeredLine = 10;
}