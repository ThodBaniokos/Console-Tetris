// Tetrominos.cpp : Defines the entry point for the application.
//

#include "Tetrominos.hpp"

Tetrominos::Tetrominos()
{
	// create all the shapes used in a game of tetris
	// appending four chars at a time. Visually represent the piece inserted
	// also every piece is implemented as an array of size 4 * 4 since the I
	// piece is the longest and needs this much space to be drawn on screen
	// for example the insertion below is identical to -> L"..Y...Y...Y...Y."
	// dots means there's nothing there and Y means there's a piece fragment there
	// I tetromino piece
	pieces[0].append(L"..Y.");
	pieces[0].append(L"..Y.");
	pieces[0].append(L"..Y.");
	pieces[0].append(L"..Y.");

	// L tetromino piece
	pieces[1].append(L"....");
	pieces[1].append(L"..Y.");
	pieces[1].append(L"..Y.");
	pieces[1].append(L"..YY");

	// O tetromino piece
	pieces[2].append(L"....");
	pieces[2].append(L".YY.");
	pieces[2].append(L".YY.");
	pieces[2].append(L"....");

	// S tetromino piece
	pieces[3].append(L"....");
	pieces[3].append(L"..YY");
	pieces[3].append(L".YY.");
	pieces[3].append(L"....");

	// Z tetromino piece
	pieces[4].append(L"....");
	pieces[4].append(L"YY..");
	pieces[4].append(L".YY.");
	pieces[4].append(L"....");

	// T tetromino piece
	pieces[5].append(L"....");
	pieces[5].append(L"..Y.");
	pieces[5].append(L".YY.");
	pieces[5].append(L"..Y.");

	// J tetromino piece
	pieces[6].append(L"....");
	pieces[6].append(L"..Y.");
	pieces[6].append(L"..Y.");
	pieces[6].append(L".YY.");

}

int Tetrominos::rotation_index(const int x, const int y, const int curr_rot) {

	// index returned for the current rotation of the piece
	// initialising with -1, indicating error
	int piece_index = -1;

	// four rotation states for tetrominos
	// described as comments below
	switch (curr_rot % 4)
	{
		// 0 degrees
		//   x
		// y 0  1  2  3			..Y.
		//   4  5  6  7     ->  ..Y. -> the index for each of the Y's in the I piece here are
		//   8  9  10 11		..Y.    2, 6, 10, 14 respectivly
		//   12 13 14 15		..Y.
		case 0:
			piece_index = x + (4 * y);
			break;

			// 90 degrees
		case 1:
			piece_index = 12 + y - (4 * x);
			break;

			// 180 degrees
		case 2:
			piece_index = 15 - (4 * y) - x;
			break;

			// 270 degrees
		case 3:
			piece_index = 3 - y + (4 * x);
			break;
	}

	// index of the current piece x and y based on it's rotation
	return piece_index;

}
