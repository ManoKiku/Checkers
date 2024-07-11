#include <iostream>
#include <SFML/Graphics.hpp>
#pragma once

struct piece
{
	sf::Sprite f;
	sf::Vector2f position;
	bool isDead = false;
	bool isWhite = NULL;
	bool isQueen = false;
};

class CheckersBoard
{
private:
	char board[8][8] =
	{ ' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b',
	 'b', ' ', 'b', ' ', 'b', ' ', 'b', ' ',
	 ' ', 'b', ' ', 'b', ' ', 'b', ' ', 'b',
	 ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	 ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	 'w', ' ', 'w', ' ', 'w', ' ', 'w', ' ',
	 ' ', 'w', ' ', 'w', ' ', 'w', ' ', 'w',
	 'w', ' ', 'w', ' ', 'w', ' ', 'w', ' ' };

	float sizeT;
	sf::Vector2f figOffset;
	sf::Vector2f brOffset;
	sf::Sprite sBoard;
	bool isWhiteMove = true, isReversed = false;
	int moveSum = 0;

	void Move(const int& n, const sf::Vector2f& oldPos, const sf::Vector2f& newPos);
public:
	enum MoveType { CANT_MOVE, MOVE, ATTACK, DOUBLE };
	enum Winner { NO_ONE, DRAW, WHITE, BLACK };

	piece figures[24];

	//Constructor for Checkers Board :D
	CheckersBoard(const sf::Vector2f& figOffset, const sf::Vector2f& brOffset, float sizeT, const sf::Texture& t1, const sf::Texture& t2);

	//Method for drawing board in console(Only for debugging)
	void BoardPrint() const;

	//Possible moves of a specific piece
	MoveType OtherMoves(const int& n) const;

	//Method for checking the possibility of a SPECIFIC move, the possibility of a double strike and whether some pieces attack others
	MoveType MandatoryMove(const int& n, const sf::Vector2f& oldPos, const sf::Vector2f& newPos);

	//Method for checking the possibility of a move in general
	MoveType DoubleMove(const int& n, const sf::Vector2f& oldPos, const sf::Vector2f& newPos) const;

	//Method for reverse board
	void ReverseBoard();

	//Check for board click
	bool IsClicked(const sf::Vector2f& mousePos) const;

	//Method for getting information about the winner if there is one
	Winner IsWinner() const;

	//Method for getting offset of CheckersBoard (full offset)
	sf::Vector2f GetFullOffset() const { return figOffset + brOffset; }

	//Method for getting size of Figures
	float GetSizeT() const { return sizeT; }

	//Method for getting current turn of the move
	bool GetIsWhiteMove() const { return isWhiteMove; }

	//Method for getting information about board reverse
	bool GetIsReversed() const { return isReversed; };

	//Draw method for CheckersBoard
	void BoardDraw(sf::RenderWindow& window, const int& n);
};