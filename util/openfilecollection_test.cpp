#include "openfilecollection.hpp"
#include <iostream>
class Node {
    int mNum;
  public:
    Node():mNum(0){
        //std::cout << "constructor 0" << std::endl;
    }
    Node(int num):mNum(num){
        //std::cout << "constructor " << num << std::endl;
    }
    Node(Node const & n):mNum(n.mNum){
        //std::cout << "copy constructor " << mNum << std::endl;
    }
    Node & operator=(Node const & n) {
      mNum = n.mNum;
      //std::cout << "assignment " << mNum << std::endl;
      return *this;
    }
    ~Node() {
      //std::cout << "Destructor " << mNum << std::endl;
    }
    void preOpen() { std::cout << "preOpen " << mNum << std::endl;}
    void lowLevelClose() { std::cout << "lowLevelClose " <<  mNum << std::endl;}
    void lowLevelOpen() {std::cout << "lowLevelOpen " <<  mNum << std::endl;}
    void postClose() {std::cout << "postClose " <<  mNum << std::endl;}
};

int main(int argc,char **argv) {
   openfilecollection<Node,4,1000> coll;
   Node n1(1);
   Node n2(2);
   Node n3(3);
   Node n4(4);
   Node n5(5);
   Node n6(6);
   uint64_t fh1=coll.open(n1); 
   uint64_t fh2=coll.open(n2);
   uint64_t fh3=coll.open(n3);
   uint64_t fh4=coll.open(n4);
   uint64_t fh5=coll.open(n5);
   uint64_t fh6=coll.open(n6);
   Node n7=coll[fh1];
   Node n8=coll[fh5];
   coll.close(fh1);
   coll.close(fh3);
   coll.close(fh5);
   return 0;
}
