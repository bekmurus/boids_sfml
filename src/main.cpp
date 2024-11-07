#include <SFML/Graphics.hpp>
#include <vector>

#define FPS 60
#define NUM_BOIDS   5
#define TURN_FACTOR 0.5f

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

#define TOP_MARGIN    WIN_HEIGHT/5.0f
#define BOTTOM_MARGIN WIN_HEIGHT - TOP_MARGIN
#define LEFT_MARGIN   WIN_WIDTH/2.0f
#define RIGHT_MARGIN  WIN_WIDTH - LEFT_MARGIN

using namespace std;
using namespace sf;

int main() {
    // Create a window with the given dimensions
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT), "SFML Moving Circle Example");
    window.setFramerateLimit(FPS); // call it once, after creating the window

    // Create a circle shape with a radius of 10
    CircleShape circle(10.0f);
    circle.setFillColor(Color::Blue);

    // Velocity vector (speed and direction of movement)
    //Vector2f velocity(10.0f, 10.0f); // Move 0.1 pixels per frame in both X and Y directions

    vector<CircleShape> boids(NUM_BOIDS, circle);
    vector<Vector2f> velocities;


    

    for (uint16_t i = 1; i <= NUM_BOIDS; ++i)
    {
        boids[i].setPosition(10.0f, i*50.0f);
        velocities.push_back(Vector2f(i*1.0f, i*1.0f));
    }

    // Main loop: run while the window is open
    while (window.isOpen()) {
        // Event handling: close window when requested
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        for(uint16_t i = 0; i < NUM_BOIDS; ++i)
        {
            boids[i].move(velocities[i]);

            // Top margin
            if (boids[i].getPosition().y <= TOP_MARGIN)
            {
                velocities[i].y += TURN_FACTOR;
            }

            // Bottom margin
            if (boids[i].getPosition().y + boids[i].getRadius() * 2 >= BOTTOM_MARGIN)
            {
                velocities[i].y -= TURN_FACTOR;
            }

            // Left margin
            if (boids[i].getPosition().x <= LEFT_MARGIN)
            {
                velocities[i].x += TURN_FACTOR;
            }

            // Right margin
            if (boids[i].getPosition().x + boids[i].getRadius() * 2 >= RIGHT_MARGIN)
            {
                velocities[i].x -= TURN_FACTOR;
            }
        }

        // Clear the window with a black color
        window.clear(Color::Black);
        for(auto &boid: boids)
        {
            // Draw the circle
            window.draw(boid);
        }
        
        // Display what was drawn to the screen
        window.display();
    }

    return 0;
}
