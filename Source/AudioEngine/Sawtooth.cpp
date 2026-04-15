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
        // Phasen gleichm��ig verteilen f�r volleren Klang
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
        leftOutput = rightOutput = saw * volume;
        return saw * volume;
    }

    int activeOscs = static_cast<int>(1 + spread * 7); // 1-8 Oszillatoren
    float rmsScale = scales[activeOscs - 1];

    if (width <= 0.0f) {
        // Mono supersaw (existing behaviour)
        value = 0;
        for (int i = 0; i < activeOscs; i++)
            value += (float)blitsaw[i]->tick();
        saw = value * rmsScale;
        leftOutput = rightOutput = saw * volume;
        return saw * volume;
    }

    // Stereo supersaw:
    //   i=0            → fundamental, centred
    //   i odd  (≥1)   → positively detuned  → biased right
    //   i even (≥2)   → negatively detuned  → biased left
    float leftSum = 0.0f, rightSum = 0.0f;
    for (int i = 0; i < activeOscs; i++) {
        float s = (float)blitsaw[i]->tick();
        if (i == 0) {
            leftSum  += s;
            rightSum += s;
        } else if (i % 2 == 1) {
            // positive detune → right
            leftSum  += s * (0.5f - 0.5f * width);
            rightSum += s * (0.5f + 0.5f * width);
        } else {
            // negative detune → left
            leftSum  += s * (0.5f + 0.5f * width);
            rightSum += s * (0.5f - 0.5f * width);
        }
    }

    leftOutput  = leftSum  * rmsScale * volume;
    rightOutput = rightSum * rmsScale * volume;

    // Mono fallback value (average for getOutput())
    saw = (leftSum + rightSum) * 0.5f * rmsScale;
    return saw * volume;
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

        // Frequenzabh�ngige Skalierung f�r nat�rlicheren Klang
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