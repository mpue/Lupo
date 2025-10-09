/*
  ==============================================================================
    Sawtooth.cpp
    Created: 3 Jun 2016 9:02:07pm
    Author:  Matthias Pueski
  ==============================================================================
*/

#include "Sawtooth.h"
#include "../Stk.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

using namespace std;

Sawtooth::Sawtooth(float sampleRate, int buffersize) : Oszillator(sampleRate) {
    this->bufferSize = buffersize;
    this->volume = 1.0f;
    this->frequency = 440.0f;
    this->fine = 0.0f;
    this->lastValue = 0;
    this->saw = 0;
    this->spread = 0.0f;
    this->value = 0;
    this->totalWeight = 0;

    // Oszillatoren mit verteilten Phasen initialisieren
    for (int i = 0; i < 8; i++) {
        blitsaw[i] = new stk::BlitSaw();
        // Phasen gleichmäßig verteilen für volleren Klang
        // Falls BlitSaw keine setPhase() Methode hat, diese Zeilen entfernen
        // blitsaw[i]->setPhase(i / 8.0f);
    }
}

Sawtooth::~Sawtooth()
{
    for (int i = 0; i < 8; i++) {
        delete blitsaw[i];
    }
}

float Sawtooth::getOutput() {
    return this->saw;
}

void Sawtooth::reset() {
    for (int i = 0; i < 8; i++) {
        blitsaw[i]->reset();
    }
}

float Sawtooth::process() {
    // Oszillator-Sync
    if (this->slave != 0 && sync) {
        if (blitsaw[0]->resetFlag) {
            slave->reset();
            blitsaw[0]->resetFlag = false;
        }
    }

    // Ohne Spread nur den ersten Oszillator verwenden
    if (spread == 0.0f) {
        saw = (float)blitsaw[0]->tick();
        return saw * volume;
    }
    else {
        // Bestimme Anzahl aktiver Oszillatoren basierend auf spread
        int activeOscs = static_cast<int>(1 + spread * 7); // 1-8 Oszillatoren

        // Verwende den korrekten scale-Wert für die Gesamtzahl
        float rmsScale = scales[activeOscs - 1]; // Index ist activeOscs - 1

        value = 0;

        // Alle aktiven Oszillatoren mit gleichem Gewicht
        for (int i = 0; i < activeOscs; i++) {
            value += (float)blitsaw[i]->tick();
        }

        // Skaliere das Gesamtresultat
        saw = value * rmsScale;
        return saw * volume;
    }
}

void Sawtooth::setFrequency(double frequency)
{
    if (frequency == 0) {
        frequency = 0.01;
    }

    this->frequency = frequency;

    // Erster Oszillator auf Grundfrequenz
    blitsaw[0]->setFrequency(frequency + this->fine + pitchMod);

    // Restliche Oszillatoren symmetrisch verstimmen
    for (int i = 1; i < 8; i++) {
        float detune = scales[i - 1] * spread;

        // Abwechselnd positive und negative Verstimmung
        if (i % 2 == 0) {
            detune = -detune;
        }

        // Frequenzabhängige Skalierung für natürlicheren Klang
        float freqScale = 1.0f + (frequency / 20000.0f);

        blitsaw[i]->setFrequency(frequency + this->fine + pitchMod + (detune * freqScale));
    }
}

void Sawtooth::setSpread(float spread)
{
    this->spread = spread;
    // Frequenz neu setzen um Verstimmung zu aktualisieren
    setFrequency(this->frequency);
}

void Sawtooth::setFine(float _fine) {
    this->fine = _fine;
    setFrequency(frequency);
}

float Sawtooth::getFine() const {
    return this->fine;
}