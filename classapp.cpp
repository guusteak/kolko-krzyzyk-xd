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

// Klasa reprezentująca grę "Kółko i krzyżyk"
class TicTacToe
{
public:
    TicTacToe();

    // Funkcja odpowiadająca za główną pętlę gry
    void run();

private:
    // Funkcja rysująca planszę i jej stan
    void draw();

    // Funkcja sprawdzająca, czy dane pole wyzwala wygraną gracza
    bool checkWin(int x, int y);

    // Plansza gry
    Cell m_board[BOARD_SIZE][BOARD_SIZE];

    // Okno wyświetlające grę
    sf::RenderWindow m_window;

    // Losowy generator liczb
    std::mt19937 m_rng;
    std::uniform_int_distribution<int> m_dist;

    // Zmienne przechowujące stan gry
    bool m_isCircleTurn = true; // czy ruch wykonuje kółko (true) czy krzyżyk (false)
    bool m_isGameOver = false; // czy gra została zakończona
    int m_occupiedCells = 0; // liczba zajętych pól
    int m_result = 0; // wynik gry (-1 - przegrana kółka, 0 - remis, 1 - wygrana kółka)
};

TicTacToe::TicTacToe()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Kółko i krzyżyk")
    , m_rng(std::random_device{}())
    , m_dist(0, BOARD_SIZE - 1)
{
    // Inicjalizacja planszy
    for(int i = 0; i < BOARD_SIZE; i++)
{
for (int j = 0; j < BOARD_SIZE; j++)
{
m_board[i][j].shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
m_board[i][j].shape.setPosition(i * CELL_SIZE, j * CELL_SIZE);
m_board[i][j].shape.setOutlineThickness(1);
m_board[i][j].shape.setOutlineColor(sf::Color::Black);
}
}
}

void TicTacToe::run()
{
while (m_window.isOpen())
{
sf::Event event;
while (m_window.pollEvent(event))
{
if (event.type == sf::Event::Closed)
m_window.close();
// Obsługa zdarzenia kliknięcia myszą
        if (event.type == sf::Event::MouseButtonPressed && !m_isGameOver)
        {
            // Sprawdzenie, czy kliknięto na planszę
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
            if (mousePos.x >= 0 && mousePos.x < BOARD_SIZE * CELL_SIZE
                && mousePos.y >= 0 && mousePos.y < BOARD_SIZE * CELL_SIZE)
            {
                // Znalezienie odpowiedniego pola na planszy
                int x = mousePos.x / CELL_SIZE;
                int y = mousePos.y/ CELL_SIZE;
Cell& cell = m_board[x][y];
if (!cell.isOccupied)
{
// Zajęcie pola
cell.isOccupied = true;
cell.isCircle = m_isCircleTurn;
m_occupiedCells++;
// Rysowanie odpowiedniego symbolu na polu
                    if (m_isCircleTurn)
                        cell.shape.setFillColor(sf::Color::Red);
                    else
                        cell.shape.setFillColor(sf::Color::Blue);

                    // Sprawdzenie, czy któryś z graczy wygrał
                    if (checkWin(x, y))
                    {
                        m_isGameOver = true;
                        m_result = (m_isCircleTurn ? 1 : -1);
                    }
                    else if (m_occupiedCells == BOARD_SIZE * BOARD_SIZE)
                    {
                        // Remis
                        m_isGameOver = true;
                        m_result = 0;
                    }
                    else
                    {
                        // Zmiana gracza
                        m_isCircleTurn = !m_isCircleTurn;

                        // Ruch komputera (jeśli to jego tura)
                        if (!m_isCircleTurn)
                        {
                            // Losowanie wolnego pola
                            int x, y;
                            do
                            {
                                x = m_dist(m_rng);
                                y = m_dist(m_rng);
                            } while (m_board[x][y].isOccupied);

                            // Zajęcie pola przez komputer
                            Cell& cell = m_board[x][y];
                            cell.isOccupied = true;
                            cell.isCircle = false;
                            cell.shape.setFillColor(sf::Color::Blue);

                            // Sprawdzenie, czy komputer wygrał
                            if (checkWin(x, y))
                            {
                                m_isGameOver = true;
                                m_result = -1;
                            }
                        }
                    }
                }
            }
        }
    }

    // Czyszczenie okna
    m_window.clear();

    // Rysowanie planszy
    draw();

    // Wyświetlenie okna
    m_window.display();
}
}

void TicTacToe::draw()
{
for (int i = 0; i < BOARD_SIZE; i++)
{
for (int j = 0; j < BOARD_SIZE; j++)
{
m_window.draw(m_board[i][j].shape);
}
}
}

bool TicTacToe::checkWin(int x, int y)
{
// Sprawdzenie wiersza
int count = 0;
for (int i= 0; i < BOARD_SIZE; i++)
{
if (m_board[i][y].isCircle == m_board[x][y].isCircle)
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
    if (m_board[x][i].isCircle == m_board[x][y].isCircle)
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
        if (m_board[i][i].isCircle == m_board[x][y].isCircle)
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
        if (m_board[i][BOARD_SIZE - i - 1].isCircle == m_board[x][y].isCircle)
            count++;
        else
            break;
    }
    if (count == BOARD_SIZE)
        return true;
}

return false;
}

int main()
{
TicTacToe game;
game.run();
return 0;
}
//pozdro