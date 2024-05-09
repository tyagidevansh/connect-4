#include <cassert>
#include <cstdlib>
#include "position.hpp"
#include "TranspositionTable.hpp"
#include "MoveSorter.hpp"

using namespace GameSolver::Connect4;

namespace GameSolver { namespace Connect4 {

  class Solver {
  private:

    unsigned long long nodeCount; // counter of explored nodes.
    int bestMove;

    int columnOrder[Position::WIDTH]; // column exploration order

    TranspositionTable transTable;

    int getRandomScore() {
      std::srand(std::time(nullptr));
      int random = std::rand() % 7;
      return random;
    }

    int negamax(const Position &P, int alpha, int beta) {
      assert(alpha < beta);
      assert(!P.canWinNext());

      nodeCount++; // increment counter of explored nodes

      uint64_t next = P.possibleNonLosingMoves();
      if(next == 0) {   // if no possible non losing move, opponent wins next move
        bestMove = getRandomScore();
        return -(Position::WIDTH*Position::HEIGHT - P.nbMoves())/2;
      }

      if(P.nbMoves() >= Position::WIDTH*Position::HEIGHT - 2) // check for draw game
        return 0; 

      int min = -(Position::WIDTH*Position::HEIGHT-2 - P.nbMoves())/2;	// lower bound of score as opponent cannot win next move
      if(alpha < min) {
        alpha = min;                     // there is no need to keep beta above our max possible score.
        if(alpha >= beta) return alpha;  // prune the exploration if the [alpha;beta] window is empty.
      }

      int max = (Position::WIDTH*Position::HEIGHT-1 - P.nbMoves())/2;	// upper bound of our score as we cannot win immediately
      if(int val = transTable.get(P.key()))
        max = val + Position::MIN_SCORE - 1;

      if(beta > max) {
        beta = max;                     // there is no need to keep beta above our max possible score.
        if(alpha >= beta) return beta;  // prune the exploration if the [alpha;beta] window is empty.
      }


      MoveSorter moves;

      for(int i = Position::WIDTH; i--; )
         if(uint64_t move = next & Position::column_mask(columnOrder[i]))
           moves.add(move, P.moveScore(move));

      while(uint64_t next = moves.getNext()) {
          Position P2(P);
          P2.play(next);  // It's opponent turn in P2 position after current player plays x column.
          int score = -negamax(P2, -beta, -alpha); // explore opponent's score within [-beta;-alpha] windows:
          // no need to have good precision for score better than beta (opponent's score worse than -beta)
          // no need to check for score worse than alpha (opponent's score worse better than -alpha)
          bestMove = next;
          if(score >= beta) return score;  // prune the exploration if we find a possible move better than what we were looking for.
          if(score > alpha) alpha = score; // reduce the [alpha;beta] window for next exploration, as we only 
          // need to search for a position that is better than the best so far.
        }

      transTable.put(P.key(), alpha - Position::MIN_SCORE + 1); // save the upper bound of the position
      return alpha;
    }

  public:

    int solve(const Position &P, bool weak = false) 
    {
      if(P.canWinNext()) // check if win in one move as the Negamax function does not support this case.
        return (Position::WIDTH*Position::HEIGHT+1 - P.nbMoves())/2;
      int min = -(Position::WIDTH*Position::HEIGHT - P.nbMoves())/2;
      int max = (Position::WIDTH*Position::HEIGHT+1 - P.nbMoves())/2;
      if(weak) {
        min = -1;
        max = 1;
      }

      while(min < max) {                    // iteratively narrow the min-max exploration window
        int med = min + (max - min)/2;
        if(med <= 0 && min/2 < med) med = min/2;
        else if(med >= 0 && max/2 > med) med = max/2;
        int r = negamax(P, med, med + 1);   // use a null depth window to know if the actual score is greater or smaller than med
        if(r <= med) max = r;
        else min = r;
      }
      return min;
    }

    unsigned long long getNodeCount() 
    {
      return nodeCount;
    }

    int getBestMove() const { 
       return bestMove;
   }

    void reset() 
    {
      nodeCount = 0;
      transTable.reset();
    }

    // Constructor
    Solver() : nodeCount{0}, transTable(8388593) { //8388593 prime = 64MB of transposition table
      reset();
      for(int i = 0; i < Position::WIDTH; i++)
        columnOrder[i] = Position::WIDTH/2 + (1-2*(i%2))*(i+1)/2;   
      // initialize the column exploration order, starting with center columns
    }

  };
}} // namespace GameSolver::Connect4