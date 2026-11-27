// Section 13 Challenge Solution: exercise the Movies collection.

#include <iostream>
#include "Movies.h"

// Function prototypes
void increment_watched(Movies &movies, std::string name);
void add_movie(Movies &movies, std::string name, std::string rating, int watched);

// Report whether the named movie's watched count was incremented.
void increment_watched(Movies &movies, std::string name) {
    if (movies.increment_watched(name)) {
        std::cout << name << " watch incremented" << std::endl;
    } else {
        std::cout << name << " not found" << std::endl;
    }
}

// Report whether the movie was added or its name already existed.
void add_movie(Movies &movies, std::string name, std::string rating, int watched) {
    if (movies.add_movie(name, rating, watched)) {
        std::cout << name << " added" << std::endl;
    } else {
        std::cout << name << " already exists" << std::endl;
    }
}

int main() {
    Movies my_movies;
    my_movies.display();

    add_movie(my_movies, "Big", "PG-13", 2);      // Added.
    add_movie(my_movies, "Star Wars", "PG", 5);   // Added.
    add_movie(my_movies, "Cinderella", "PG", 7);  // Added.

    my_movies.display();  // Big, Star Wars, Cinderella

    add_movie(my_movies, "Cinderella", "PG", 7);  // Already exists
    add_movie(my_movies, "Ice Age", "PG", 12);    // Added.

    my_movies.display(); // Big, Star Wars, Cinderella, Ice Age

    increment_watched(my_movies, "Big");      // Incremented.
    increment_watched(my_movies, "Ice Age");  // Incremented.

    my_movies.display();  // The watched counts for Big and Ice Age have increased by one.

    increment_watched(my_movies, "XXX");  // XXX was not found.

    return 0;
}
