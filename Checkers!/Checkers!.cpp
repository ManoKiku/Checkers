#include <iostream>
#include <ctime>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include "CheckersBoard.h"
#include "UI.h"

sf::Texture t[54];
sf::Font font[2];
int theme = 0, sprites = 27;
sf::Color colors[2] = { sf::Color(124, 90, 74), sf::Color(255, 255, 255) };

enum GameType { COMPUTER, OFFLINE, ONLINE_C, ONLINE_H };

//Function for changing value coordinates in characters coordinates
std::string ToCheckersNote(sf::Vector2f p)
{
	std::string s = "";
	s += char(p.x + 97);
	s += char(56 - p.y);
	return s;
}

//Function for changing characters coordinates in value coordinates
sf::Vector2f ToCoords(const char& a, const char& b)
{
	return sf::Vector2f(int(a) - 97, 56 - int(b));
}

MainMenuUI::MainMenuUI()
{
	//BG
	sp[0] = sf::Sprite(t[0]);
	sp[1] = sf::Sprite(t[1]);
	sp[1].setPosition(sf::Vector2f(107, 29));

	//MAIN FORM

	//Play
	btn[0] = Button(sf::Vector2f(181, 148), t[2]);

	//Settings
	btn[1] = Button(sf::Vector2f(181, 282), t[3]);

	//Exit
	btn[2] = Button(sf::Vector2f(181, 416), t[4]);
	//MAIN FORM

	//SETTINGS
	btn[6] = Button(sf::Vector2f(181, 148), t[17]);

	btn[7] = Button(sf::Vector2f(283, 148), t[18]);

	btn[8] = Button(sf::Vector2f(181, 282), t[19]);

	btn[9] = Button(sf::Vector2f(561, 29), t[15]);

	sp[2] = sf::Sprite(t[20]);
	sp[2].setPosition(sf::Vector2f(385, 148));

	std::ifstream file("info.txt");
	int g, w, d, l;
	file >> g >> w >> d >> l;
	txt[0].setPosition(sf::Vector2f(87, 570));
	txt[0].setString("Games: " + std::to_string(g) + "\t Win: " + std::to_string(w) + "\t Draw: " + std::to_string(d) + "\t Lose: " + std::to_string(l));
	txt[0].setFont(font[0]);
	txt[0].setCharacterSize(29);
	txt[0].setFillColor(colors[theme]);
	file.close();
	//SETTINGS

	//PLAY FORM
	//Play with computer
	btn[3] = Button(sf::Vector2f(181, 148), t[12]);

	//Play offline
	btn[4] = Button(sf::Vector2f(181, 282), t[13]);

	//Back button
	btn[5] = Button(sf::Vector2f(561, 29), t[15]);
	//PLAY FORM
	
	//TYPE FORM
	btn[10] = Button(sf::Vector2f(181, 148), t[21]);
	btn[11] = Button(sf::Vector2f(283, 148), t[22]);
	btn[11].isChecked = true;
	btn[12] = Button(sf::Vector2f(385, 148), t[23]);

	btn[13] = Button(sf::Vector2f(181, 282), t[24]);
	btn[14] = Button(sf::Vector2f(283, 282), t[25]);
	btn[14].isChecked = true;
	btn[15] = Button(sf::Vector2f(385, 282), t[26]);

	btn[16] = Button(sf::Vector2f(181, 416), t[14]);

	btn[17] = Button(sf::Vector2f(561, 29), t[15]);
	//TYPE FORM
}

GameUI::GameUI()
{
	//BG
	sp[0].setTexture(t[7 +sprites * theme]);

	//Black pieces timer
	tm[0] = Timer(sf::Vector2f(73, 0), sf::Vector2f(15, 0), t[8 + sprites * theme], 600.9f, 56, font[1]);

	//White pieces timer
	tm[1] = Timer(sf::Vector2f(73, 577), sf::Vector2f(15, 0), t[8 + sprites * theme], 600.9f, 56, font[1]);

	//Pause button
	btn[0] = Button(sf::Vector2f(504, 0), t[9 + sprites * theme]);

	//Reverse Button
	btn[1] = Button(sf::Vector2f(433, 0), t[11 + sprites * theme]);

	//Winner text
	winner.setFont(font[0]);
	winner.setCharacterSize(20);
	winner.setPosition(sf::Vector2f(280, 577));
	winner.setString("");
}

void InAfterGame(sf::RenderWindow& window, MainMenuUI& ui, char whoWins, sf::Sound& sound, char color);
char InGame(sf::RenderWindow& window, GameType playType, char timerType = NULL, char pieceColor = 'w');

void Button0Click(MainMenuUI& ui)
{
	for (int i = 0; i < 3; i++) { ui.btn[i].SetEnable(false); ui.btn[i].SetVisible(false); }
	for (int i = 3; i < 6; i++) { ui.btn[i].SetEnable(true); ui.btn[i].SetVisible(true); }
	std::cout << "Call play btw" << std::endl;
}

void Button1Click(MainMenuUI& ui)
{
	for (int i = 6; i < 10; i++) { ui.btn[i].SetEnable(true); ui.btn[i].SetVisible(true); }
	for (int i = 0; i < 3; i++) { ui.btn[i].SetEnable(false); ui.btn[i].SetVisible(false); }
	ui.spVisible[2] = true;
	ui.txtVisible[0] = true;
	ui.txt[0].setPosition(sf::Vector2f((650 - ui.txt[0].getGlobalBounds().width) / 2, 570));
	std::cout << "Call settings btw" << std::endl;

}

void Button3Click(MainMenuUI& ui)
{
	for (int i = 10; i < 18; i++) { ui.btn[i].SetEnable(true); ui.btn[i].SetVisible(true); }
	for (int i = 3; i < 6; i++) { ui.btn[i].SetEnable(false); ui.btn[i].SetVisible(false); }
	std::cout << "Call play with pc btw" << std::endl;
}

void Button4Click(MainMenuUI& ui)
{
	for (int i = 13; i < 18; i++) { ui.btn[i].SetEnable(true); ui.btn[i].SetVisible(true); }
	for (int i = 3; i < 6; i++) { ui.btn[i].SetEnable(false); ui.btn[i].SetVisible(false); }
	std::cout << "Call play offline btw" << std::endl;
}

void Button5Click(MainMenuUI& ui)
{
	for (int i = 0; i < 3; i++) { ui.btn[i].SetEnable(true); ui.btn[i].SetVisible(true); }
	for (int i = 3; i < 6; i++) { ui.btn[i].SetEnable(false); ui.btn[i].SetVisible(false); }
}

void Button6Click(sf::Music &music)
{
	int volume = music.getVolume();
	volume += 5;
	volume = volume > 100 ? 100 : volume;
	volume = volume < 0 ? 0 : volume;
	music.setVolume(volume);
}

void Button7Click(sf::Music& music)
{
	int volume = music.getVolume();
	volume -= 5;
	volume = volume > 100 ? 100 : volume;
	volume = volume < 0 ? 0 : volume;
	music.setVolume(volume);
}

void Button8Click(MainMenuUI& ui)
{
	++theme;
	theme = theme > 1 ? 0 : theme;
	ui.sp[0].setTexture(t[0 + sprites * theme]);
	ui.sp[1].setTexture(t[1 + sprites * theme]);
	ui.btn[0].sp.setTexture(t[2 + sprites * theme]);
	ui.btn[1].sp.setTexture(t[3 + sprites * theme]);
	ui.btn[2].sp.setTexture(t[4 + sprites * theme]);
	ui.btn[6].sp.setTexture(t[17 + sprites * theme]);
	ui.btn[7].sp.setTexture(t[18 + sprites * theme]);
	ui.btn[8].sp.setTexture(t[19 + sprites * theme]);
	ui.btn[9].sp.setTexture(t[15 + sprites * theme]);
	ui.sp[2].setTexture(t[20 + sprites * theme]);
	ui.btn[3].sp.setTexture(t[12 + sprites * theme]);
	ui.btn[4].sp.setTexture(t[13 + sprites * theme]);
	ui.btn[5].sp.setTexture(t[15 + sprites * theme]);
	ui.btn[10].sp.setTexture(t[21 + sprites * theme]);
	ui.btn[11].sp.setTexture(t[22 + sprites * theme]);
	ui.btn[12].sp.setTexture(t[23 + sprites * theme]);
	ui.btn[13].sp.setTexture(t[24 + sprites * theme]);
	ui.btn[14].sp.setTexture(t[25 + sprites * theme]);
	ui.btn[15].sp.setTexture(t[26 + sprites * theme]);
	ui.btn[16].sp.setTexture(t[14 + sprites * theme]);
	ui.btn[17].sp.setTexture(t[15 + sprites * theme]);
	ui.txt[0].setFillColor(colors[theme]);
}

void Button9Click(MainMenuUI& ui)
{
	for (int i = 0; i < 3; i++) { ui.btn[i].SetEnable(true); ui.btn[i].SetVisible(true); }
	for (int i = 6; i < 10; i++) { ui.btn[i].SetEnable(false); ui.btn[i].SetVisible(false); }
	ui.spVisible[2] = false;
	ui.txtVisible[0] = false;
}

void Button10Click(MainMenuUI& ui)
{
	ui.btn[10].isChecked = true;
	ui.btn[11].isChecked = false;
	ui.btn[12].isChecked = false;
}

void Button11Click(MainMenuUI& ui)
{
	ui.btn[10].isChecked = false;
	ui.btn[11].isChecked = true;
	ui.btn[12].isChecked = false;
}

void Button12Click(MainMenuUI& ui)
{
	ui.btn[10].isChecked = false;
	ui.btn[11].isChecked = false;
	ui.btn[12].isChecked = true;
}

void Button13Click(MainMenuUI& ui)
{
	ui.btn[13].isChecked = true;
	ui.btn[14].isChecked = false;
	ui.btn[15].isChecked = false;
}

void Button14Click(MainMenuUI& ui)
{
	ui.btn[13].isChecked = false;
	ui.btn[14].isChecked = true;
	ui.btn[15].isChecked = false;
}

void Button15Click(MainMenuUI& ui)
{
	ui.btn[13].isChecked = false;
	ui.btn[14].isChecked = false;
	ui.btn[15].isChecked = true;
}

void Button16Click(sf::RenderWindow& window, MainMenuUI& ui, sf::Sound &sound, char color, char timer, bool isPc)
{
	char pieceColor = (color == 'a' ? (rand() % 2 == 0 ? 'b' : 'w') : color);
	std::cout << color << " " << pieceColor << std::endl;
	InAfterGame(window, ui, InGame(window, isPc ? COMPUTER : OFFLINE, timer, isPc ? pieceColor : 'w'), sound, isPc ? pieceColor : 'o');
}

void Button17Click(MainMenuUI& ui)
{
	for (int i = 3; i < 6; i++) { ui.btn[i].SetEnable(true); ui.btn[i].SetVisible(true); }
	for (int i = 10; i < 18; i++) { ui.btn[i].SetEnable(false); ui.btn[i].SetVisible(false); }
	std::cout << "Call bck1" << std::endl;
}

void ButtonReverseClick(GameUI &ui, CheckersBoard &br)
{
	sf::Vector2f buff = ui.tm[0].GetPosition();
	ui.tm[0].SetPosition(ui.tm[1].GetPosition());
	ui.tm[1].SetPosition(buff);
	br.ReverseBoard();
}

CheckersBoard::MoveType ComputerMove(GameUI& ui, CheckersBoard& br, bool isYourTurn)
{
	float sizeT = br.GetSizeT();
	int pieceControl = isYourTurn ? 0 : 12, curPiece;
	sf::Vector2f op, np;
	CheckersBoard::MoveType botMove;
	do
	{
		int mulX, mulY, size;
		do
		{
			curPiece = rand() % (12 + pieceControl);
		} while (br.figures[curPiece].isDead);
		op = br.figures[curPiece].position;
		if (br.figures[curPiece].isQueen)
		{
			size = rand() % 8;
		}
		else
		{
			size = rand() % 2 + 1;
		}
		mulX = rand() % 2 == 0 ? -1 : 1;
		mulY = rand() % 2 == 0 ? -1 : 1;
		np = sf::Vector2f(op.x + mulX * size, op.y + mulY * size);
		botMove = br.MandatoryMove(curPiece, op, np);
	} while (botMove == CheckersBoard::CANT_MOVE);
	br.figures[curPiece].f.setPosition((!br.GetIsReversed() ? np : sf::Vector2f(7, 7) - np) * sizeT);
	br.BoardPrint();
	return botMove;
}

void InMainMenu(sf::RenderWindow& window, sf::Sound& sound)
{
	MainMenuUI ui;
	sf::SoundBuffer choose;
	sf::Music music;
	music.setVolume(100);
	int btn_amount = 0, cur_btn = -1;
	bool isPc = false;

	choose.loadFromFile("vfx/choose.wav");
	music.openFromFile("vfx/music.ogg");
	music.play();
	sound.setBuffer(choose);

	btn_amount = sizeof(ui.btn) / sizeof(Button);
	for (int i = 3; i < btn_amount; i++) { ui.btn[i].SetEnable(false); ui.btn[i].SetVisible(false); }
	ui.txtBox.SetEnable(false);
	ui.txtBox.SetVisible(false);

	while (window.isOpen())
	{
		sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
		sf::Event e;
		for (int i = 0; i < btn_amount; i++)
		{
			if (ui.btn[i].sp.getGlobalBounds().contains(mousePos) || ui.btn[i].isChecked) ui.btn[i].sp.setColor(sf::Color(230, 230, 230, 255));
			else ui.btn[i].sp.setColor(sf::Color(255, 255, 255, 255));
		}

		while (window.pollEvent(e))
		{
			if (ui.txtBox.Input(e) != "")
			{
			}

			if (e.type == sf::Event::Closed)
			{
				window.close();
				exit(0);
			}

			if (e.type == sf::Event::MouseButtonPressed)
			{
				if (e.key.code == sf::Mouse::Left)
				{
					for (int i = 0; i < btn_amount; i++)
						if (ui.btn[i].IsClicked(mousePos) && !ui.btn[i].isChecked)
						{
							cur_btn = i; break;
						}
						

					ui.txtBox.SetSelected(ui.txtBox.IsClicked(mousePos));
				}

			}
			else if (e.type == sf::Event::MouseButtonReleased)
			{
				bool isThisButton = false;
				for (int i = 0; i < btn_amount; i++)
					if (ui.btn[i].IsClicked(mousePos) && cur_btn == i) { sound.play(); isThisButton = true; break; }

				if (!isThisButton) break;
				char color = ui.btn[10].isChecked ? 'w' : (ui.btn[11].isChecked ? 'a' : 'b');
				char timer = ui.btn[13].isChecked ? 'r' : (ui.btn[14].isChecked ? 'b' : 's');
				switch (cur_btn)
				{
				case 0:
					Button0Click(ui);
					break;
				case 1:
					Button1Click(ui);
					break;
				case 2:
					exit(0);
					break;
				case 3:
					Button3Click(ui);
					isPc = true;
					break;
				case 4:
					Button4Click(ui);
					isPc = false;
					break;
				case 5:
					Button5Click(ui);
					break;
				case 6:
					Button6Click(music);
					break;
				case 7:
					Button7Click(music);
					break;
				case 8:
					Button8Click(ui);
					break;
				case 9:
					Button9Click(ui);
					break;
				case 10:
					Button10Click(ui);
					break;
				case 11:
					Button11Click(ui);
					break;
				case 12:
					Button12Click(ui);
					break;
				case 13:
					Button13Click(ui);
					break;
				case 14:
					Button14Click(ui);
					break;
				case 15:
					Button15Click(ui);
					break;
				case 16:
					Button16Click(window, ui, sound, color, timer, isPc);
					break;
				case 17:
					Button17Click(ui);
					break;
				default:
					break;
				}
				cur_btn = -1;
			}
		}

		window.clear();
		ui.MainMenuUIDraw(window);
		window.display();
	}
}

void InAfterGame(sf::RenderWindow& window, MainMenuUI& ui, char whoWins, sf::Sound& sound, char color)
{
	Button btnAg(sf::Vector2f(561, 29), t[15 +sprites * theme]);
	sf::Text txtAg((whoWins != 'd' ? (whoWins == 'w' ? "WHITE  WIN THIS GAME" : "BLACK  WIN THIS GAME") : "THE GAME ENDED IN A DRAW"), font[0], 40);
	txtAg.setPosition(sf::Vector2f((650 - txtAg.getGlobalBounds().width) / 2, 148));
	txtAg.setFillColor(theme == 0 ? sf::Color(63, 23, 2, 255) : colors[theme]);
	

	if (color != 'o')
	{
		std::ifstream filei("info.txt");
		int g, w, d, l;
		filei >> g >> w >> d >> l;
		++g;
		if (whoWins == color) ++w;
		else if (whoWins == 'd') ++d;
		else ++l;
		filei.close();

		std::ofstream fileo("info.txt");
		fileo << g <<std::endl << w << std::endl << d << std::endl << l;
		fileo.close();
		ui.txt[0].setPosition(sf::Vector2f(87, 570));
		ui.txt[0].setString("Games: " + std::to_string(g) + "\t Win: " + std::to_string(w) + "\t Draw: " + std::to_string(d) + "\t Lose: " + std::to_string(l));
	}


	bool isClicked = false;

	while (window.isOpen())
	{
		sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
		sf::Event e;
		if (btnAg.sp.getGlobalBounds().contains(mousePos)) btnAg.sp.setColor(sf::Color(230, 230, 230, 255));
		else btnAg.sp.setColor(sf::Color(255, 255, 255, 255));

		if (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
				exit(0);
			}
			if (e.type == sf::Event::MouseButtonPressed)
			{
				if (e.key.code == sf::Mouse::Left)
				{
					isClicked = btnAg.IsClicked(mousePos);
				}

			}
			else if (e.type == sf::Event::MouseButtonReleased)
			{
				if (btnAg.IsClicked(mousePos) && isClicked)
				{
					sound.play();
					return;
				}
			}
		}

		window.clear();
		window.draw(ui.sp[0]);
		window.draw(ui.sp[1]);
		window.draw(txtAg);
		btnAg.ButtonDraw(window);
		window.display();
	}
}

char InGame(sf::RenderWindow& window, GameType playType, char timerType, char pieceColor)
{
	sf::SoundBuffer move;
	sf::SoundBuffer choose;

	move.loadFromFile("vfx/move.wav");
	choose.loadFromFile("vfx/choose.wav");

	sf::Sound sound[2];
	sound[0].setBuffer(move);
	sound[1].setBuffer(choose);

	CheckersBoard br(sf::Vector2f(28, 28), sf::Vector2f(73, 73), 56, t[5 + +sprites * theme], t[6 + +sprites * theme]);
	CheckersBoard::Winner win = CheckersBoard::NO_ONE;
	GameUI ui;

	bool isMove = false, isButtonPressed = false, isYourTurn = true;
	float dx = 0, dy = 0;
	sf::Vector2f oldPos, newPos;
	std::string str;

	
	int n = 0, cur_btn = -1;
	float  waitTime = 0, timer = 0;
	int btn_amount = sizeof(ui.btn) / sizeof(Button);

	isYourTurn = pieceColor == 'w' ? true : false;
	
	if (!isYourTurn)
	{
		sf::Vector2f buff = ui.tm[0].GetPosition();
		ui.tm[0].SetPosition(ui.tm[1].GetPosition());
		ui.tm[1].SetPosition(buff);
		br.ReverseBoard();
	}
	
	int gameTime = 600;

	if (timerType == 'b') gameTime = 300;
	else if (timerType == 's') gameTime = 120;
	ui.tm[0].SetTime(gameTime);
	ui.tm[1].SetTime(gameTime);

	sf::Clock clock, tmClock;
	while (window.isOpen())
	{
		sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
		sf::Vector2i pos = sf::Vector2i(mousePos) - sf::Vector2i(br.GetFullOffset());
		sf::Event e;

		for (int i = 0; i < btn_amount; i++)
		{
			if (ui.btn[i].sp.getGlobalBounds().contains(mousePos)) ui.btn[i].sp.setColor(sf::Color(230, 230, 230, 255));
			else ui.btn[i].sp.setColor(sf::Color(255, 255, 255, 255));
		}

		if (win == CheckersBoard::NO_ONE)
		{
			if (br.GetIsWhiteMove())
			{
				if (!ui.tm[1].TimerSub(clock.getElapsedTime().asSeconds()))
				{
					win = CheckersBoard::BLACK;
					std::cout << "Black is win!" << std::endl;
					ui.winner.setString("BLACK WINS!");
					isMove = false;
					br.figures[n].f.setPosition(br.figures[n].position * br.GetSizeT());
				}
			}
			else if (!ui.tm[0].TimerSub(clock.getElapsedTime().asSeconds()))
			{
				win = CheckersBoard::WHITE;
				std::cout << "White is win!" << std::endl;
				ui.winner.setString("WHITE WINS!");
				isMove = false;
				br.figures[n].f.setPosition(br.figures[n].position * br.GetSizeT());
			}
		}

		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed) { window.close(); }


			if (e.type == sf::Event::MouseButtonPressed)
			{
				if (e.key.code == sf::Mouse::Left)
				{
					for (int i = 0; i < btn_amount; i++)
						if (ui.btn[i].IsClicked(mousePos)) { cur_btn = i; break; }

					if (br.IsClicked(mousePos) && ((isYourTurn == br.GetIsWhiteMove() && playType == COMPUTER) || (playType == OFFLINE)) && win == CheckersBoard::NO_ONE)
					{
						int pieceControl = br.GetIsWhiteMove() ? 12 : 0;

						for (int i = pieceControl; i < 12 + pieceControl; i++)
						{
							if (br.figures[i].f.getGlobalBounds().contains(pos.x, pos.y))
							{
								isMove = true; n = i;
								dx = pos.x - br.figures[i].f.getPosition().x;
								dy = pos.y - br.figures[i].f.getPosition().y;
								oldPos = br.figures[i].f.getPosition();
								break;
							}
						}
					}
				}
			}

			if (e.type == sf::Event::MouseButtonReleased)
			{
				if (e.key.code == sf::Mouse::Left)
				{
					bool isThisButton = false;
					for (int i = 0; i < btn_amount; i++)
						if (ui.btn[i].IsClicked(mousePos) && cur_btn == i) { sound[1].play(); isThisButton = true; break; }

					if (cur_btn != -1 && isThisButton)
					{
						switch (cur_btn)
						{
						case 0:
							std::cout << "Call menu btw" << std::endl;
							if (playType == COMPUTER)
							{
								return win == CheckersBoard::NO_ONE ? (isYourTurn ? 'b' : 'w') : (win == CheckersBoard::DRAW ? 'd' : (win == CheckersBoard::WHITE ? 'w' : 'b'));
							}
							else
							{	
								return win == CheckersBoard::NO_ONE || win == CheckersBoard::DRAW ? 'd' : (win == CheckersBoard::WHITE ? 'w' : 'b');
							}
							break;
						case 1:
							ButtonReverseClick(ui, br);
							break;
						default:
							break;
						}
						cur_btn = -1;
					}
					else if (isMove && win == CheckersBoard::NO_ONE)
					{
						float sizeT = br.GetSizeT();
						sf::Vector2f p = br.figures[n].f.getPosition() + sf::Vector2f(sizeT / 2, sizeT / 2);
						newPos = sf::Vector2f(sizeT * int(p.x / sizeT), sizeT * int(p.y / sizeT));
						isMove = false;

						newPos = !br.GetIsReversed() ? newPos : sf::Vector2f(7, 7) * sizeT - newPos;
						oldPos = !br.GetIsReversed() ? oldPos : sf::Vector2f(7, 7) * sizeT - oldPos;
						CheckersBoard::MoveType moveType = br.MandatoryMove(n, oldPos / sizeT, newPos / sizeT);
						if (moveType != CheckersBoard::CANT_MOVE)
						{
							br.figures[n].f.setPosition(!br.GetIsReversed() ? newPos : sf::Vector2f(7, 7) * sizeT - newPos);
							str = ToCheckersNote((oldPos / sizeT)) + ToCheckersNote((newPos / sizeT));
							sound[0].play();
							waitTime = rand() % 5 + 3;
							std::cout << waitTime << std::endl;
							timer = 0;
							win = br.IsWinner();
							if (win == CheckersBoard::BLACK) { ui.winner.setString("BLACK WINS!"); }
							else if (win == CheckersBoard::WHITE) { ui.winner.setString("WHITE WINS!"); }
							else if (win == CheckersBoard::DRAW) { ui.winner.setString("END IN DRAW"); }
						}
						else br.figures[n].f.setPosition((!br.GetIsReversed() ? sf::Vector2f(br.figures[n].position) : sf::Vector2f(7, 7) - sf::Vector2f(br.figures[n].position)) * sizeT);
						br.BoardPrint();
					}
				}
			}
		}
		if (playType == COMPUTER && win == CheckersBoard::NO_ONE)
		{
			timer += clock.getElapsedTime().asSeconds();
			if (timer >= waitTime && !(isYourTurn == br.GetIsWhiteMove()))
			{
				CheckersBoard::MoveType botMove = ComputerMove(ui, br, isYourTurn);
				timer = 0;
				sound[0].play();
				sound[0].play();
				win = br.IsWinner();
				if (win == CheckersBoard::BLACK) { ui.winner.setString("BLACK WINS!"); }
				else if (win == CheckersBoard::WHITE) { ui.winner.setString("WHITE WINS!"); }
				else if (win == CheckersBoard::DRAW) { ui.winner.setString("END IN DRAW"); }
				if (botMove == CheckersBoard::DOUBLE)
				{
					waitTime = rand() % 3 + 5;
				}
			}
		}
		

		if (isMove)
		{
			int boardSize = 7 * br.GetSizeT();
			br.figures[n].f.setPosition(pos.x - dx > boardSize ? boardSize : pos.x - dx < 0 ? 0 : pos.x - dx, pos.y - dy > boardSize ? boardSize : pos.y - dy < 0 ? 0 : pos.y - dy);
		}
		clock.restart();

		////// draw  ///////
		window.clear();
		ui.GameUIDraw(window);
		br.BoardDraw(window, n);
		window.display();
	}
}

void TextureLoad()
{
	for (int i = 0; i < 2; i++)
	{
		t[0 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/mainmenubg.png");
		t[1 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/mainform.png");
		t[2 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/playbtn.png");
		t[3 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/settingsbtn.png");
		t[4 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/exitbtn.png");
		t[5 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/figures.png");
		t[6 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/board.png");
		t[7 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/bg.png");
		t[8 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/timer.png");
		t[9 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/pause.png");
		t[11 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/reverse.png");
		t[12 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/withpcbtn.png");
		t[13 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/offlinebtn.png");
		t[14 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/ingamebtn.png");
		t[15 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/backbtn.png");
		t[16 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/input.png");
		t[17 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/plusbtn.png");
		t[18 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/minusbtn.png");
		t[19 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/themebtn.png");
		t[20 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/volume.png");
		t[21 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/whitebtn.png");
		t[22 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/anybtn.png");
		t[23 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/blackbtn.png");
		t[24 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/rapidbtn.png");
		t[25 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/blitzbtn.png");
		t[26 + i * sprites].loadFromFile("images/style" + std::to_string(i) + "/bulletbtn.png");
	}
}

int main() 
{
	sf::RenderWindow window(sf::VideoMode(650, 650), "The Checkers!", sf::Style::Titlebar | sf::Style::Close);
	sf::Image icon;

	font[0].loadFromFile("fonts/BERNHC.ttf");
	font[1].loadFromFile("fonts/digital-7.ttf");

	//Textures load//
	TextureLoad();
	icon.loadFromFile("images/icon.png");
	//Textures load//

	srand(time(0));

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	sf::Sound sound;

	while (window.isOpen())
	{
		InMainMenu(window, sound);
	}

	return 0;
}
