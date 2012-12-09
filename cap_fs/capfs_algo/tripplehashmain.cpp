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
