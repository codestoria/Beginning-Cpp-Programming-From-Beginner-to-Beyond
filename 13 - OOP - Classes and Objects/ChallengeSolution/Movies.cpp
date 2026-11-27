// Section 13 Challenge Solution: a collection of movies.

#include <iostream>
#include "Movies.h"

// Default constructor.
Movies::Movies() {}

// Destructor.
Movies::~Movies() {}

// Add a movie if its name is not already present; return whether it was added.
bool Movies::add_movie(std::string name, std::string rating, int watched) {
    for (const Movie &movie : movies) {
        if (movie.get_name() == name)
            return false;
    }

    Movie temp{name, rating, watched};
    movies.push_back(temp);

    return true;
}

// Increment the watched count for the named movie; return whether it was found.
bool Movies::increment_watched(std::string name) {
    for (Movie &movie : movies) {
        if (movie.get_name() == name) {
            movie.increment_watched();
            return true;
        }
    }

    return false;
}

// Display all movies, or a message if the collection is empty.
void Movies::display() const {
    if (movies.size() == 0) {
        std::cout << "Sorry, no movies to display\n" << std::endl;
    } else {
        std::cout << "\n===================================" << std::endl;
        for (const auto &movie : movies)
            movie.display();
        std::cout << "\n===================================" << std::endl;
    }
}
