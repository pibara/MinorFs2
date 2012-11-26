#include <openssl/engine.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
namespace capfs {
CapFsNode::CapFsNode():mSalt(""),mName(""),mCanWrite(false),mValid(false),mKey1(""),mKey2(""),mKey3(""){}

CapFsNode::CapFsNode(std::string salt,std::string nodename,std::string parentkey2,bool attenuate): 
   mSalt(salt),mName(nodename),mCanWrite(not attenuate),mValid(true)
{
    mKey1=hmacSha256(parentkey2,nodename,salt);
    mKey2=hmacSha256(mKey1,"Read Only Key","nosalt");
    mKey3=hmacSha256(mKey2,"Storage Location Key","nosalt");
}
CapFsNode::CapFsNode(std::string relpath) {
    
}
std::string CapFsNode::hmacSha256(std::string parentkey,std::string name,std::string salt) {
    unsigned char result[32];
    unsigned int result_len;
    data=name + "::" + salt;
    HMAC(EVP_sha256(),key.c_str(),key.size(),data.c_str(),data.size(),result,&result_len);
        
}
CapFsNode CapFsNode::roAttenuated(){
  CapFsNode rocopy=*this;
  rocopy.setRo();
  return rocopy;
}
CapFsNode CapFsNode::operator()(std::string childnodename) {
  if (mValid) {
    return CapFsNode(mSalt,childnodename,mKey2,not mCanWrite);
  }
  return CapFsNode();
}
}
