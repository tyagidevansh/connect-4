#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <cstdint>

namespace GameSolver { namespace Connect4 {

    class Position {
        public:
            static const int WIDTH = 7;
            static const int HEIGHT = 6;
            static_assert(WIDTH < 10, "Board's width must be less than 10");

            bool canPlay(int col) const {
                return (mask & top_mask(col)) == 0;
            } 

            void play(int col) {
                current_position ^= mask;
                mask |= mask + bottom_mask(col);
                moves++;
            }

            unsigned int play(std::string seq) {
                for ( unsigned int i = 0; i < seq.size(); i++) {
                    int col = seq[i] - '1';
                    if (col < 0 || col >= Position::WIDTH || !canPlay(col) || isWinningMove(col)) return i;
                    play(col);
                }
                return seq.size();
            }

            bool isWinningMove(int col) const {
                uint64_t pos = current_position;
                pos |= (mask + bottom_mask(col)) & column_mask(col);
                return alignment(pos);
            }

            unsigned int nbMoves() const {
                return moves;
            }

            uint64_t key() const {
                return current_position + mask;
            }

            Position() : current_position{0}, mask{0}, moves{0} {}
        
        private:
            uint64_t current_position;
            uint64_t mask;
            unsigned int moves; //number of moves

            static bool alignment(uint64_t pos) {
                //horizontal
                uint64_t m = pos & (pos >> (HEIGHT + 1));
                if (m & (m >> (2*(HEIGHT+1)))) return true;

                //diagonal 1
                m = pos & (pos >> HEIGHT);
                if (m & (m >> (2*HEIGHT))) return true;

                //diagonal 2
                m = pos & (pos >> (HEIGHT + 2));
                if (m & (m >> (2*(HEIGHT+2)))) return true;

                //vertical
                m = pos & (pos >> 1);
                if (m & (m >> 2)) return true;

                return false;
            }

            static uint64_t top_mask(int col) {
                return (UINT64_C(1) << (HEIGHT - 1) << col*(HEIGHT+1));
            }

            static uint64_t bottom_mask (int col) {
                return UINT64_C(1) << col * (HEIGHT+1);
            }

            static uint64_t column_mask(int col) {
                return ((UINT64_C(1) << HEIGHT) - 1) << col * (HEIGHT + 1);
            }
    };
}}

#endif