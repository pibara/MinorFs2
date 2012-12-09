#ifndef MINORFS_CAPFS_TRIPLEHASHNODE_HPP
#define MINORFS_CAPFS_TRIPLEHASHNODE_HPP
#include <string>
namespace capfs {
class TripleHashNode;
class TripleHashNode {
     unsigned char mKey1[35];
     unsigned char mKey2[35];     
     unsigned char mKey3[35];
     std::string mSalt;
     bool mVKey1;
     bool mVKey3;
     TripleHashNode(std::string nodename,TripleHashNode *parent);
     void derivekey(unsigned char *inkey,std::string instring,std::string salt,unsigned char *);
     std::string treepath(std::string);
   public:
     TripleHashNode(std::string secretsalt,std::string b32cap,bool writeaccess);
     bool canWrite();
     std::string rwcap();
     std::string rocap();
     unsigned char * cryptokey();
     std::string rawpath();
     TripleHashNode operator[](std::string childnodename);
};
}
#endif
