#include <cassert>
#include "position.hpp"
#include "transpositionTable.hpp"
#include <utility>

using namespace GameSolver::Connect4;

namespace GameSolver { namespace Connect4 {
    class Solver {
        private:
            unsigned long long nodeCount;
            int bestMove;

            int columnOrder[Position::WIDTH];
            TranspositionTable transTable;

            int negamax(const Position &P, int alpha, int beta, int depth) {
                assert(alpha < beta);  // Score window
                nodeCount++; // Nodes explored

                if (depth == 0 || P.nbMoves() == Position::WIDTH * Position::HEIGHT) {
                    bestMove = 0;
                    return 0;
                }

                // Check for winning move
                for (int x = 0; x < Position::WIDTH; x++)
                    if (P.canPlay(x) && P.isWinningMove(x)) {
                        bestMove = x;
                        return (Position::WIDTH * Position::HEIGHT + 1 - P.nbMoves()) / 2; 
                    }

                // Calculate maximum possible score achievable in the current state
                int max = (Position::WIDTH * Position::HEIGHT - 1 - P.nbMoves()) / 2;
                if (int val = transTable.get(P.key()))
                    max = val + Position::MIN_SCORE - 1;

                if (beta > max) { // Update beta if necessary
                    beta = max;
                    if (alpha >= beta) {
                        return beta; // Prune
                    }
                }

                int bestScore = -max; // Initialize bestScore to the lowest possible value
                for (int x = 0; x < Position::WIDTH; x++) {
                    if (P.canPlay(columnOrder[x])) {
                        Position P2(P);
                        P2.play(columnOrder[x]);
                        int score = -negamax(P2, -beta, -alpha, depth - 1); // Decrement depth here

                        if (score >= beta) {
                            bestMove = x; // Update bestMove here
                            return score;
                        }
                        if (score > bestScore) {
                            bestScore = score; // Update bestScore
                            bestMove = x; // Update bestMove
                        }
                        alpha = std::max(alpha, score); // Update alpha
                    }
                }
                transTable.put(P.key(), alpha - Position::MIN_SCORE + 1);
                //bestscore?
                return alpha;
            }

        public:

            void reset() {
                nodeCount = 0;
                transTable.reset();
            }
            Solver() : nodeCount{0}, transTable(8388593) {
                reset();
                for (int i = 0; i < Position::WIDTH; i++) {
                    columnOrder[i] = Position::WIDTH/2 + (1-2*(i%2))*(i+1)/2;
                }
            }

            int solve(const Position &P, int depth, bool weak = false) { // Add depth parameter to solve
                int min = -(Position::WIDTH*Position::HEIGHT - P.nbMoves())/2;
                int max = (Position::WIDTH*Position::HEIGHT+1 - P.nbMoves())/2;

                if (weak) {
                    min = -1;
                    max = 1;
                }

                while (min < max) {
                    int med = min + (max - min)/2;
                    if (med <= 0 && max/2 > med) med = min/2;
                    else if (med >= 0 && max/2 > med) med = max/2;
                    int r = negamax(P, med, med + 1, depth);
                    if (r <= med) max = r;
                    else min = r;
                }
                return min;
            }

            unsigned long long getNodeCount() const { // Add const to getNodeCount
                return nodeCount;
            }

            int getBestMove() const { // Add getter for bestMove
                return bestMove;
            }
        };
}}
