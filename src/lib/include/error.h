#ifndef ERROR_H
#define ERROR_H

#include <string>

namespace no_engine_game { namespace lib {

class Error {
public:
  Error(std::string message): message_(message) {}
  std::string getMessage() const {
    return message_;
  }

private:
  std::string message_;
};

}}

#endif
