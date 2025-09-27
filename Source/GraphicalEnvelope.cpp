/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Model.h"
//[/Headers]

#include "GraphicalEnvelope.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GraphicalEnvelope::GraphicalEnvelope (Model* model)
{
    //[Constructor_pre] You can add your own custom stuff here..
	this->model = model;
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	addMouseListener(this,true);
	attackHandler = new Rectangle<float>(10, 10,10,10);
	decayHandler = new Rectangle<float>(100,50, 10, 10);
	sustainHandler = new Rectangle<float>(200, 50, 10, 10);
	releaseHandler = new Rectangle<float>(300, 80, 10, 10);
	dashes = new float[2] {5, 5};
    //[/Constructor]
}

GraphicalEnvelope::~GraphicalEnvelope()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
	delete attackHandler;
	delete decayHandler;
	delete sustainHandler;
	delete releaseHandler;
	delete dashes;
    //[/Destructor]
}

//==============================================================================
void GraphicalEnvelope::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff616161));

    //[UserPaint] Add your own custom painting code here..

	g.setColour(Colours::grey);
	Line<float> a = Line<float>(attackHandler->getPosition().x + 5, 0, attackHandler->getPosition().x + 5, getHeight());
	Line<float> d = Line<float>(decayHandler->getPosition().x + 5, 0, decayHandler->getPosition().x + 5, getHeight());
	Line<float> s = Line<float>(sustainHandler->getPosition().x + 5, 0, sustainHandler->getPosition().x + 5, getHeight());
	Line<float> r = Line<float>(releaseHandler->getPosition().x + 5, 0, releaseHandler->getPosition().x + 5, getHeight());

	Line<float> level = Line<float>(0, sustainHandler->getPosition().y + 5, getWidth(), sustainHandler->getPosition().y + 5);

	g.drawDashedLine(a, dashes, 2, 1.0f);
	g.drawDashedLine(d, dashes, 2, 1.0f);
	g.drawDashedLine(s, dashes, 2, 1.0f);
	g.drawDashedLine(r, dashes, 2, 1.0f);
	g.drawDashedLine(level, dashes, 2, 1.0f);
	
	// Draw the envelope curve with more realistic curves
	g.setColour(Colours::darkgrey.darker());
	
	// Create a path for the envelope curve
	Path envelopePath;
	
	float startX = 15;
	float startY = getHeight();
	float attackX = attackHandler->getPosition().x + 5;
	float attackY = attackHandler->getPosition().y + 5;
	float decayX = decayHandler->getPosition().x + 5;
	float decayY = decayHandler->getPosition().y + 5;
	float sustainX = sustainHandler->getPosition().x + 5;
	float sustainY = sustainHandler->getPosition().y + 5;
	float releaseX = releaseHandler->getPosition().x + 5;
	float releaseY = getHeight();
	
	// Start the path
	envelopePath.startNewSubPath(startX, startY);
	
	// Attack phase - exponential curve
	envelopePath.quadraticTo(startX + (attackX - startX) * 0.3f, startY, attackX, attackY);
	
	// Decay phase - exponential decay curve
	envelopePath.quadraticTo(attackX + (decayX - attackX) * 0.7f, attackY, decayX, decayY);
	
	// Sustain phase - straight line
	envelopePath.lineTo(sustainX, sustainY);
	
	// Release phase - exponential decay curve
	envelopePath.quadraticTo(sustainX + (releaseX - sustainX) * 0.3f, sustainY, releaseX, releaseY);
	
	// Draw the smooth envelope curve
	g.strokePath(envelopePath, PathStrokeType(2.0f));
	
	// Also draw the original straight lines in a lighter color for reference
	g.setColour(Colours::darkgrey.darker().withAlpha(0.3f));
	g.drawLine(startX, startY, attackX, attackY, 1.0f);
	g.drawLine(attackX, attackY, decayX, decayY, 1.0f);
	g.drawLine(decayX, decayY, sustainX, sustainY, 1.0f);
	g.drawLine(sustainX, sustainY, releaseX, releaseY, 1.0f);

	// Draw real-time phase indicator
	if (currentPhase > 0) {
		float indicatorX = 15; // Start position
		float indicatorY = getHeight();
		
		// Get curve points
		float startX = 15;
		float startY = getHeight();
		float attackX = attackHandler->getPosition().x + 5;
		float attackY = attackHandler->getPosition().y + 5;
		float decayX = decayHandler->getPosition().x + 5;
		float decayY = decayHandler->getPosition().y + 5;
		float sustainX = sustainHandler->getPosition().x + 5;
		float sustainY = sustainHandler->getPosition().y + 5;
		float releaseX = releaseHandler->getPosition().x + 5;
		float releaseY = getHeight();
		
		// Calculate position based on current phase and envelope value
		switch (currentPhase) {
			case 1: // Attack phase (env_attack)
				{
					// Use quadratic curve for attack
					float t = currentPhaseValue; // Use envelope value as progress
					float controlX = startX + (attackX - startX) * 0.3f;
					float controlY = startY;
					
					// Quadratic Bezier curve calculation
					float u = 1.0f - t;
					indicatorX = u * u * startX + 2 * u * t * controlX + t * t * attackX;
					indicatorY = u * u * startY + 2 * u * t * controlY + t * t * attackY;
				}
				break;
				
			case 2: // Decay phase (env_decay)
				{
					// Calculate progress based on current value relative to sustain level
					float sustainLevel = (getHeight() - sustainHandler->getPosition().y - 5) / getHeight();
					float progress = 0.0f;
					if (sustainLevel < 1.0f) {
						progress = (1.0f - currentPhaseValue) / (1.0f - sustainLevel);
						progress = jlimit(0.0f, 1.0f, progress);
					}
					
					// Use quadratic curve for decay
					float controlX = attackX + (decayX - attackX) * 0.7f;
					float controlY = attackY;
					
					// Quadratic Bezier curve calculation
					float u = 1.0f - progress;
					indicatorX = u * u * attackX + 2 * u * progress * controlX + progress * progress * decayX;
					indicatorY = u * u * attackY + 2 * u * progress * controlY + progress * progress * decayY;
				}
				break;
				
			case 3: // Sustain phase (env_sustain)
				{
					// Linear interpolation for sustain phase
					indicatorX = decayX + (sustainX - decayX) * currentPhasePosition;
					indicatorY = sustainY;
				}
				break;
				
			case 4: // Release phase (env_release)
				{
					// Use current envelope value to position the indicator
					float sustainLevel = (getHeight() - sustainHandler->getPosition().y - 5) / getHeight();
					float progress = 0.0f;
					if (sustainLevel > 0.0f) {
						progress = 1.0f - (currentPhaseValue / sustainLevel);
						progress = jlimit(0.0f, 1.0f, progress);
					}
					
					// Use quadratic curve for release
					float controlX = sustainX + (releaseX - sustainX) * 0.3f;
					float controlY = sustainY;
					
					// Quadratic Bezier curve calculation
					float u = 1.0f - progress;
					indicatorX = u * u * sustainX + 2 * u * progress * controlX + progress * progress * releaseX;
					indicatorY = u * u * sustainY + 2 * u * progress * controlY + progress * progress * releaseY;
				}
				break;
		}
		
			// Update trail points
		trailPoints[trailIndex] = TrailPoint(indicatorX, indicatorY, 1.0f);
		trailIndex = (trailIndex + 1) % maxTrailPoints;
		
		// Update trail point alphas (fade out over time)
		for (int i = 0; i < maxTrailPoints; ++i) {
			trailPoints[i].alpha *= 0.85f; // Fade factor
		}
		
		// Draw trail points
		for (int i = 0; i < maxTrailPoints; ++i) {
			if (trailPoints[i].alpha > 0.1f) {
				float size = 2.0f + 3.0f * trailPoints[i].alpha;
				g.setColour(Colours::cyan.withAlpha(trailPoints[i].alpha * 0.6f));
				g.fillEllipse(trailPoints[i].x - size/2, trailPoints[i].y - size/2, size, size);
			}
		}
		
		// Draw the phase indicator as a bright, animated circle
		float time = Time::getMillisecondCounterHiRes() * 0.003f;
		float pulseSize = 3.0f + 2.0f * sin(time * 2.0f); // Faster pulsing
		
		// Outer glow with gradient
		ColourGradient glow(Colours::cyan.withAlpha(0.4f), indicatorX, indicatorY,
			Colours::cyan.withAlpha(0.0f), indicatorX, indicatorY, false);
		glow.addColour(0.0, Colours::cyan.withAlpha(0.4f));
		glow.addColour(1.0, Colours::cyan.withAlpha(0.0f));
		g.setGradientFill(glow);
		g.fillEllipse(indicatorX - pulseSize - 4, indicatorY - pulseSize - 4, 
			(pulseSize + 4) * 2, (pulseSize + 4) * 2);
		
		// Main indicator circle
		g.setColour(Colours::cyan.withAlpha(0.9f));
		g.fillEllipse(indicatorX - pulseSize, indicatorY - pulseSize, 
			pulseSize * 2, pulseSize * 2);
		
		// Bright center
		g.setColour(Colours::white);
		g.fillEllipse(indicatorX - 2, indicatorY - 2, 4, 4);
		
		// Outline with slight transparency
		g.setColour(Colours::white.withAlpha(0.8f));
		g.drawEllipse(indicatorX - pulseSize, indicatorY - pulseSize, 
			pulseSize * 2, pulseSize * 2, 1.5f);
	} else {
		// Clear trail when envelope is idle
		for (int i = 0; i < maxTrailPoints; ++i) {
			trailPoints[i].alpha = 0.0f;
		}
	}

	if (isInsideAttackHandler) {
		g.setColour(Colours::yellow);
	}
	else {
		g.setColour(Colours::white);
	}

	g.drawRect(*attackHandler, 2.0f);

	if (isInsideDecayHandler) {
		g.setColour(Colours::yellow);
	}
	else {
		g.setColour(Colours::white);
	}

	g.drawRect(*decayHandler, 2.0f);

	if (isInsideSustainHandler) {
		g.setColour(Colours::yellow);
	}
	else {
		g.setColour(Colours::white);
	}

	g.drawRect(*sustainHandler, 2.0f);

	if (isInsideReleaseHandler) {
		g.setColour(Colours::yellow);
	}
	else {
		g.setColour(Colours::white);
	}

	g.drawRect(*releaseHandler, 2.0f);

	g.setColour(Colours::darkgrey.darker());
	g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 5, 2.0f);

    //[/UserPaint]
}

void GraphicalEnvelope::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GraphicalEnvelope::mouseDown(const MouseEvent& event) {

}

void GraphicalEnvelope::mouseUp(const MouseEvent& event) {

}

void GraphicalEnvelope::mouseDrag(const MouseEvent& event) {
	Point <float> pos = Point<float>(event.getPosition().x - 5, event.getPosition().y - 5);

	if (pos.x < 10)
		pos.x = 10;
	if (pos.x > getWidth() - 10)
		pos.x = getWidth() - 10;
	if (pos.y < 10)
		pos.y = 10;
	if (pos.y > getHeight() - 10)
		pos.y = getHeight() - 10;

	if (isInsideAttackHandler) {

		if (pos.y > 10)
			pos.y = 10;

		if (pos.x + 10 >= decayHandler->getPosition().x) {
			pos.x = decayHandler->getPosition().x - 10;
		}
		attackHandler->setPosition(pos);
	}
	if (isInsideDecayHandler) {
		if (pos.x + 10 >= sustainHandler->getPosition().x) {
			pos.x = sustainHandler->getPosition().x - 10;
		}
		if (pos.x - 10 <= attackHandler->getPosition().x) {
			pos.x = attackHandler->getPosition().x + 10;
		}

		decayHandler->setPosition(pos);
		sustainHandler->setPosition(sustainHandler->getPosition().x, decayHandler->getPosition().y);
	}
	if (isInsideSustainHandler) {

		if (pos.x + 10 >= releaseHandler->getPosition().x) {
			pos.x = releaseHandler->getPosition().x - 10;
		}
		if (pos.x - 10 <= decayHandler->getPosition().x) {
			pos.x = decayHandler->getPosition().x + 10;
		}

		sustainHandler->setPosition(pos);
		decayHandler->setPosition(decayHandler->getPosition().x, sustainHandler->getPosition().y);
	}
	if (isInsideReleaseHandler) {

		if (pos.x - 10 <= sustainHandler->getPosition().x) {
			pos.x = sustainHandler->getPosition().x + 10;
		}
		if (pos.y < getHeight() - 10)
			pos.y = getHeight() - 10;

		releaseHandler->setPosition(pos);
	}

	repaint();

	attack = 2.5 * (getWidth() / 100) * (attackHandler->getPosition().x  - 10)/ 100;
	decay = 2.5 * (getWidth() / 100) * abs(decayHandler->getPosition().x - attackHandler->getPosition().x - 10) / 100;
	sustain = ((getHeight() - sustainHandler->getPosition().y - 10) / getHeight()) * 1.2;
	release = 2.5 * (getWidth() / 100) * abs(sustainHandler->getPosition().x - releaseHandler->getPosition().x - 10) / 100 - 1.5;

	updateModel();

}

void GraphicalEnvelope::mouseMove(const MouseEvent& event) {

	Point <float> pos = Point<float>(event.getPosition().x, event.getPosition().y);
	isInsideAttackHandler = attackHandler->contains(pos);
	isInsideDecayHandler = decayHandler->contains(pos);
	isInsideSustainHandler = sustainHandler->contains(pos);
	isInsideReleaseHandler = releaseHandler->contains(pos);
	repaint();
}

void GraphicalEnvelope::updateModel() {


	if (getName() == "ampEnvelope") {
		model->ampAttack = attack;
		model->ampDecay = decay;
		model->ampSustain = sustain;
		model->ampRelease = release;
	}
	if (getName() == "filterEnvelope") {
		model->fltAttack = attack;
		model->fltDecay = decay;
		model->fltSustain = sustain;
		model->fltRelease = release;
	}
	if (getName() == "auxEnvelope") {
		model->auxAttack = attack;
		model->auxDecay = decay;
		model->auxSustain = sustain;
		model->auxRelease = release;
	}

	sendChangeMessage();

}

void GraphicalEnvelope::setAttack(float attack) {
	this->attack = attack;
	// Update attack handler position based on attack value
	float attackX = 10 + (attack / 2.5f) * (getWidth() - 20) * 0.25f;
	attackHandler->setPosition(attackX, 10);
}

void GraphicalEnvelope::setDecay(float decay) {
	this->decay = decay;
	// Update decay handler position based on decay value
	float decayX = attackHandler->getPosition().x + 10 + (decay / 5.0f) * (getWidth() - 20) * 0.25f;
	decayHandler->setPosition(decayX, sustainHandler->getPosition().y);
}

void GraphicalEnvelope::setSustain(float sustain) {
	this->sustain = sustain;
	// Update sustain level (Y position)
	float sustainY = 10 + (1.0f - sustain) * (getHeight() - 20);
	sustainHandler->setPosition(sustainHandler->getPosition().x, sustainY);
	decayHandler->setPosition(decayHandler->getPosition().x, sustainY);
}

void GraphicalEnvelope::setRelease(float release) {
	this->release = release;
	// Update release handler position based on release value
	float releaseX = sustainHandler->getPosition().x + 10 + (release / 2.5f) * (getWidth() - 20) * 0.25f;
	releaseHandler->setPosition(releaseX, getHeight() - 10);
}

float GraphicalEnvelope::getAttack() {
	return attack;
}

float GraphicalEnvelope::getDecay() {
	return decay;
}

float GraphicalEnvelope::getSustain() {
	return sustain;
}

float GraphicalEnvelope::getRelease() {
	return release;
}

void GraphicalEnvelope::setCurrentPhase(int phase) {
	currentPhase = phase;
}

void GraphicalEnvelope::setCurrentPhasePosition(float position) {
	currentPhasePosition = jlimit(0.0f, 1.0f, position);
}

void GraphicalEnvelope::setCurrentPhaseValue(float value) {
	currentPhaseValue = jlimit(0.0f, 1.0f, value);
}

void GraphicalEnvelope::updateHandlerPositions() {
	// Update all handler positions based on current envelope values
	setAttack(attack);
	setDecay(decay);
	setSustain(sustain);
	setRelease(release);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GraphicalEnvelope" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster" constructorParams="Model* model"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff616161"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

