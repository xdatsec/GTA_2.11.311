#include "../gui.h"
#include "../../main.h"

SplashScreen::SplashScreen()
{
	m_logo = new Image("menu", "menu_adjback");
	this->addChild(m_logo);

	m_progressBar = new ProgressBar(ImColor(0.0f, 0.0f, 0.0f),
		ImColor(190, 0, 0), ImColor(255, 100, 0),
		ImColor(255, 100, 0), ImColor(190, 0, 0));
	this->addChild(m_progressBar);
}

void SplashScreen::performLayout()
{
	m_logo->setFixedSize(UISettings::splashscreenLogoSize());
	m_logo->setPosition(UISettings::splashscreenLogoPos());

	m_progressBar->setFixedSize(UISettings::splashscreenPBarSize());
	m_progressBar->setPosition(UISettings::splashscreenPBarPos());
}

void SplashScreen::draw(ImGuiRenderer* renderer)
{
	// background
	renderer->drawRect(absolutePosition(), absolutePosition() + size(), ImColor(0.0f, 0.0f, 0.0f), true);
	// logo
	m_logo->draw(renderer);
	// progress bar
	m_progressBar->draw(renderer);
}