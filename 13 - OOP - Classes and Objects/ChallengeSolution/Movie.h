// Section 13 Challenge Solution: a movie and its attributes.

#ifndef SECTION13_CHALLENGESOLUTION_MOVIE_H
#define SECTION13_CHALLENGESOLUTION_MOVIE_H

#include <string>

class Movie {
private:
    std::string name;    // Movie name.
    std::string rating;  // Movie rating.
    int watched;         // Number of times watched.

public:
    // Initialize all three movie attributes.
    Movie(std::string name, std::string rating, int watched);

    // Copy constructor
    Movie(const Movie &source);

    // Destructor
    ~Movie();

    // Get and set the movie attributes.
    void set_name(std::string name) { this->name = name; }
    std::string get_name() const { return name; }

    void set_rating(std::string rating) { this->rating = rating; }
    std::string get_rating() const { return rating; }

    void set_watched(int watched) { this->watched = watched; }
    int get_watched() const { return watched; }

    // Increment the watched count.
    void increment_watched() { ++watched; }

    // Print the movie information, for example: Big, PG-13, 2.
    void display() const;
};

#endif  // SECTION13_CHALLENGESOLUTION_MOVIE_H
