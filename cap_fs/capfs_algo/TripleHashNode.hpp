#ifndef MINORFS_CAPFS_TRIPLEHASHNODE_HPP
#define MINORFS_CAPFS_TRIPLEHASHNODE_HPP
#include <string>
namespace capfs {
class TripleHashNode;
class TripleHashNode {
     unsigned char mKey1[35];
     unsigned char mKey2[35];     
     mutable unsigned char mKey3[35];
     std::string mSalt;
     bool mVKey1;
     bool mVKey3;
     TripleHashNode(std::string nodename,TripleHashNode const * parent);
     void derivekey(unsigned char const *inkey,std::string instring,std::string salt,unsigned char *) const;
     std::string treepath(std::string) const;
   public:
     TripleHashNode(std::string secretsalt,std::string b32cap);
     bool canWrite() const;
     std::string rwcap() const;
     std::string rocap() const;
     unsigned char const * cryptokey() const;
     std::string rawpath() const;
     TripleHashNode operator[](std::string childnodename) const;
};
}
#endif
