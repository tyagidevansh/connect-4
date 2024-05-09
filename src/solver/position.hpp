#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <cstdint>

namespace GameSolver { namespace Connect4 {

  constexpr static uint64_t bottom(int width, int height) {
    return width == 0 ? 0 : bottom(width-1, height) | 1LL << (width-1)*(height+1);
  }


  class Position {
    public:

      static const int WIDTH = 7;  
      static const int HEIGHT = 6; 
      static const int MIN_SCORE = -(WIDTH*HEIGHT)/2 + 3;
      static const int MAX_SCORE = (WIDTH*HEIGHT+1)/2 - 3;

      static_assert(WIDTH < 10, "Board's width must be less than 10");
      static_assert(WIDTH*(HEIGHT+1) <= 64, "Board does not fit in 64bits bitboard");

      void play(uint64_t move)
      {
        current_position ^= mask;
        mask |= move;
        moves++;
      }

      unsigned int play(std::string seq)
      {
        for(unsigned int i = 0; i < seq.size(); i++) {
          int col = seq[i] - '1';
          if(col < 0 || col >= Position::WIDTH || !canPlay(col) || isWinningMove(col)) return i; // invalid move
          playCol(col);
        }
        return seq.size();
      }

      bool canWinNext() const
      {
        return winning_position() & possible();
      }

      int nbMoves() const
      {
        return moves;
      }

      uint64_t key() const
      {
        return current_position + mask;
      }

      uint64_t possibleNonLosingMoves() const {
        assert(!canWinNext());
        uint64_t possible_mask = possible();
        uint64_t opponent_win = opponent_winning_position();
        uint64_t forced_moves = possible_mask & opponent_win;
        if(forced_moves) {
          if(forced_moves & (forced_moves - 1)) // check if there is more than one forced move
            return 0;                           // the opponnent has two winning moves and you cannot stop him
          else possible_mask = forced_moves;    // enforce to play the single forced move
        }
        return possible_mask & ~(opponent_win >> 1);  // avoid to play below an opponent winning spot
      }


      int moveScore(uint64_t move) const {
        return popcount(compute_winning_position(current_position | move, mask));
      }


      Position() : current_position{0}, mask{0}, moves{0} {}

    private:
      uint64_t current_position; // bitmap of the current_player stones
      uint64_t mask;             // bitmap of all the already palyed spots
      unsigned int moves;        // number of moves played since the beinning of the game.

      bool canPlay(int col) const
      {
        return (mask & top_mask_col(col)) == 0;
      }

      void playCol(int col)
      {
        play((mask + bottom_mask_col(col)) & column_mask(col));
      }


      bool isWinningMove(int col) const
      {
        return winning_position() & possible() & column_mask(col);
      }

      uint64_t winning_position() const {
        return compute_winning_position(current_position, mask);
      }

      uint64_t opponent_winning_position() const {
        return compute_winning_position(current_position ^ mask, mask);
      }

      uint64_t possible() const {
        return (mask + bottom_mask) & board_mask;
      }


      static unsigned int popcount(uint64_t m) {
        unsigned int c = 0; 
        for (c = 0; m; c++) m &= m - 1;
        return c;
      }


      static uint64_t compute_winning_position(uint64_t position, uint64_t mask) {
        // vertical;
        uint64_t r = (position << 1) & (position << 2) & (position << 3);

        //horizontal
        uint64_t p = (position << (HEIGHT+1)) & (position << 2*(HEIGHT+1));
        r |= p & (position << 3*(HEIGHT+1));
        r |= p & (position >> (HEIGHT+1));
        p = (position >> (HEIGHT+1)) & (position >> 2*(HEIGHT+1));
        r |= p & (position << (HEIGHT+1));
        r |= p & (position >> 3*(HEIGHT+1));

        //diagonal 1
        p = (position << HEIGHT) & (position << 2*HEIGHT);
        r |= p & (position << 3*HEIGHT);
        r |= p & (position >> HEIGHT);
        p = (position >> HEIGHT) & (position >> 2*HEIGHT);
        r |= p & (position << HEIGHT);
        r |= p & (position >> 3*HEIGHT);

        //diagonal 2
        p = (position << (HEIGHT+2)) & (position << 2*(HEIGHT+2));
        r |= p & (position << 3*(HEIGHT+2));
        r |= p & (position >> (HEIGHT+2));
        p = (position >> (HEIGHT+2)) & (position >> 2*(HEIGHT+2));
        r |= p & (position << (HEIGHT+2));
        r |= p & (position >> 3*(HEIGHT+2));

        return r & (board_mask ^ mask);
      }

      // Static bitmaps

      const static uint64_t bottom_mask = bottom(WIDTH, HEIGHT);
      const static uint64_t board_mask = bottom_mask * ((1LL << HEIGHT)-1);

      // return a bitmask containg a single 1 corresponding to the top cel of a given column
      static constexpr uint64_t top_mask_col(int col) {
        return UINT64_C(1) << ((HEIGHT - 1) + col*(HEIGHT+1));
      }

      // return a bitmask containg a single 1 corresponding to the bottom cell of a given column
      static constexpr uint64_t bottom_mask_col(int col) {
        return UINT64_C(1) << col*(HEIGHT+1);
      }

    public:
      // return a bitmask 1 on all the cells of a given column
      static constexpr uint64_t column_mask(int col) {
        return ((UINT64_C(1) << HEIGHT)-1) << col*(HEIGHT+1);
      }
  };

}} // end namespaces

#endif
