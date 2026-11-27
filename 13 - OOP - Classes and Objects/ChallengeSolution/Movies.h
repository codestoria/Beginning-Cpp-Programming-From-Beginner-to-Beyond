// Section 13 Challenge Solution: a collection of movies.

#ifndef SECTION13_CHALLENGESOLUTION_MOVIES_H
#define SECTION13_CHALLENGESOLUTION_MOVIES_H

#include <vector>
#include <string>
#include "Movie.h"

class Movies {
private:
    std::vector<Movie> movies;

public:
    Movies();   // Constructor
    ~Movies();  // Destructor

    // Add a movie if its name is not already present; return whether it was added.
    bool add_movie(std::string name, std::string rating, int watched);

    // Increment the watched count for the named movie; return whether it was found.
    bool increment_watched(std::string name);

    // Display all movies, or a message if the collection is empty.
    void display() const;
};

#endif  // SECTION13_CHALLENGESOLUTION_MOVIES_H
