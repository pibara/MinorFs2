#include "TripleHashParentChild.hpp"
#include <boost/tokenizer.hpp>

namespace capfs {
namespace algo {
TripleHashParentChild::TripleHashParentChild(std::string salt,std::string path) {
   boost::char_separator<char> sep("/");
   boost::tokenizer<boost::char_separator<char> > tokens(path,sep);
   bool firsttoken=true;
   for (boost::tokenizer<boost::char_separator<char> >::iterator tok_iter = tokens.begin();tok_iter != tokens.end(); ++tok_iter) {
       std::string curtoken=*tok_iter;
       if (firsttoken) {
           if (curtoken != "") {
               mChild=TripleHashNode(salt,curtoken);
               firsttoken=false;
           }
       } else {
           mParent=mChild;
           mChild=mParent[curtoken];
       }
   }
}
TripleHashParentChild::TripleHashParentChild(){}
TripleHashNode TripleHashParentChild::child() const {
  return mChild;
}
TripleHashNode TripleHashParentChild::parent() const {
  return mParent;
}
} 
}
