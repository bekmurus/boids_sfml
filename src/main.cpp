#include <SFML/Graphics.hpp>
#include <vector>
#include "../include/sim_config.hpp"

using namespace std;
using namespace sf;

int main() {
    // Create a window with the given dimensions
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT), "BoidSim");
    window.setFramerateLimit(FPS); // call it once, after creating the window

    // Create a boid shape
    CircleShape circle(1.0f);
    circle.setFillColor(Color::White);

    // Create a predator shape
    CircleShape predator(10.0f);
    predator.setFillColor(Color::Red);
    predator.setPosition(Vector2f(WIN_WIDTH/2, WIN_HEIGHT/2));

    // Vector of boid elements
    vector<CircleShape> boids(NUM_BOIDS, circle);
    // Velocity vector (speed and direction of movement)
    vector<Vector2f> velocities;


    for (uint16_t i = 0; i <= NUM_BOIDS; ++i)
    {
        boids[i].setPosition(10.0f, i*50.0f);
        velocities.push_back(Vector2f(i*1.0f, i*1.0f));
    }

    float close_dx = 0;
    float close_dy = 0;
    float xvel_avg = 0;
    float yvel_avg = 0;
    float xpos_avg = 0;
    float ypos_avg = 0;
    float neighbor_cnt = 0;
    float dx = 0;
    float dy = 0;

    // Main loop: run while the window is open
    while (window.isOpen()) {
        // Event handling: close window when requested
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            
            if (event.type == Event::MouseMoved)
            {
                // Move the predator with mouse cursor
                predator.setPosition(Vector2f(event.mouseMove.x, event.mouseMove.y));
            }
        }

        
        for (uint16_t current = 0; current < NUM_BOIDS; ++current)
        {
            /* Zero accumulating variables */
            close_dx = 0;
            close_dy = 0;
            xvel_avg = 0;
            yvel_avg = 0;
            xpos_avg = 0;
            ypos_avg = 0;
            neighbor_cnt = 0;

            /* Flocking Algorithm */
            for (uint16_t other = current+1; other < NUM_BOIDS; ++other)
            {
                // get distance between boids
                dx = boids[current].getPosition().x - boids[other].getPosition().x;
                dy = boids[current].getPosition().y - boids[other].getPosition().y;

                float distance_sqrd = dx*dx + dy*dy;

                
                if (distance_sqrd < PROTECTED_RANGE_SQRD)
                {
                    close_dx += dx; /* Separation */
                    close_dy += dy; /* Separation */
                }

                else if(distance_sqrd < VISUAL_RANGE_SQRD)
                {
                    xvel_avg += velocities[other].x; /* Alignment */
                    yvel_avg += velocities[other].y; /* Alignment */

                    xpos_avg += boids[other].getPosition().x; /* Cohesion */
                    ypos_avg += boids[other].getPosition().y; /* Cohesion */

                    neighbor_cnt++;
                }

            }

            if (0 < neighbor_cnt)
            {
                xvel_avg = xvel_avg/neighbor_cnt; /* Alignment */
                yvel_avg = yvel_avg/neighbor_cnt; /* Alignment */

                xpos_avg = xpos_avg/neighbor_cnt; /* Cohesion */
                ypos_avg = ypos_avg/neighbor_cnt; /* Cohesion */

                velocities[current].x += (xpos_avg - boids[current].getPosition().x) * CENTERING_FACTOR; /* Cohesion */
                velocities[current].y += (ypos_avg - boids[current].getPosition().y) * CENTERING_FACTOR; /* Cohesion */

                velocities[current].x += (xvel_avg - velocities[current].x) * MATCHING_FACTOR; /* Alignment */
                velocities[current].y += (yvel_avg - velocities[current].y) * MATCHING_FACTOR; /* Alignment */
            }
            

            velocities[current].x += close_dx * AVOID_FACTOR; /* Separation */
            velocities[current].y += close_dy * AVOID_FACTOR; /* Separation */

            /* Screen edges */
            /* Top margin */
            if (boids[current].getPosition().y <= TOP_MARGIN)
            {
                velocities[current].y += TURN_FACTOR;
            }

            /* Bottom margin */
            if (boids[current].getPosition().y + boids[current].getRadius() * 2 >= BOTTOM_MARGIN)
            {
                velocities[current].y -= TURN_FACTOR;
            }

            /* Left margin */
            if (boids[current].getPosition().x <= LEFT_MARGIN)
            {
                velocities[current].x += TURN_FACTOR;
            }

            /* Right margin */ 
            if (boids[current].getPosition().x + boids[current].getRadius() * 2 >= RIGHT_MARGIN)
            {
                velocities[current].x -= TURN_FACTOR;
            }

            // Speed calculation of current boid
            float speed = sqrtf(velocities[current].x * velocities[current].x + velocities[current].y * velocities[current].y);

            /* Speed Limits */
            if (MIN_SPEED > speed)
            {
                velocities[current].x = (velocities[current].x / speed) * MIN_SPEED;
                velocities[current].y = (velocities[current].y / speed) * MIN_SPEED;
            }
            if (MAX_SPEED < speed)
            {
                velocities[current].x = (velocities[current].x / speed) * MAX_SPEED;
                velocities[current].y = (velocities[current].y / speed) * MAX_SPEED;
            }

            /* Avoid the predator */
            dx = boids[current].getPosition().x - predator.getPosition().x;
            dy = boids[current].getPosition().y - predator.getPosition().y;

            float distance_sqrd = dx*dx + dy*dy;
            if (distance_sqrd < VISUAL_RANGE_SQRD*2)
            {
                velocities[current].x += dx * 0.005f; /* Avoid */
                velocities[current].y += dy * 0.005f; /* Avoid */
            }

            boids[current].move(velocities[current]);
        } // end for loop

        // Clear the window with a black color
        window.clear(Color::Black);
        for(auto &boid: boids)
        {
            // Draw the boids
            window.draw(boid);
        }

        // Draw the predator
        window.draw(predator);
        
        // Display what was drawn to the screen
        window.display();
    }

    return 0;
}
