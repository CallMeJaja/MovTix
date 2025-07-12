#ifndef MOVIE_H
#define MOVIE_H

//============================================================
// MOVIE MANAGEMENT FUNCTION PROTOTYPES
//============================================================

void initializeMovies();
void showMovieList();
void showMovieDetail(int movieIndex);
void showMovieSchedule(int movieIndex);
void handleMovieListMenu();
void handleSearchMovie();

#endif // MOVIE_H