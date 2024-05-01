#include "position.hpp"

using namespace GameSolver::Connect4;

namespace GameSolver { namespace Connect4 {
    class Solver {
        private:
        unsigned long long nodeCount;

        int negamax(const Position &P) {
            nodeCount++;
            
            //check for draw
            if (P.nbMoves() == Position::WIDTH*Position::HEIGHT)
                return 0;
            
            for (int x = 0; x < Position::WIDTH; x++) 
                if (P.canPlay(x) && P.isWinningMove(x))
                    return (Position::WIDTH*Position::HEIGHT + 1 - P.nbMoves())/2;
            
            int bestScore = -Position::WIDTH*Position::HEIGHT;

            for(int x = 0; x < Position::WIDTH; x++) 
                if (P.canPlay(x)) {
                    Position P2(P);
                    P2.play(x);
                    int score = -negamax(P2); //assuming opponents score to be the negative of our score
                    if (score > bestScore) bestScore = score;
                }

            return bestScore;
        }

        public:

        int solve(const Position &P) {
            nodeCount = 0;
            return negamax(P);
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
int main() {
    Solver solver;

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
        int score = solver.solve(P);
        unsigned long long end_time = getTimeMicrosec();
        std::cout << line << " " << score << " " << solver.getNodeCount() << " " << (end_time - start_time);
        }
        std::cout << std::endl;
    }
}