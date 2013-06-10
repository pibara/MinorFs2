#include "openfilecollection.hpp"
#include <iostream>
#include <vector>
class Node {
    int mNum;
  public:
    Node():mNum(0){
        std::cout << "OOPS: constructor 0" << std::endl;
    }
    Node(int num):mNum(num){
        std::cout << "constructor " << num << std::endl;
    }
    Node(Node const & n):mNum(n.mNum){
        std::cout << "copy constructor " << mNum << std::endl;
    }
    Node(Node&& n):mNum(n.mNum){
        std::cout << "move constructor " << mNum << std::endl;
        n.mNum=0;
    }
    Node & operator=(Node const & n) {
      mNum = n.mNum;
      std::cout << "assignment " << mNum << std::endl;
      return *this;
    }
    Node& operator=(Node&& n) {
      mNum = n.mNum;
      std::cout << "move assignment " << mNum << std::endl;
      n.mNum=0;
      return *this;
    }
    ~Node() {
      if (mNum) {
        std::cout << "Destructor " << mNum << std::endl;
      }
    }
    void preOpen() { std::cout << "preOpen " << mNum << std::endl;}
    void lowLevelClose() { std::cout << "lowLevelClose " <<  mNum << std::endl;}
    void lowLevelOpen() {std::cout << "lowLevelOpen " <<  mNum << std::endl;}
    void postClose() {std::cout << "postClose " <<  mNum << std::endl;}
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
