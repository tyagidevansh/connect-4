#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <iostream>

struct moveScore {
    int move;
    int score;
};

namespace GameSolver {namespace Connect4 {
    class Position {
        public:
            static const int WIDTH = 7;
            static const int HEIGHT = 6;
            //static const int MOVES_SEARCHED = 10;
            static_assert(WIDTH < 10, "Board's width must be less than 10");

            bool canPlay(int col) const {
                return height[col] < HEIGHT; 
            }

            void play(int col) {
                board[col][height[col]] = 1 + moves % 2;
                height[col]++;
                moves++;
            }

            //initialize board
            unsigned int play(std::string seq) {
                for (unsigned int i = 0; i < seq.size(); i++) {
                    int col = seq[i] - '1';
                    if (col < 0 || col >= Position::WIDTH || !canPlay(col) || isWinningMove(col)) return i;
                    play(col);
                }
                return seq.size();
            }

            bool isWinningMove(int col) const {
                int current_player = 1 + moves % 2;

                if (height[col] >= 3
                    && board[col][height[col]-1] == current_player
                    && board[col][height[col]-2] == current_player
                    && board[col][height[col]-3] == current_player)
                    return true;
                
                for (int dy = -1; dy <= 1; dy++) {
                    int nb = 0;
                    for(int dx = -1; dx <= 1; dx += 1)
                        for (int x = col + dx, y = height[col] + dx * dy; x >= 0 && x < WIDTH && y < HEIGHT && board[x][y] == current_player; nb++) {
                            x += dx;
                            y += dx * dy;
                        }
                    if (nb >= 3) return true;
                }
                return false;
            }

            unsigned int nbMoves() const  {
                return moves;
            }

            Position() : board{0}, height{0}, moves{0} {}
        
        private:
            int board[WIDTH][HEIGHT];
            int height[WIDTH];
            unsigned int moves;
    };
}}

#endif // !POSITION_HPP
