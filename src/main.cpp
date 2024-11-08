#include <SFML/Graphics.hpp>
#include <vector>

#define FPS 30

#define NUM_BOIDS         50
#define VISUAL_RANGE      100.0f
#define VISUAL_RANGE_SQRD      (VISUAL_RANGE * VISUAL_RANGE)
#define PROTECTED_RANGE   10.0f
#define PROTECTED_RANGE_SQRD   (PROTECTED_RANGE * PROTECTED_RANGE)
#define TURN_FACTOR       0.2f
#define MATCHING_FACTOR   0.05f
#define AVOID_FACTOR      0.05f
#define CENTERING_FACTOR  0.0005f
#define MIN_SPEED 2
#define MAX_SPEED 3

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

#define TOP_MARGIN    WIN_HEIGHT/16.0f
#define BOTTOM_MARGIN WIN_HEIGHT - TOP_MARGIN
#define LEFT_MARGIN   WIN_WIDTH/16.0f
#define RIGHT_MARGIN  WIN_WIDTH - LEFT_MARGIN

using namespace std;
using namespace sf;

int main() {
    // Create a window with the given dimensions
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT), "SFML Moving Circle Example");
    window.setFramerateLimit(FPS); // call it once, after creating the window

    // Create a circle shape with a radius of 10
    CircleShape circle(1.0f);
    circle.setFillColor(Color::White);

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
            

            velocities[current].y += close_dy * AVOID_FACTOR; /* Separation */
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

            boids[current].move(velocities[current]);
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
