#include <cassert>
#include "position.hpp"

using namespace GameSolver::Connect4;

namespace GameSolver { namespace Connect4 {
    class Solver {
        private:
        unsigned long long nodeCount;

        int negamax(const Position &P, int alpha, int beta) {
            assert(alpha < beta);
            nodeCount++;

            //check for a draw
            if (P.nbMoves() == Position::WIDTH*Position::HEIGHT)
                return 0;

            for (int x = 0; x < Position::WIDTH; x++)
                if (P.canPlay(x) && P.isWinningMove(x))
                    return (Position::WIDTH*Position::HEIGHT+1 - P.nbMoves())/2;

            int max = (Position::WIDTH*Position::HEIGHT - 1 - P.nbMoves()) / 2;
            if (beta > max) {
                beta = max;
                if (alpha >= beta) return beta;
            }

            for (int x = 0; x < Position::WIDTH; x++)
                if (P.canPlay(x)) {
                    Position P2(P);
                    P2.play(x);
                    int score = -negamax(P2, -beta, -alpha);

                    if (score >= beta) return score;
                    if (score > alpha) alpha = score;
                }

            return alpha;
        }

        public:

        int solve (const Position &P, bool weak = false) {
            nodeCount = 0;
            if(weak)
                return negamax(P, -1, 1);
            else
                return negamax(P, -Position::WIDTH*Position::HEIGHT/2, Position::WIDTH*Position::HEIGHT/2);
        }  

        unsigned long long getNodeCount() {
            return nodeCount;
        }
    };
}}

#include <sys/time.h>
unsigned long long getTimeMicrosec() {
    timeval NOW;
    gettimeofday(&NOW, NULL);
    return NOW.tv_sec*1000000LL + NOW.tv_usec;    
}

#include <iostream>
int main(int argc, char** argv) {
    Solver solver;

    bool weak = false;
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'w') weak = true;
    std::string line;

    for(int l = 1; std::getline(std::cin, line); l++) {
        Position P;
        if(P.play(line) != line.size())
        {
        std::cerr << "Line << " << l << ": Invalid move " << (P.nbMoves()+1) << " \"" << line << "\"" << std::endl;
        }
        else
        {
        unsigned long long start_time = getTimeMicrosec();
        int score = solver.solve(P, weak);
        unsigned long long end_time = getTimeMicrosec();
        std::cout << line << " " << score << " " << solver.getNodeCount() << " " << (end_time - start_time);
        }
        std::cout << std::endl;
    }
}