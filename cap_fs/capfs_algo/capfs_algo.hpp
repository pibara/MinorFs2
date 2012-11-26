namespace capfs {
class CapFsNode;
class CapFsNode {
     std::string mSalt;
     std::string mName;
     std::string mKey1;
     std::string mKey2;
     std::string mKey3;
     bool mCanWrite;
     bool mValid;
     CapFsNode();
     void setRo() {mCanWrite=false;}
     std::string hmacSha256(std::string parentkey,std::string name,std::string salt)
     CapFsNode(std::string salt,std::string nodename,std::string parentkey2,bool attenuate);
   public:
     CapFsNode(std::string salt);  // constructor for top node.
     CapFsNode(std::string relpath); // constructor using a relative path.
     bool valid() {return mValid;}
     bool canWrite(){return mCanWrite;}
     std::string rwcap() {return mnWrite ? mKey1 : "" ;}
     std::string rocap() {return mKey2;}
     char * cryptokey() {return mRawKey2;}
     std::string rawpath() {return mRawPath;}
     CapFsNode roAttenuated();
     CapFsNode operator()(std::string childnodename);
};
}
