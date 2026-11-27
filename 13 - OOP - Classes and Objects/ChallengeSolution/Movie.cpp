// Section 13 Challenge Solution: a movie and its attributes.

#include <iostream>
#include "Movie.h"

// Constructor.
Movie::Movie(std::string name, std::string rating, int watched)
    : name(name), rating(rating), watched(watched) {}

// Copy constructor.
Movie::Movie(const Movie &source) : Movie{source.name, source.rating, source.watched} {}

// Destructor.
Movie::~Movie() {}

// Print the movie attributes.
void Movie::display() const {
    std::cout << name << ", " << rating << ", " << watched << std::endl;
}
