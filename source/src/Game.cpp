// Game.cpp : implementation of the game class used
// to implement Tetris

// libs
#include "Game.hpp"

Game::Game() {

	// console screem buffer information to get the console default
	// width and height
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	// get the buffer info
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	// get the sreen and height and store them in the class
	screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	// initialize booleans used for controls
	is_A_pressed = false;
	is_S_pressed = false;
	is_D_pressed = false;
	is_R_pressed = false;
	is_ESC_pressed = false;

	// initialize boolean for game completion
	isGameOver = false;

	// create the screen buffer and playing field
	createScreenBuffer();
	createTetrisFieldBuffer();
}

void Game::run(void)
{
	// initial values before game starts
	int curr_tetromino = rand() % tetrominos.num_pieces;
	int curr_rotation = 0;
	int curr_x = (TetrisFieldWidth - 1) / 2;
	int curr_y = 0;
	bool isPlayerHoldingRotation = false;
	int current_speed = 25;
	int speed_counter = 0;
	bool movePieceDown = false;
	int score = 0;


	// main game loop
	// runs while the game over boolean is not true
	while (!isGameOver) {

		// game timing
		// everything moves cell by cell
		// we can emulate that with a simple sleep command
		std::this_thread::sleep_for(std::chrono::milliseconds(game_tick));

		// each game tick increment speed counter
		speed_counter++;

		// check if the speed counter is the same as the speed and then make the game run faster
		movePieceDown = current_speed == speed_counter;

		// input handling
		handle_input();

		// logic handling
		handle_logic(&curr_tetromino, &curr_rotation, &curr_x, &curr_y, &isPlayerHoldingRotation, &movePieceDown, &isGameOver, &speed_counter, &score);

		// diplaying graphics
		draw(curr_tetromino, curr_rotation, curr_x, curr_y, score);
	}

	CloseHandle(console_buffer);
	std::cout << "Game over, Score: " << score << std::endl;
	system("pause");

	return;
}

void Game::createScreenBuffer(void) {
	
	// allocate memory for screen array of wchars
	screen = new wchar_t[screenWidth * screenHeight];

	// fill with spaces
	for (int i = 0; i < screenWidth * screenHeight; i++) screen[i] = L' ';

	// create the console buffer
	console_buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// set active console buffer to the one created above
	SetConsoleActiveScreenBuffer(console_buffer);

	// initialize written bytes variable
	written_bytes = 0;

	return;
}

void Game::createTetrisFieldBuffer(void) {
	
	// allocate memory for the tertis play field
	// creates an array which we will manipulate as a 2d x, y valued array
	tetris_field = new unsigned char[TetrisFieldWidth * TetrisFieldHeight];

	// initialize the play field, since we have 7 tetromino pieces one 
	// character for the space, one for the field border and one for
	// the tetris line when it occurs we're going to initialize
	//  the "walls" with 9 and the inside with 0, representing
	// the wall character and the empty space character respectivly
	// let's say we have a play field of 4 * 4 built as the example below
	//   x
	// y 0  1  2  3	
	//   4  5  6  7 
	//   8  9  10 11
	//   12 13 14 15
	// to get to the cell containing 10 we can translate the x and y values
	// of the array, for x = 2, y = 2 we can get 10 with this equation
	// value = y * width of array + x -> 2 * 4 + 2 = 10
	for (int x = 0; x < TetrisFieldWidth; x++) {
		for (int y = 0; y < TetrisFieldHeight; y++) {

			// if we're in the field's wall, i.e. x == 0, x == width - 1,
			// y == height - 1 set it to 9 else set it to 0
			tetris_field[y * TetrisFieldWidth + x] = (x == 0 || x == TetrisFieldWidth - 1 || y == TetrisFieldHeight - 1) ? 9 : 0;
		}
	}

	return;
}

void Game::draw(int curr_tetromino, int curr_rotation, int curr_x, int curr_y, int score)
{
	// first we format the screen field
	formatField(curr_tetromino, curr_rotation, curr_x, curr_y);

	// draw the score of the game
	swprintf_s(&screen[screenWidth + TetrisFieldWidth + 6], 16, L"Score: %8d", score);

	// check if there are lines
	if (!lines.empty()) {

		// draw frame in order to make the lines appear to the player before final drawing
		WriteConsoleOutputCharacter(console_buffer, screen, screenWidth * screenHeight, { 0,0 }, &written_bytes);

		// sleep for a little bit of time to make the player see the lines
		std::this_thread::sleep_for(std::chrono::milliseconds(line_delay));

		// for every line completed in the playing field move all the pieces one cell
		// below and remove the '=' symbol since line is not present anymore
		for (int line_y : lines) {

			// for every column
			for (int x = 1; x < TetrisFieldWidth - 1; x++) {

				// make every piece fall down one cell, this is done top down
				for (int y = line_y; y > 0; y--) {
					tetris_field[y * TetrisFieldWidth + x] = tetris_field[(y - 1) * TetrisFieldWidth + x];
				}

				// clear the x'th cell
				tetris_field[x] = 0;
			}

		}

		// empty lines vector
		lines.clear();
	}

	// draw the frame
	WriteConsoleOutputCharacter(console_buffer, screen, screenWidth * screenHeight, { 0,0 }, &written_bytes);

	return;
}

void Game::formatField(int curr_tetromino, int curr_rotation, int curr_x, int curr_y) {
	
	// using the field's array value to print one of the following characters
	// A B C D E F G using the first 7 seven letters for tetrominoes, = for tetris line, 
	// # for walls
	
	for (int x = 0; x < TetrisFieldWidth; x++) {
		for (int y = 0; y < TetrisFieldHeight; y++) {

			// using the same notation as for the initialization
			// offesting by 5 so it won't print on the top left corner of the screen
			// also using the tetris field value as an index in the drawing chars to output
			// specified chars contained in that array
			screen[(y + 2) * screenWidth + (x + 2)] = drawing_chars[tetris_field[y * TetrisFieldWidth + x]];
		}
	}

	// draw current piece
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			// checking for the piece blocks that are going to be on the field, these are the ones with no '.'
			if (tetrominos.pieces[curr_tetromino][tetrominos.rotation_index(x, y, curr_rotation)] != L'.')

				// setting the current cell of the screen buffer to the current tetromino plus the ascii value of 'A'
				screen[(curr_y + y + 2) * screenWidth + (curr_x + x + 2)] = curr_tetromino + 'A';
		}

	}

	return;
}

void Game::handle_input(void)
{	
	// check if escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE)) {
		int result = MessageBox(NULL, L"Do you want to exit the game?", L"Exit Game", MB_YESNOCANCEL);

		switch (result)
		{
			// change game over boolean to true
			// only if the user presses yes
			case IDYES:
				isGameOver = true;
				return;
			case IDNO:
			case IDCANCEL:
			default:
				break;
		}
	}

	// reset the booleans
	// and check for input again
	is_A_pressed = false;
	is_S_pressed = false;
	is_D_pressed = false;
	is_R_pressed = false;
	
	// check for a, s, d keys respectivly
	if (GetAsyncKeyState('A') & 0x8000) is_A_pressed = true;
	if (GetAsyncKeyState('S') & 0x8000) is_S_pressed = true;
	if (GetAsyncKeyState('D') & 0x8000) is_D_pressed = true;
	if (GetAsyncKeyState('R') & 0x8000) is_R_pressed = true;

	return;
}

bool Game::doesPieceFit(int pieceID, int curr_x, int curr_y, int curr_rot) {


	// using 4 because every tetromino is represented by a 4*4 array
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			// get the index into the piece using the current rotation
			int piece_index = tetrominos.rotation_index(x, y, curr_rot);

			// get the index into the field using the y * width + x notation
			// but with an offset of curr_x and curr_y 
			int field_index = (y + curr_y) * TetrisFieldWidth + (x + curr_x);


			// we check if the piece is in bounds of the playing field
			if (x + curr_x >= 0 && x + curr_x < TetrisFieldWidth) {
				if (y + curr_y >= 0 && y + curr_y < TetrisFieldHeight) {
					// if the character stored in the piece index of the current piece is not a dot
					// or if the number stored on playing field cell field_index is not 0 then the
					// piece does not fit
					if (tetrominos.pieces[pieceID][piece_index] != L'.' && tetris_field[field_index] != 0) {

						// no need to check every cell if one of them does not fit
						return false;
					}
				}
			}

		}
	}

	// all good, piece fits
	return true;
}

void Game::handle_logic(int *curr_tetromino, int *curr_rotation, int *curr_x, int *curr_y, bool *isPlayerHoldingRotation, bool *movePieceDown, bool *isGameOver, int *speed_counter, int *score)
{
	// check for user input
	// move left right or down, if key is pressed and piece fits
	*curr_x += (is_D_pressed && doesPieceFit(*curr_tetromino, *curr_x + 1, *curr_y, *curr_rotation)) ? 1 : 0;
	*curr_x -= (is_A_pressed && doesPieceFit(*curr_tetromino, *curr_x - 1, *curr_y, *curr_rotation)) ? 1 : 0;
	*curr_y += (is_S_pressed && doesPieceFit(*curr_tetromino, *curr_x, *curr_y + 1, *curr_rotation)) ? 1 : 0;

	// change rotation of the piece
	if (is_R_pressed) {
		
		// using a boolean to check if the player is holding the rotation button in order make one rotation per press
		// without caring of the duration of the press
		*curr_rotation += (!(*isPlayerHoldingRotation) && doesPieceFit(*curr_tetromino, *curr_x, *curr_y, *curr_rotation + 1)) ? 1 : 0;
		*isPlayerHoldingRotation = true;
	}
	else *isPlayerHoldingRotation = false;

	// time to check if we have to move the piece down
	if (*movePieceDown) {
		
		// check if the next y value is in bounds and if yes move the piece
		if (doesPieceFit(*curr_tetromino, *curr_x, *curr_y + 1, *curr_rotation)) {
			*curr_y = *curr_y + 1;
		}
		else {

			// we have to lock the piece in place
			// lock piece in place
			for (int x = 0; x < 4; x++) {
				
				for (int y = 0; y < 4; y++) {

					if (tetrominos.pieces[*curr_tetromino][tetrominos.rotation_index(x, y, *curr_rotation)] != L'.') {
						
						tetris_field[(*curr_y + y) * TetrisFieldWidth + (*curr_x + x)] = *curr_tetromino + 1;
					}
				}
			}

			// check if lines are present
			// only check the cells where the last tetromino piece
			// fell, no point to check the whole playing field
			// since a line can only be formed were the last piece
			// fell
			for (int y = 0; y < 4; y++) {

				// check if we're in bounds
				if (*curr_y + y < TetrisFieldHeight - 1) {

					// boolean to know if there are lines or not
					// we asume we have formed lines
					bool linesExist = true;

					// for every x value of the playing field in the current y
					// we check if the value stored is not 0
					// skipping first and last, these are the walls
					for (int x = 1; x < TetrisFieldWidth - 1; x++) {
						linesExist &= (tetris_field[(*curr_y + y) * TetrisFieldWidth + x] != 0);
					}

					if (linesExist) {

						// set every character in that line to be equal to this '='
						// indicating a line is present
						for (int x = 1; x < TetrisFieldWidth - 1; x++) {

							// setting it to zero since in this array wchar_t drawing_chars[11] = L" ABCDEFG=#";
							// this character, '=', is in the 8th cell
							tetris_field[(*curr_y + y) * TetrisFieldWidth + x] = 8;
						}

						lines.push_back(*curr_y + y);
					}

				}

			}

			// placed a new tetromino, increase score
			*score += 20;

			// check if there are lines
			if (!lines.empty()) {

				// get the amount of lines in the vector
				// and give the player the correct amount of points
				switch (lines.size()) {

					// one line
					case 1:
						*score += 40;
						break;
						
					// two lines
					case 2:
						*score += 100;
						break;

					// three lines
					case 3:
						*score += 300;
						break;

					// tetris made
					case 4:
						*score += 1200;
						break;
				}
			}

			// generate new piece
			*curr_tetromino = rand() % tetrominos.num_pieces;
			*curr_rotation = 0;
			*curr_x = (TetrisFieldWidth - 1) / 2;
			*curr_y = 0;

			// check if the new piece does not fit and if yes the game is over
			*isGameOver = !doesPieceFit(*curr_tetromino, *curr_x, *curr_y, *curr_rotation);

		}

		*speed_counter = 0;
		*movePieceDown = false;
	}

	return;
}

void Game::exit() {

	// free allocated memory for every field
	OutputDebugString(L"Exit game, deleting pointers created with new keyword\n");
	delete tetris_field;
	delete screen;
}