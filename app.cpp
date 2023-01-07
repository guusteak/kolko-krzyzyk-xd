#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 500;
constexpr int BOARD_SIZE = 10; // rozmiar planszy (w polach)
constexpr int CELL_SIZE = 50; // rozmiar pojedynczego pola (w pikselach)

// Struktura przechowująca stan pojedynczego pola planszy
struct Cell
{
    sf::RectangleShape shape;
    bool isOccupied = false; // czy pole jest zajęte
    bool isCircle = false; // czy na polu jest kółko (true) czy krzyżyk (false)
};

int main()
{
    // Inicjalizacja okna
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Kółko i krzyżyk");

    // Inicjalizacja planszy
    Cell board[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j].shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            board[i][j].shape.setPosition(i * CELL_SIZE, j * CELL_SIZE);
            board[i][j].shape.setOutlineThickness(1);
            board[i][j].shape.setOutlineColor(sf::Color::Black);
        }
    }

    // Losowy generator liczb
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, BOARD_SIZE - 1);

    // Zmienne przechowujące stan gry
    bool isCircleTurn = true; // czy ruch wykonuje kółko (true) czy krzyżyk (false)
    bool isGameOver = false; // czy gra została zakończona
    int occupiedCells = 0; // liczba zajętych pól
    int result = 0; // wynik gry (-1 - przegrana kółka, 0 - remis, 1 - wygrana kółka)

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Obsługa zdarzenia kliknięcia myszą
            if (event.type == sf::Event::MouseButtonPressed && !isGameOver)
{
// Sprawdzenie, czy kliknięto na planszę
sf::Vector2i mousePos = sf::Mouse::getPosition(window);
if (mousePos.x >= 0 && mousePos.x < BOARD_SIZE * CELL_SIZE
&& mousePos.y >= 0 && mousePos.y < BOARD_SIZE * CELL_SIZE)
{
// Znalezienie odpowiedniego pola na planszy
int x = mousePos.x / CELL_SIZE;
int y = mousePos.y / CELL_SIZE;
Cell& cell = board[x][y];
if (!cell.isOccupied)
{
// Zajęcie pola
cell.isOccupied = true;
cell.isCircle = isCircleTurn;
occupiedCells++;
// Rysowanie odpowiedniego symbolu na polu
                    if (isCircleTurn)
                        cell.shape.setFillColor(sf::Color::Red);
                    else
                        cell.shape.setFillColor(sf::Color::Blue);

                    // Sprawdzenie, czy któryś z graczy wygrał
                    if (checkWin(board, x, y))
                    {
                        isGameOver = true;
                        result = (isCircleTurn ? 1 : -1);
                    }
                    else if (occupiedCells == BOARD_SIZE * BOARD_SIZE)
                    {
                        // Remis
                        isGameOver = true;
                        result = 0;
                    }
                    else
                    {
                        // Zmiana gracza
                        isCircleTurn = !isCircleTurn;

                        // Ruch komputera (jeśli to jego tura)
                        if (!isCircleTurn)
                        {
                            // Losowanie wolnego pola
                            int x, y;
                            do
                            {
                                x = dist(rng);
                                y = dist(rng);
                            } while (board[x][y].isOccupied);

                            // Zajęcie pola przez komputer
                            Cell& cell = board[x][y];
                            cell.isOccupied = true;
                            cell.isCircle = false;
                            cell.shape.setFillColor(sf::Color::Blue);

                            // Sprawdzenie, czy komputer wygrał
                            if (checkWin(board, x, y))
                            {
                                isGameOver = true;
                                result = -1;
                            }
                        }
                    }
                }
            }
        }
    }

    // Czyszczenie okna
    window.clear();

    // Rysowanie planszy
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            window.draw(board[i][j].shape);
        }
    }
    // Wyświetlenie okna
window.display();
}
return 0;
}

// Funkcja sprawdzająca, czy dane pole wyzwala wygraną gracza
bool checkWin(const Cell board[BOARD_SIZE][BOARD_SIZE], int x, int y)
{
// Sprawdzenie wiersza
int count = 0;
for (int i = 0; i < BOARD_SIZE; i++)
{
if (board[i][y].isCircle == board[x][y].isCircle)
count++;
else
break;
}
if (count == BOARD_SIZE)
return true;
// Sprawdzenie kolumny
count = 0;
for (int i = 0; i < BOARD_SIZE; i++)
{
    if (board[x][i].isCircle == board[x][y].isCircle)
        count++;
    else
        break;
}
if (count == BOARD_SIZE)
    return true;

// Sprawdzenie skosu (jeśli jest to możliwe)
if (x == y)
{
    count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i][i].isCircle == board[x][y].isCircle)
            count++;
        else
            break;
    }
    if (count == BOARD_SIZE)
        return true;
}
if (x == BOARD_SIZE - y - 1)
{
    count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i][BOARD_SIZE - i - 1].isCircle == board[x][y].isCircle)
            count++;
        else
            break;
    }
    if (count == BOARD_SIZE)
        return true;
}

return false;
}