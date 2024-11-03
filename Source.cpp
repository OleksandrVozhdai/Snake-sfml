#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>

using namespace std;

int rows = 50;
int columns = 50;
int cellSize = 8;

vector<vector<int>> grid(rows, vector<int>(columns, 0));
vector<pair<int, int>> snake;
pair<int, int> foodpos;
int length = 2;

pair<int, int> generateRandomCoordinates()
{
	int r = rand() % rows;
	int r1 = rand() & columns;
	return { r, r1 };
}

void InitializeGrid(int i, int j)
{
	if (snake.size() < length)
	{
		snake.push_back({ i,j });
	}
	else
	{
		grid[snake[0].first][snake[0].second] = 0;
		snake.erase(snake.begin());
		snake.push_back({ i, j });
	}
	grid[i][j] = 1;
}


void SnakeMovement(int& i, int& j, int direction)
{
	switch (direction) {
	case 0:
		j++;
		break;
	case 1:
		i++;
		break;
	case 2:
		j--;
		break;
	case 3:
		i--;
		break;
	}
}

void WindowRec(int& i, int& j)
{
	if (i < 0)
	{
		i = rows - 1;
	}
	else if (i >= rows)
	{
		i = 0;
	}
	else if (j < 0)
	{
		j = columns - 1;
	}
	else if (j >= columns)
	{
		j = 0;
	}
}

void spawnFood() {
	do {
		foodpos = generateRandomCoordinates();
	} while (grid[foodpos.first][foodpos.second] == 1);
	grid[foodpos.first][foodpos.second] = 2;
}


int main()
{
	srand(static_cast<unsigned int>(time(0)));
	int i = 1, j = 2;
	int direction = 0;
	sf::RenderWindow window(sf::VideoMode(400, 400), "SFML Snake");

	spawnFood();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction != 2) {
			direction = 0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction != 3) {
			direction = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction != 0) {
			direction = 2;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction != 1) {
			direction = 3;
		}

		int oldI = i, oldJ = j;

		SnakeMovement(i, j, direction);
		WindowRec(i, j);
		InitializeGrid(i, j);

		if (i == foodpos.first && j == foodpos.second)
		{
			length++;
			spawnFood();
		}

		for (int k = 0; k < snake.size() - 1; ++k)
		{
			if (snake[k].first == i && snake[k].second == j)
			{
				std::cout << "Game Over! You collided with your body." << std::endl;
				return 0;
			}
		}

		window.clear(sf::Color::Black);


		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < columns; ++col)
			{
				if (grid[row][col] == 1)
				{
					sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
					cell.setPosition(col * cellSize, row * cellSize);
					cell.setFillColor(sf::Color::Green);
					window.draw(cell);
				}
				else if (grid[row][col] == 2)
				{
					sf::RectangleShape food(sf::Vector2f(cellSize - 1, cellSize - 1));
					food.setPosition(col * cellSize, row * cellSize);
					food.setFillColor(sf::Color::Red);
					window.draw(food);
				}
			}
		}
		window.display();
		sf::sleep(sf::milliseconds(100));
	}
	return 0;
}