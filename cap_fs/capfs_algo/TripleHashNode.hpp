//  Copyright (c) 2012, Rob J Meijer
//
//Permission is hereby granted, free of charge, to any person or organization
//obtaining a copy of the software and accompanying documentation covered by
//this license (the "Software") to use, reproduce, display, distribute,
//execute, and transmit the Software, and to prepare derivative works of the
//Software, and to permit third-parties to whom the Software is furnished to
//do so, all subject to the following:
//
//The copyright notices in the Software and this entire statement, including
//the above license grant, this restriction and the following disclaimer,
//must be included in all copies of the Software, in whole or in part, and
//all derivative works of the Software, unless such copies or derivative
//works are solely in the form of machine-executable object code generated by
//a source language processor.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//DEALINGS IN THE SOFTWARE.
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