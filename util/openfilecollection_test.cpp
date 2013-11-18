#include "openfilecollection.hpp"
#include <iostream>
#include <vector>
class Node {
    int mNum;
    bool mValid;
    bool mOpen;
  public:
    Node(Node const &) = delete;
    Node & operator=(Node const & n) = delete; 
    Node& operator=(Node&& n) = delete;
    Node():mNum(0),mValid(false),mOpen(false){}
    /*Constructor for the null node*/
    Node(int num):mNum(num),mValid(true),mOpen(false){
        std::cout << "constructor " << num << std::endl;
    }
    /*Move constructor*/
    Node(Node&& n):mNum(n.mNum),mValid(n.mValid),mOpen(n.mOpen){
        if (&n != this) {
          if (mValid) {
            n.mValid=false;
          } else {
            std::cout << "BOGUS use of move constructor " << mNum << std::endl;
          }
        } else {
           std::cout << "BOGUS self assignment in move constructor " << mNum << std::endl;
        }
    }
    /*Destructor*/
    ~Node() {
      if (mNum) {
        if (mValid) {
          if (mOpen) {
             std::cout << "Destructor of open node " << mNum << std::endl;
             this->lowLevelClose();
          } else {
              std::cout << "Destructor of closed node " << mNum << std::endl;
          }
        }
      }
    }
    void lowLevelClose() { 
       if (mValid) {
         if (mOpen) {
           std::cout << "lowLevelClose of open node " <<  mNum << std::endl;
           mOpen=false;
         } else {
           std::cout << "lowLevelClose of closed node " <<  mNum << std::endl;
         }
       } else {
          std::cout << "BOGUS lowLevelClose of invalidated node" <<  mNum << std::endl;
       }
    }
    void lowLevelOpen() {
       if (mValid) {
         if (mOpen) {
           std::cout << "BOGUS lowLevelOpen of open node " <<  mNum << std::endl;
         } else {
           std::cout << "lowLevelOpen " <<  mNum << std::endl;
           mOpen=true;
         }
       } else {
          std::cout << "BOGUS lowLevelOpen of invalidated node" <<  mNum << std::endl;
       }
    }
};

int main(int argc,char **argv) {
   openfilecollection<Node,4,1000> coll;
   std::vector<uint64_t> fhs(6);
   for (int index=0; index < 6; ++index) {
      fhs[index]=coll.open(Node(index + 1));
   }
   Node & n7=coll[fhs[0]];
   Node & n8=coll[fhs[4]];
   coll.close(fhs[0]);
   coll.close(fhs[2]);
   coll.close(fhs[4]);
   return 0;
}
