#include <iostream>
#include <string>
#include <TripleHashNode.hpp>

void printkey(capfs::TripleHashNode const &node,std::string name) {
  std::cerr << name << ":" << std::endl;
  std::cout << "\trw=" << node.rwcap() << std::endl;
  std::cout << "\tro=" << node.rocap() << std::endl;
  std::cout << "\tsp=" << node.rawpath() << std::endl;
} 

int main(int argc,char **argv){
  capfs::TripleHashNode topnode("ThisIsASPrettyBadSecretSalt","rw-ABCDEFGHIJKLMNOPQRSTUVWXYZ234567ABCDEFGHIJKLMNOPQRST");
  const capfs::TripleHashNode rotopnode("ThisIsASPrettyBadSecretSalt",topnode.rocap());
  const capfs::TripleHashNode childnode=topnode["Foo"];
  const capfs::TripleHashNode rochildnode=rotopnode["Foo"];
  const capfs::TripleHashNode grandchildnode=childnode["Bar"];
  const capfs::TripleHashNode rograndchildnode=rochildnode["Bar"];
  printkey(topnode,"topnode");
  printkey(rotopnode,"rotopnode");
  printkey(childnode,"childnode");
  printkey(rochildnode,"rochildnode");
  printkey(grandchildnode,"grandchildnode");
  printkey(rograndchildnode,"rograndchildnode");
}
