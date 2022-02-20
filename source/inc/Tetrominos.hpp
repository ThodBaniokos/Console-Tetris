// Tetrominos.hpp : Include file for standard system include files,
// or project specific include files.
// inc guard
#pragma once

// libs used
#include <iostream>

class Tetrominos {

	public:

		// using string representation for all of the 7 common tetris pieces
		static const int num_pieces = 7;

		// using wstring since we're going to use the windows screen buffer
		// working with wchars
		std::wstring pieces[num_pieces];
		
		// constructor
		Tetrominos();

		// get index of tetromino piece, based on it's current x and y value, by it's current rotation
		int rotation_index(const int x, const int y, const int curr_rot);
};


