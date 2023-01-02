#ifndef HIGHSCORES_HPP
#define HIGHSCORES_HPP

#include <iostream>
#include <fstream>
#include <algorithm>

const int NUM_HIGH_SCORES = 5; // Number of high scores to store

// High score structure
struct HighScore
{
    std::string name;  // Name of the player
    int score;         // Score achieved
};

// High scores array
HighScore highScores[NUM_HIGH_SCORES];

// Read high scores from file
void ReadHighScores()
{
    std::ifstream file("other/highscores.txt");
    if (file.is_open())
    {
        for (int i = 0; i < NUM_HIGH_SCORES; i++)
        {
            file >> highScores[i].name;
            file >> highScores[i].score;
        }
        file.close();
    }
}

// Write high scores to file
void WriteHighScores()
{
    std::ofstream file("other/highscores.txt");
    if (file.is_open())
    {
        for (int i = 0; i < NUM_HIGH_SCORES; i++)
        {
            file << highScores[i].name << " " << highScores[i].score << std::endl;
        }
        file.close();
    }
}

// Update high scores
void UpdateHighScores(std::string name, int score)
{
    // Check if the score is high enough to make it into the top 3
    if (score > highScores[NUM_HIGH_SCORES - 1].score)
    {
        // Add the new score to the high scores array
        highScores[NUM_HIGH_SCORES - 1].name = name;
        highScores[NUM_HIGH_SCORES - 1].score = score;

        // Sort the high scores array in descending order
        std::sort(highScores, highScores + NUM_HIGH_SCORES, [](const HighScore& a, const HighScore& b)
        {
            return a.score > b.score;
        });
    }
}

#endif //! HIGHSCORES_HPP