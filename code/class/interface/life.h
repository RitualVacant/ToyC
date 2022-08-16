#ifndef LIFE_H
#define LIFE_H

#pragma once
#include <algorithm>
#include <string>
#include <vector>

struct var_life {
  std::size_t start;
  std::size_t end;
  std::string name;

  var_life(std::string name_, std::size_t start_, std::size_t end_)
      : name{name_}, start{start_}, end{end_} {};
  bool operator<(var_life const x) const {
    // TODO 先后时间， 不仅仅只考虑结束时间
    return start < x.start;
  }
};

class life {
private:
  std::vector<var_life> var_life_list;

public:
  void push(var_life);
  void clear();
  void sort();
  void loc();
};

#endif