// Game.hpp : class defition of the game engine
// include guard
#pragma once

// libs used
#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include "Tetrominos.hpp"

class Game {
	
	public :

		// constants for screen and play field
		int screenWidth;
		int screenHeight;

		const int TetrisFieldWidth = 12;
		const int TetrisFieldHeight = 18;

		// game tick
		const int game_tick = 50;
		const int line_delay = 400;

		// screen buffer related
		wchar_t* screen;

		// console buffer
		HANDLE console_buffer;

		// bytes writen on screen
		DWORD written_bytes;

		// tetris play field buffer
		unsigned char* tetris_field;

		// const wchar_t array for the characters
		// for drawing
		wchar_t drawing_chars[11] = L" ABCDEFG=#";

		// controls pressed
		bool is_A_pressed;
		bool is_S_pressed;
		bool is_D_pressed;
		bool is_R_pressed;
		bool is_ESC_pressed;

		// vector to store how many lines are completed
		std::vector<int> lines;

		// boolean to keep track of game ending
		bool isGameOver;

		// Tetrominos class
		Tetrominos tetrominos;

		// constructor
		Game();

		// screen buffer
		void createScreenBuffer(void);

		// tetris field
		void createTetrisFieldBuffer(void);

		// game timing ticks
		void game_timing(void);

		// drawing
		void draw(int curr_tetromino, int curr_rotation, int curr_x, int curr_y, int score);

		void formatField(int curr_tetromino, int curr_rotation, int curr_x, int curr_y);

		// run game loop
		void run(void);

		// handle user input
		void handle_input(void);

		// handle game logic
		void handle_logic(int* curr_tetromino, int *curr_rotation, int *curr_x, int *curr_y, bool *isPlayerHoldingRotation, bool *movePieceDown, bool *isGameOver, int *speed_counter, int *score);

		// checks if a piece fits in playing field or not
		bool doesPieceFit(int pieceID, int curr_x, int curr_y, int curr_rot);

		// void cleans up game and allocated sources
		void exit();

};
