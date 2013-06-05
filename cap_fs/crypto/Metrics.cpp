#include "Metrics.hpp"
namespace capfs {
  namespace crypto {
    Metrics::Metrics(off_t o,size_t s):mPreSize(o%16),mPostSize(15 - ((o+s+15)%16)),mCtr((o-mPreSize)/16){}
    size_t Metrics::presize() {
        return mPreSize;
    }
    size_t Metrics::postsize() {
        return mPostSize;
    }
    off_t Metrics::ctr() {
        return mCtr;
    }
  }
}
