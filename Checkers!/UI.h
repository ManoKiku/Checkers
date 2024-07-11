#include <iostream>
#include <SFML/Graphics.hpp>
#pragma once

class ui
{
public:
	bool isVisible = true, isEnabled = true;

	//Method for set enable
	void SetEnable(const bool& buff) { isEnabled = buff; }

	//Method for set visible
	void SetVisible(const bool& buff) { isVisible = buff; }
};

class Button : public ui
{
public:
	sf::Sprite sp;
	bool isChecked = 0;
	//Constructors for button :L
	Button() = default;
	Button(const sf::Vector2f& pos, const sf::Texture& t);

	//Checking for button clicks
	bool IsClicked(const sf::Vector2f& mousePos) const;

	//Draw method for Button
	void ButtonDraw(sf::RenderWindow& window);
};

class Timer : public ui
{
private:
	float time = 0;

	sf::Sprite sp;
	sf::Text timeTxt;
	sf::Vector2f pos;

public:
	//Constructors for timer :V
	Timer() = default;
	Timer(const sf::Vector2f& pos, const sf::Vector2f& offset, const sf::Texture& t, const float& time, const int& charSz, const sf::Font& ft);

	//Displaying time on text
	std::string IntToTimer() const;

	//Timer substraction
	bool TimerSub(const float& t);

	//Set position for a timer
	void SetPosition(const sf::Vector2f& pos);

	//Method for set new time
	void SetTime(const float& tm);

	//Method for getting position of a timer
	sf::Vector2f GetPosition() const { return sp.getPosition(); }

	//Method for getting a time
	float GetTime() const { return time; }

	//Draw method for Timer
	void TimerDraw(sf::RenderWindow& window);
};

class TextBox : public ui
{
private:
	sf::Sprite sp;
	std::string input_text;
	bool isSelected = false;

public:
	sf::Text textInput;

	//Constructor for Text Box :3
	TextBox() = default;
	TextBox(const sf::Vector2f& pos, const sf::Vector2f& offset, const sf::Texture& t, const std::string txt, const int& charSz, const sf::Font &ft);

	bool IsClicked(const sf::Vector2f& mousePos) const;
	
	void TextClear() { textInput.setString(" "); input_text = ""; }

	std::string Input(const sf::Event& e);

	void SetSelected(const bool& buff);

	void TextBoxDraw(sf::RenderWindow& window);
};

class MainMenuUI
{
public:
	Button btn[18];
	sf::Sprite sp[3];
	sf::Text txt[2];
	bool spVisible[3] = {1, 1, 0};
	bool txtVisible[2] = {0, 0};

	TextBox txtBox;

	//Constructor for In Game UI :O
	MainMenuUI();

	//Draw method for In Main Menu UI
	void MainMenuUIDraw(sf::RenderWindow& window);
};

class GameUI
{
public:
	sf::Sprite sp[1];
	Timer tm[2];
	Button btn[2];
	sf::Text winner;

	//Constructor for In Game UI >:)
	GameUI();

	//Draw method for In Game UI
	void GameUIDraw(sf::RenderWindow& window);
};