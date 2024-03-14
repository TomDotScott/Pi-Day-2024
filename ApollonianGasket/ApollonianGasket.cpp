// ApollonianGasket.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    // Declare and create a new render-window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Apollonian Gasket");

    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);

    // The main loop - ends as soon as the window is closed
    while (window.isOpen())
    {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the whole window before rendering a new frame
        window.clear();

        // End the current frame and display its contents on screen
        window.display();
    }
}