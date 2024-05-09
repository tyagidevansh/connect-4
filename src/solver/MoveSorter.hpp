#ifndef MOVE_SORTER_HPP
#define MOVE_SORTER_HPP

#include "position.hpp"

namespace GameSolver { namespace Connect4 {

  class MoveSorter {
  public:

    void add(uint64_t move, int score) 
    {
      int pos = size++;
      for(; pos && entries[pos-1].score > score; --pos) entries[pos] = entries[pos-1];
      entries[pos].move = move;
      entries[pos].score = score;
    }

    uint64_t getNext() 
    {
      if(size) 
        return entries[--size].move;
      else 
        return 0;
    }

    void reset()
    {
      size = 0;
    }

    MoveSorter(): size{0} 
    {
    }

  private:

    unsigned int size;


    struct {uint64_t move; int score;} entries[Position::WIDTH];   
  };

}};

#endif
