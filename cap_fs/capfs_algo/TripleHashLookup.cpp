#include <TripleHashLookup.hpp>
namespace capfs {
  TripleHashLookup::TripleHashLookup(std::string salt):mSalt(salt){}
  TripleHashParentChild TripleHashLookup::operator[](std::string path) {
     return TripleHashParentChild(mSalt,path);
  }
}
