#include "UI.h"

//////////////////////////////////BUTTON//////////////////////////////////
Button::Button(const sf::Vector2f& pos, const sf::Texture& t)
{
	isVisible = true;
	isEnabled = true;
	sp.setTexture(t);
	sp.setPosition(pos);
}

bool Button::IsClicked(const sf::Vector2f& mousePos) const
{
	if (!isEnabled) return false;
	return sp.getGlobalBounds().contains(mousePos);
}

void Button::ButtonDraw(sf::RenderWindow& window)
{
	if (!isVisible) return;
	window.draw(sp);
}
//////////////////////////////////BUTTON//////////////////////////////////

//////////////////////////////////TIMER///////////////////////////////////
Timer::Timer(const sf::Vector2f& pos, const sf::Vector2f& offset, const sf::Texture& t, const float& time, const int& charSz, const sf::Font& ft) : time(time), pos(pos)
{
	sp.setTexture(t);
	sp.setPosition(pos);
	timeTxt.setCharacterSize(charSz);
	timeTxt.setFont(ft);
	timeTxt.setPosition(pos + offset);
	timeTxt.setString(IntToTimer());
}


std::string Timer::IntToTimer() const
{
	if (time > 10) return ((int)time / 60 < 10 ? " " : "") + std::to_string((int)time / 60) + ":" + ((int)time % 60 < 10 ? "0" : "") + std::to_string((int)time % 60);
	else return "0:" + std::to_string((int)(time)) + "." + std::to_string((int)(time * 10) - (int)(time) * 10);
}

bool Timer::TimerSub(const float& t)
{
	time -= t;
	if (time <= 0) return false;
	timeTxt.setString(IntToTimer());
	return true;
}

void Timer::SetTime(const float& tm)
{
	time = tm;
	std::string buff = IntToTimer();
	timeTxt.setString(buff);
}

void Timer::SetPosition(const sf::Vector2f& pos)
{
	sp.setPosition(pos);
	timeTxt.setPosition(sf::Vector2f(pos.x + 15, pos.y));
}

void Timer::TimerDraw(sf::RenderWindow& window)
{
	if (!isVisible) return;
	window.draw(sp);
	window.draw(timeTxt);
}
//////////////////////////////////TIMER///////////////////////////////////

////////////////////////////////TEXT BOX//////////////////////////////////
TextBox::TextBox(const sf::Vector2f &pos, const sf::Vector2f& offset, const sf::Texture& t, const std::string txt, const int& charSz, const sf::Font &ft) : input_text(txt)
{
	textInput = sf::Text(txt, ft, charSz);
	sp.setTexture(t);
	sp.setPosition(pos);
	textInput.setPosition(pos + offset);
}

void TextBox::SetSelected(const bool& buff)
{
	isSelected = buff;
}

bool TextBox::IsClicked(const sf::Vector2f& mousePos) const
{
	if (!isEnabled) return false;
	return sp.getGlobalBounds().contains(mousePos);
}

std::string TextBox::Input(const sf::Event &e)
{
	if (!isSelected) return "";
	if (e.type == sf::Event::TextEntered) 
	{
		if (std::isprint(e.text.unicode)) input_text += e.text.unicode;
	}
	else if (e.type == sf::Event::KeyPressed) 
	{
		if (e.key.code == sf::Keyboard::BackSpace) 
		{
			if (!input_text.empty()) input_text.pop_back();
		}
	}
	textInput.setString(input_text);
	return "";
}

void TextBox::TextBoxDraw(sf::RenderWindow& window)
{
	if (!isVisible) return;
	window.draw(sp);
	window.draw(textInput);
}
////////////////////////////////TEXT BOX//////////////////////////////////
// 
/////////////////////////////IN MAIN MENU UI//////////////////////////////
void MainMenuUI::MainMenuUIDraw(sf::RenderWindow& window)
{
	for (int i = 0; i < sizeof(sp) / sizeof(sf::Sprite); i++) if(spVisible[i]) window.draw(sp[i]);
	for (int i = 0; i < sizeof(btn) / sizeof(Button); i++) btn[i].ButtonDraw(window);
	for (int i = 0; i < sizeof(txt) / sizeof(sf::Text); i++) if(txtVisible[i]) window.draw(txt[i]);
	txtBox.TextBoxDraw(window);
}
/////////////////////////////IN MAIN MENU UI//////////////////////////////

////////////////////////////////IN GAME UI////////////////////////////////
void GameUI::GameUIDraw(sf::RenderWindow& window)
{
	for (int i = 0; i < sizeof(sp) / sizeof(sf::Sprite); i++) window.draw(sp[i]);
	for (int i = 0; i < sizeof(tm) / sizeof(Timer); i++) tm[i].TimerDraw(window);
	for (int i = 0; i < sizeof(btn) / sizeof(Button); i++) btn[i].ButtonDraw(window);
	window.draw(winner);
}
////////////////////////////////IN GAME UI////////////////////////////////