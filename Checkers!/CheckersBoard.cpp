#include "CheckersBoard.h"

void CheckersBoard::Move(const int& n, const sf::Vector2f& oldPos, const sf::Vector2f& newPos)
{
	figures[n].position = newPos;
	board[(int)newPos.y][(int)newPos.x] = board[(int)oldPos.y][(int)oldPos.x];
	board[(int)oldPos.y][(int)oldPos.x] = ' ';
}

CheckersBoard::CheckersBoard(const sf::Vector2f& figOffset, const sf::Vector2f& brOffset, float sizeT, const sf::Texture& t1, const sf::Texture& t2)
	: figOffset(figOffset), brOffset(brOffset), sizeT(sizeT)
{
	sBoard.setTexture(t2);
	for (int i = 0; i < 24; i++)
	{
		figures[i].f.setTexture(t1);
		if (i < 12) figures[i].isWhite = false;
		else figures[i].isWhite = true;
	}

	int k = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			char n = board[i][j];
			if (n == ' ') continue;
			figures[k].position = sf::Vector2f(j, i);
			int y = n == 'b' ? 0 : 1;
			figures[k].f.setTextureRect(sf::IntRect(0, sizeT * y, sizeT, sizeT));
			figures[k].f.setPosition(sizeT * j, sizeT * i);
			k++;
		}
	}
}

void CheckersBoard::BoardPrint() const
{
	for (int i = 0; i < 8; i++)
	{
		std::cout << 8 - i << "| ";
		for (int j = 0; j < 8; j++)
		{
			std::cout << board[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "   ";
	for (int i = 0; i < 8; i++)
	{
		std::cout << "--";
	}
	std::cout << std::endl << "   ";
	for (int i = 0; i < 8; i++)
	{
		std::cout << char(97 + i) << " ";
	}
	std::cout << std::endl << std::endl;
}

CheckersBoard::MoveType CheckersBoard::OtherMoves(const int& n) const
{
	MoveType buffReturn = CANT_MOVE;
	int size = figures[n].isQueen ? 7 : 2;
	for (int i = -1; i <= 1; i += 2)
		for (int j = -1; j <= 1; j += 2)
			for (int pos = 1; pos <= size; pos++)
			{
				MoveType buff = DoubleMove(n, figures[n].position, sf::Vector2f(figures[n].position.x + i * pos, figures[n].position.y + j * pos));
				if (buff == ATTACK)
				{
					return ATTACK;
				}
				else if (buff == MOVE)
				{
					buffReturn = MOVE;
				}
			}
	return buffReturn;
}

CheckersBoard::MoveType CheckersBoard::MandatoryMove(const int& n, const sf::Vector2f& oldPos, const sf::Vector2f& newPos)
{
	int mulX, mulY, figuresFix, atcFig = -1;
	bool skip = false;

	if (figures[n].isWhite != isWhiteMove) return CANT_MOVE;
	figuresFix = figures[n].isWhite ? 12 : 0;

	mulX = newPos.x - oldPos.x > 0 ? 1 : -1;
	mulY = newPos.y - oldPos.y > 0 ? 1 : -1;

	if ((newPos.x == oldPos.x || newPos.y == oldPos.y) || (0 > newPos.x || newPos.x > 7 || 0 > newPos.y || newPos.y > 7))
	{
		return CANT_MOVE;
	}

	//If current piece can move and atack another piece, than we can skip next checking
	{
		MoveType temp = DoubleMove(n, oldPos, newPos);
		if (temp == ATTACK)
		{
			skip = true;
		}
		else if (temp == CANT_MOVE) return CANT_MOVE;
	}

	//Checking whether a piece beats another piece
	for (int fig = figuresFix; fig < 12 + figuresFix && !skip; fig++)
	{
		if (figures[fig].isDead) continue;
		if (OtherMoves(fig) == ATTACK) return CANT_MOVE;
	}
	//If not, than make a move
	Move(n, oldPos, newPos);

	if (figures[n].position.y == (figures[n].isWhite ? 0 : 7))
	{
		figures[n].isQueen = true;
		board[(int)figures[n].position.y][(int)figures[n].position.x] = figures[n].isWhite ? 'W' : 'B';
		figures[n].f.setTextureRect(sf::IntRect(0, sizeT * (figures[n].isWhite ? 3 : 2), sizeT, sizeT));
	}

	figuresFix = figures[n].isWhite ? 0 : 12;

	//Finding figure, that current piece atacking
	int moveSz = abs(newPos.y - oldPos.y);
	for (int i = figuresFix; i < figuresFix + 12; i++)
	{
		if (figures[n].isQueen)
		{
			bool leave = false;
			for (int j = 1; j < moveSz; j++)
				if (figures[i].position == sf::Vector2f(oldPos.x + mulX * j, oldPos.y + mulY * j))
				{
					atcFig = i;
					leave = true;
					break;
				}
			if (leave) break;
		}
		else
		{
			if (figures[i].position == sf::Vector2f(oldPos.x + mulX, oldPos.y + mulY))
			{
				atcFig = i;
				break;
			}
		}
	}

	//if current figure dont attacking another, than skip this and next construction
	if (atcFig != -1)
	{
		board[(int)figures[atcFig].position.y][(int)figures[atcFig].position.x] = ' ';
		figures[atcFig].position = sf::Vector2f(-10, -10);
		figures[atcFig].f.setPosition(figures[atcFig].position * sizeT);
		figures[atcFig].isDead = true;
	}

	//Checking the possibility of a double strike
	for (int i = -1; i <= 1 && atcFig != -1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			if (figures[n].isQueen)
			{
				if (-1 * mulX == i && -1 * mulY == j) continue;
				for (int pos = 2; pos < 7; pos++)
					if (DoubleMove(n, newPos, sf::Vector2f(newPos.x + i * pos, newPos.y + j * pos)) == ATTACK)
					{
						return DOUBLE;
					}
			}
			else if (DoubleMove(n, newPos, sf::Vector2f(newPos.x + i * 2, newPos.y + j * 2)) == ATTACK)
			{
				return DOUBLE;
			}
		}
	}
	isWhiteMove = !isWhiteMove;

	moveSum += 1;
	if (!figures[n].isQueen || atcFig != -1) moveSum = 0;
	return (atcFig == -1) ? MOVE : ATTACK;
}

CheckersBoard::MoveType CheckersBoard::DoubleMove(const int& n, const sf::Vector2f& oldPos, const sf::Vector2f& newPos) const
{

	int mulX, mulY;

	mulX = newPos.x - oldPos.x > 0 ? 1 : -1;
	mulY = newPos.y - oldPos.y > 0 ? 1 : -1;

	if ((newPos.x == oldPos.x || newPos.y == oldPos.y) || (0 > newPos.x || newPos.x > 7 || 0 > newPos.y || newPos.y > 7) || n == -1)
	{
		return CANT_MOVE;
	}

	if (board[(int)newPos.y][(int)newPos.x] != ' ') return CANT_MOVE;

	if (!figures[n].isQueen)
	{
		if (abs(newPos.x - oldPos.x) == 1)
		{
			if (newPos.y - oldPos.y == (figures[n].isWhite ? -1 : 1)) return MOVE;
			return CANT_MOVE;
		}
		else if (abs(newPos.x - oldPos.x) == 2 && abs(newPos.y - oldPos.y) == 2)
		{
			sf::Vector2f buff(oldPos.x + mulX, oldPos.y + mulY);
			for (int fig = 0; fig < 24; fig++)
			{
				if (figures[fig].position == buff)
				{
					if (figures[fig].isWhite != figures[n].isWhite)
					{
						return ATTACK;
					}
					else return CANT_MOVE;
				}
				else if (fig == 23) return CANT_MOVE;
			}
		}
		else return CANT_MOVE;
	}
	else
	{
		if (abs(newPos.x - oldPos.x) == abs(newPos.y - oldPos.y))
		{
			int size = abs(newPos.x - oldPos.x);
			bool seePiece = false;
			sf::Vector2f buff;
			for (int i = 1; i <= size; i++)
			{
				buff = sf::Vector2f(oldPos.x + i * mulX, oldPos.y + i * mulY);
				if (board[(int)buff.y][(int)buff.x] != ' ')
				{
					for (int fig = 0; fig < 24; fig++)
					{
						if (seePiece) return CANT_MOVE;
						if ((figures[fig].isWhite == figures[n].isWhite) && buff == figures[fig].position) return CANT_MOVE;
					}
					seePiece = true;
				}
			}
			if (seePiece) return ATTACK;
		}
		else return CANT_MOVE;
	}
	return MOVE;

}

void CheckersBoard::ReverseBoard()
{
	isReversed = !isReversed;
	int width = sBoard.getLocalBounds().width, height = sBoard.getLocalBounds().height;
	sBoard.setTextureRect(isReversed ? sf::IntRect(width, height, -width, -height) : sf::IntRect(0, 0, width, height));
	for (int i = 0; i < 24; i++)
	{
		figures[i].f.setPosition(sf::Vector2f(7, 7) * sizeT - figures[i].f.getPosition());
	}
}

bool CheckersBoard::IsClicked(const sf::Vector2f& mousePos) const
{
	return sBoard.getGlobalBounds().contains(mousePos - brOffset - figOffset);
}

CheckersBoard::Winner CheckersBoard::IsWinner() const
{
	for (int i = 0; i < 12; i++)
	{
		if (!figures[i].isDead && OtherMoves(i) != CANT_MOVE) break;
		if (i >= 11) return !isWhiteMove ? WHITE : NO_ONE;
	}
	for (int i = 12; i < 24; i++)
	{
		if (!figures[i].isDead && OtherMoves(i) != CANT_MOVE) break;
		if (i >= 23) return isWhiteMove ? BLACK : NO_ONE;
	}
	if (moveSum > 30) return DRAW;
	return NO_ONE;
}


void CheckersBoard::BoardDraw(sf::RenderWindow& window, const int& n)
{
	sBoard.move(brOffset);
	window.draw(sBoard);
	sBoard.move(-brOffset);
	for (int i = 0; i < 24; i++)
		figures[i].f.move(figOffset + brOffset);
	for (int i = 0; i < 24; i++)
		window.draw(figures[i].f); window.draw(figures[n].f);
	for (int i = 0; i < 24; i++)
		figures[i].f.move(-(figOffset + brOffset));
}