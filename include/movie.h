#ifndef MOVIE_H
#define MOVIE_H

#include "types.h"

void initializeMovies();
void calculateAverageRating(int movieIndex);
void formatShowtimes(Movie movie, char *result);
void sortMoviesByRating();
void sortMoviesByPopularity();
void sortMoviesByTitle();
void sortMoviesByDuration();
void searchMovies(const char *keyword, int results[], int &resultCount);

#endif // MOVIE_H