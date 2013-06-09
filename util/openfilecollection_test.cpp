#include "openfilecollection.hpp"
#include <iostream>
class Node {
  public:
    void initialOpen() { std::cout << "initialOpen" << std::endl;}
    void tempClose() { std::cout << "tempClose" << std::endl;}
    void reOpen() {std::cout << "reOpen" << std::endl;}
    void finalClose() {std::cout << "finalClose" << std::endl;}
};

int main(int argc,char **argv) {
   openfilecollection<Node,4> coll;
   Node n1;
   Node n2;
   Node n3;
   Node n4;
   Node n5;
   Node n6;
   uint64_t fh1=coll.open(n1); 
   uint64_t fh2=coll.open(n2);
   uint64_t fh3=coll.open(n3);
   uint64_t fh4=coll.open(n4);
   uint64_t fh5=coll.open(n5);
   uint64_t fh6=coll.open(n6);
   Node n7=coll[fh1];
   Node n8=coll[fh5];
   coll.close(fh1);
   coll.close(fh2);
   coll.close(fh3);
   coll.close(fh4);
   coll.close(fh5);
   coll.close(fh6);
}
