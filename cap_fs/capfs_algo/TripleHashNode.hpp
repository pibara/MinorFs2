#ifndef MINORFS_CAPFS_TRIPLEHASHNODE_HPP
#define MINORFS_CAPFS_TRIPLEHASHNODE_HPP
namespace capfs {
class TripleHashNode;
class TripleHashNode {
     unsigned char mKey1[32];
     unsigned char mKey2[32];     
     unsigned char mKey3[32];
     unsigned char *mSalt;
     bool mVKey1;
     bool mVKey2;
     TripleHashNode(std::string nodename,TripleHashNode *parent);
     derivekey(unsigned char *inkey,std::string instring,std::string salt,unsigned char *);
     std::string b32(unsigned char *key);
     std::string treepath(std::string);
   public:
     TripleHashNode(std::string secretsalt,std::string b32cap,bool writeaccess);
     bool canWrite(){return mVKey1;}
     std::string rwcap() {return mCanWrite ? std::string("rw-") + b32(mKey1) : "" ;}
     std::string rocap() {return std::string("ro-") + b32(mKey1)}
     unsigned char * cryptokey() {return mKey2;}
     std::string rawpath() {return treepath(b32(mKey1));}
     TripleHashNode operator[](std::string childnodename);
};
}
#endif
