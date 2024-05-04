#include <cassert>
#include "position.hpp"
#include <utility>

using namespace GameSolver::Connect4;

namespace GameSolver { namespace Connect4 {
    class Solver {
        private:
            unsigned long long nodeCount;
            int bestMove;

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

                if (beta > max) { // Update beta if necessary
                    beta = max;
                    if (alpha >= beta) {
                        return beta; // Prune
                    }
                }

                int bestScore = -max; // Initialize bestScore to the lowest possible value
                for (int x = 0; x < Position::WIDTH; x++) {
                    if (P.canPlay(x)) {
                        Position P2(P);
                        P2.play(x);
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
                return bestScore;
            }

        public:
            Solver() : nodeCount(0), bestMove(-1) {} // Initialize bestMove in the constructor

            int solve(const Position &P, int depth, bool weak = false) { // Add depth parameter to solve
                nodeCount = 0;
                if (weak)
                    return negamax(P, -1, 1, depth); // Weak solver
                else
                    return negamax(P, -Position::WIDTH * Position::HEIGHT / 2, Position::WIDTH * Position::HEIGHT / 2, depth); // Strong solver
            }

            unsigned long long getNodeCount() const { // Add const to getNodeCount
                return nodeCount;
            }

            int getBestMove() const { // Add getter for bestMove
                return bestMove;
            }
        };
}}
