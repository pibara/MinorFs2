#ifndef OPENFILECOLLECTION_HPP
#define OPENFILECOLLECTION_HPP
#include <map>
#include <deque>
#include <stdint.h>
#include <sys/types.h>
template <typename nodeType,size_t maxOpenFiles>
class openfilecollection {
    uint64_t mLastHandle;
    std::map<uint64_t, nodeType> mCollection;
    std::map<uint64_t, size_t> mFullyOpen;
    std::deque<uint64_t> mOpperQue;
    uint64_t  getFreeFhNumber() {
       while ((mLastHandle == 0) || (mCollection.count(mLastHandle))) {
         mLastHandle += 1;
       }
       return mLastHandle;
    }
    void tempCloseIfNeeded() {
        while (mFullyOpen.size() > maxOpenFiles) {
            uint64_t candidate = mOpperQue.front();
            mOpperQue.pop_front();
            if (mFullyOpen.count(candidate)) {
              mFullyOpen[candidate] -= 1;
              if (mFullyOpen[candidate] == 0) {
                  mCollection[candidate].tempClose();
                  mFullyOpen.erase(candidate);
              }
            }
        }
    }
  public:
    openfilecollection():mLastHandle(0){}
    nodeType & operator[](uint64_t fh) {
        if (mFullyOpen.count(fh)) {
          mFullyOpen[fh] += 1;
          mOpperQue.push_back(fh);
        } else {
          mFullyOpen[fh] = 1;
          mOpperQue.push_back(fh);
          this->tempCloseIfNeeded();
          mCollection[fh].reOpen();
        }
        return mCollection[fh];
    }
    uint64_t open(nodeType node) {
        uint64_t fh=this->getFreeFhNumber();
        mCollection[fh] = node;
        mFullyOpen[fh] = 1;
        mOpperQue.push_back(fh);
        this->tempCloseIfNeeded();
        node.initialOpen(); 
    }
    void close(uint64_t fh) {
        if (mFullyOpen.count(fh)) {
            mFullyOpen.erase(fh);
        }
        mCollection[fh].finalClose();
        mCollection.erase(fh);
    }
};
#endif
