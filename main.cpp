//Programmers: Johnny Myers Jr & Michael Campise
//Class: COP 2006, Spring 2025
//Updated Date: 08 April, 2025
//Description: Runs a SFML GUI of a coded Snakes and Ladders Game!

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <array>
#include <random>
#include <iostream>
#include <unordered_map>

int main()
{
    const int windowWidth = 800;
    const int windowHeight = 800;
    const int rows = 10;
    const int cols = 10;
    int roll1;
    int currentTile = 0; // Track the current tile the sprite is on
    const int totalTiles = rows * cols; // 100 tiles in total

    // Calculate the size of each cell
    const float cellWidth = static_cast<float>(windowWidth) / cols;
    const float cellHeight = static_cast<float>(windowHeight) / rows;

    // Font
    const sf::Font font("font.otf");

    // create the window
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "My window");

    std::array<sf::Texture, 6> diceTextures;
    for (int i = 0; i < 6; ++i)
    {
        if (!diceTextures[i].loadFromFile("dice" + std::to_string(i + 1) + ".png"))
        {
            std::cerr << "Failed to load dice" << (i + 1) << ".png\n";
            return 1;
        }
    }

    sf::Texture texture2;
    if (!texture2.loadFromFile("board.png"))
    {
        std::cerr << "Error loading texture" << std::endl;
        return -1;
    }

    sf::Sprite board(texture2);
    board.setPosition({0.f, 0.f});

    // Set up pawn sprite
    sf::Texture texture;
    if (!texture.loadFromFile("sprite.png"))
    {
        std::cerr << "Error loading texture" << std::endl;
        return -1;
    }

    sf::Sprite sprite(texture);
    sprite.setTextureRect({{0, 0}, {80, 80}});
    sprite.setPosition({0.f, 710.f});

    // Set up dice sprites
    sf::Sprite die1(diceTextures[0]);
    die1.setPosition({850.f, 250.f});
    die1.setScale({0.25f, 0.25f});

    // Set up button
    sf::RectangleShape button(sf::Vector2f(120.f, 50.f));
    button.setFillColor(sf::Color(200, 200, 200));
    button.setOutlineColor(sf::Color::Black);
    button.setOutlineThickness(2.f);
    button.setPosition({850.f, 445.f});

    sf::Text buttonText(font, "Roll Dice");
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition({860.f, 455.f});

    sf::Text winText(font, "You Win!");
    winText.setCharacterSize(200);
    winText.setFillColor(sf::Color::Red);
    winText.setPosition({150.f, 200.f});

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 5); // 0 to 5 → dice1.png to dice6.png

    // Ladder and Snake mappings (updated)
    std::unordered_map<int, int> ladders =
    {
        {3, 37}, {20, 39}, {46, 55}, {51, 69}, {60, 80},
        {83, 98}
    };
    std::unordered_map<int, int> snakes =
    {
        {34, 14}, {40, 2}, {50, 31}, {67, 36}, {82, 62},
        {97, 65}
    };

    bool hasWon = false;

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        // Check all the window's events that were triggered since the last iteration of the loop
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent)
                {
                    sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x), static_cast<float>(mouseEvent->position.y));
                    if (button.getGlobalBounds().contains(mousePos))
                    {
                        // Click occurred within the area
                        roll1 = dist(gen);
                        die1.setTexture(diceTextures[roll1]);
                        std::cout << "Rolled: " << (roll1 + 1) << std::endl;

                        int stepsRemaining = roll1 + 1;

                        // First, calculate the potential destination tile
                        int tempTile = currentTile + stepsRemaining;

                        // Check if the player can land exactly on the last tile
                        if (tempTile > totalTiles)
                        {
                            // If they can't land exactly, don't move
                            std::cout << "Cannot move, you need an exact roll to land on tile " << totalTiles << std::endl;
                            continue;
                        }

                        // Check if we land on a ladder or snake, and apply it only once
                        if (ladders.find(tempTile) != ladders.end())
                        {
                            currentTile = ladders[tempTile];
                            std::cout << "Landed on ladder! Now on tile: " << currentTile << std::endl;
                        }
                        else if (snakes.find(tempTile) != snakes.end())
                        {
                            currentTile = snakes[tempTile];
                            std::cout << "Landed on snake! Now on tile: " << currentTile << std::endl;
                        }
                        else
                        {
                            currentTile = tempTile;
                        }

                        // Now, move the sprite to the new tile
                        int row = (currentTile - 1) / cols;
                        int col = (currentTile - 1) % cols;

                        // Flip direction every other row (like zigzag)
                        if (row % 2 == 1)
                        {
                            col = cols - 1 - col;
                        }

                        float spriteX = col * cellWidth;
                        float spriteY = windowHeight - cellHeight * (row + 1);

                        sprite.setPosition({spriteX, spriteY});

                        // Check if the player has won
                        if (currentTile == totalTiles)
                        {
                            hasWon = true;
                            std::cout << "You Win!" << std::endl;
                        }
                    }
                }
            }

            // Clear the window with a color
            window.clear(sf::Color(139, 90, 43));

            // Draw vertical lines (grid)
            for (int i = 0; i <= cols; ++i)
            {
                std::array line =
                {
                    sf::Vertex{sf::Vector2f(i * cellWidth, 0), sf::Color::Black},
                    sf::Vertex{sf::Vector2f(i * cellWidth, windowHeight), sf::Color::Black}
                };
                window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
            }

            // Draw horizontal lines (grid)
            for (int i = 0; i <= rows; ++i)
            {
                std::array line =
                {
                    sf::Vertex{sf::Vector2f(0, i * cellHeight), sf::Color::Black},
                    sf::Vertex{sf::Vector2f(windowWidth, i * cellHeight), sf::Color::Black}
                };
                window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
            }

            // Draw dice
            window.draw(die1);

            // Draw button
            window.draw(button);
            window.draw(buttonText);

            // Draw sprite
            window.draw(sprite);

            window.draw(board);

            // Draw win text if the player has won
            if (hasWon)
            {
                window.draw(winText);
            }

            // End the current frame
            window.display();
        }
    }
    return 0;
}
