#include <iostream>
#include <string>
#include <TripleHashNode.hpp>

void printkey(capfs::TripleHashNode &node,std::string name) {
  std::cerr << name << ":" << std::endl;
  std::cout << "\trw=" << node.rwcap() << std::endl;
  std::cout << "\tro=" << node.rocap() << std::endl;
  std::cout << "\tsp=" << node.rawpath() << std::endl;
} 

int main(int argc,char **argv){
  capfs::TripleHashNode topnode("ThisIsASPrettyBadSecretSalt","ABCDEFGHIJKLMNOPQRSTUVWXYZ234567ABCDEFGHIJKLMNOPQRST",true);
  capfs::TripleHashNode childnode=topnode["Foo"];
  capfs::TripleHashNode grandchildnode=childnode["Bar"];
  printkey(topnode,"topnode");
  printkey(childnode,"childnode");
  printkey(grandchildnode,"grandchildnode");
}
