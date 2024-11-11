#pragma once

// Window configuration
#define WIN_WIDTH  800
#define WIN_HEIGHT 600
#define FPS 60

// Boid parameters
#define NUM_BOIDS         50
#define VISUAL_RANGE      50.0f
#define VISUAL_RANGE_SQRD (VISUAL_RANGE * VISUAL_RANGE)
#define PROTECTED_RANGE   5.0f
#define PROTECTED_RANGE_SQRD (PROTECTED_RANGE * PROTECTED_RANGE)

// Behavior factors
#define TURN_FACTOR       0.2f
#define MATCHING_FACTOR   0.05f
#define AVOID_FACTOR      0.05f
#define CENTERING_FACTOR  0.0005f

// Speed limits
#define MIN_SPEED 2
#define MAX_SPEED 3

// Window margins
#define TOP_MARGIN    WIN_HEIGHT/16.0f
#define BOTTOM_MARGIN WIN_HEIGHT - TOP_MARGIN
#define LEFT_MARGIN   WIN_WIDTH/16.0f
#define RIGHT_MARGIN  WIN_WIDTH - LEFT_MARGIN 