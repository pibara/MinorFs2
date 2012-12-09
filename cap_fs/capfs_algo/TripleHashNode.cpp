#include <iostream>
#include <TripleHashNode.hpp>
#include <base32.hpp>
#include <openssl/engine.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

namespace capfs {
//The public constructor.
TripleHashNode::TripleHashNode(std::string secretsalt,std::string b32cap):
    mSalt(secretsalt),
    mVKey3(false) { //We are lazy about calculating Key3
  mVKey1=(b32cap.substr(0,3)=="rw-");
  if (mVKey1) {
     b32decode<52>(b32cap.substr(3,52),mKey1);
     derivekey(mKey1,"read-only","nosalt",mKey2); //We imediately calculate Key2.
  } else {
     b32decode<52>(b32cap.substr(3,52),mKey2);
  }  
}
//The private constructo used by operator[] to derive a child node.
TripleHashNode::TripleHashNode(std::string nodename,TripleHashNode const *parent):
    mSalt(parent->mSalt),
    mVKey1(parent->mVKey1), //If the parent has marked Key1 as invalid, so will we to prevent 
                            //privilege escalation.
    mVKey3(false){ //We are lazy about calculating Key3
  derivekey(parent->mKey2,nodename,mSalt,mKey1); //We calculate Key1. This does not change the fact 
                                                 //that the valid Key1 may be marked invalid.
  derivekey(mKey1,"read-only","nosalt",mKey2); //We imediately calculate Key2.
}
void TripleHashNode::derivekey(unsigned char const *inkey,std::string instring,std::string salt,
                               unsigned char *outkey) const {
  unsigned int out_len=32;
  std::string data=instring + "::" + salt;
  HMAC(EVP_sha256(),inkey,32,(const unsigned char*) data.c_str(),data.size(),outkey,&out_len);
}

std::string TripleHashNode::treepath(std::string bs) const{
  return std::string("/") + bs.substr(0,2) + "/" + bs.substr(2,2) + "/" + bs.substr(4,48);
}

bool TripleHashNode::canWrite() const{
  return mVKey1;
}
std::string TripleHashNode::rwcap() const {
  return mVKey1 ? std::string("rw-") + b32encode<32>(mKey1) : "" ;
}
std::string TripleHashNode::rocap() const {
  return std::string("ro-") + b32encode<32>(mKey2);
}
unsigned char const * TripleHashNode::cryptokey() const {
  return mKey2;
}
std::string TripleHashNode::rawpath() const {
  if (mVKey3 == false) {
      derivekey(mKey2,"storage","nosalt",mKey3);
  }
  return treepath(b32encode<32>(mKey3));
}
TripleHashNode TripleHashNode::operator[](std::string childnodename) const {
  return TripleHashNode(childnodename,this);
}
}
