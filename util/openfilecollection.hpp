#ifndef OPENFILECOLLECTION_HPP
#define OPENFILECOLLECTION_HPP
#include <map>
#include <deque>
#include <stdint.h>
#include <sys/types.h>
//This template is meant as a generic container for node objects.
//A node object needs to have the following zero argument void methods defined:
//  * preOpen()
//  * lowLevelClose()  
//  * lowLevelOpen()
//  * postClose()
//The template takes 3 arguments:
//  * The type of the node to be used.
//  * The maximum number of low level file objects that we can keep open at the same time.
//  * The maximum size of the internal event queue.
template <typename nodeType,size_t maxOpenFiles,size_t maxQueueSize>
class openfilecollection {
    uint64_t mLastHandle; //Used for determinign the unique file handle. 
    std::map<uint64_t, nodeType> mCollection; //The collection ov virtually open file nodes.
    std::map<uint64_t, size_t> mFullyOpen; //A map holding the event count for each low-level open file node mentioned 
                                           //in the event queue.
    std::deque<uint64_t> mOpperQue;        //An event queue that is used to determine what virtualy open node may be low-level
                                           //closed next when needed.
    //Method for getting the next free high-level file handle
    uint64_t  getFreeFhNumber() {
       //0 isn't a valid file handle, other numbers are valid as long as they are not yet issued.
       while ((mLastHandle == 0) || (mCollection.count(mLastHandle))) {
         mLastHandle += 1;
       }
       return mLastHandle;
    }
    //This method will remove events from the front of the event queue when possible or needed.
    void cleanupQueFront() {
        //If the front most node is in the queue more often than once, we can savely remove it and adjust
        //the count in mFullyOpen. If the size of the queue is larger than the maximum size, we shall remove it
        //even if its not needed from a number of open file handles perspective.
        while ((mFullyOpen[mOpperQue.front()] > 1) || (mOpperQue.size() > maxQueueSize)) {
            mFullyOpen[mOpperQue.front()] -= 1; //Reduce the count.
            if (mFullyOpen[mOpperQue.front()] == 0) { //If this means the count drops to zero, low-level close the node for now.
                mCollection[mOpperQue.front()].lowLevelClose();
                mFullyOpen.erase(mOpperQue.front());
            }
            mOpperQue.pop_front(); //Drop the event from the event queue.
            
        }
    }
    //This method will low-level close high-level opened file node's untill we drop below the 
    //maximum allowed number of open files.
    void tempCloseIfNeeded() {
        while (mFullyOpen.size() > maxOpenFiles) {
            uint64_t candidate = mOpperQue.front(); //First remember the fh of the event we are to remove from the queue.
            mOpperQue.pop_front(); //Now drop the entry from the queue.
            if (mFullyOpen.count(candidate)) { //Check if the removed fh was still to be considered to be low-level open.
              mFullyOpen[candidate] -= 1;  //Adjust the counter
              if (mFullyOpen[candidate] == 0) { //If the counter drops to 0, low level close the node.
                  mCollection[candidate].lowLevelClose();
                  mFullyOpen.erase(candidate);
              }
            }
        }
        this->cleanupQueFront(); //There may be some entries now at the end of the queue that can be cleaned up.
    }
    openfilecollection(openfilecollection const &); //Copy constructor declared private -> no copy.
    openfilecollection &operator=(openfilecollection const &); //assignment declared provate -> no copy.
  public:
    openfilecollection():mLastHandle(0){} //Constructor initializes the mLastHandle value.
    ~openfilecollection() { 
       //Destructor will close any low level open file node.
       for (std::map<uint64_t, size_t>::iterator   i1=mFullyOpen.begin();  i1 != mFullyOpen.end(); ++i1) {
           uint64_t fh = i1->first;
           mCollection[fh].lowLevelClose();
       }
       //After the low-level close, the destructor also will call the post close method.
       for (typename std::map<uint64_t, nodeType >::iterator i2=mCollection.begin(); i2 != mCollection.end(); ++i2) {
           i2->second.postClose();
       }
    }
    //Operator for accessing the file node object. This operator will return a low-level opened file object.
    nodeType & operator[](uint64_t fh) {
        if (mFullyOpen.count(fh)) { //If the file node is already low-level open:
          if (fh != mOpperQue.back()) { // If adding the operation to the queue won't result in a duplicate:
            mFullyOpen[fh] += 1;   //Update the occurence count
            mOpperQue.push_back(fh);//And add an other occurence of theis file handle to the back of the queu. 
          }
        } else { //If the file curently wasn't low-level opened:
          mFullyOpen[fh] = 1; //Set the occurence count to one.
          mOpperQue.push_back(fh); //Add the file handle to the event queu.
          this->tempCloseIfNeeded(); //and make sure we don't exeed the max number of open files.
          mCollection[fh].lowLevelOpen(); //Now do a low level open of the file node.
        }
        return mCollection[fh]; //Return our open file node by reference.
    }
    //This method adds a new node to the container and does both a high level and low level open.
    uint64_t open(nodeType &node) {
        uint64_t fh=this->getFreeFhNumber(); //Get a new fh number.
        mCollection[fh] = node; //Add the node to our container under the new fh.
        mFullyOpen[fh] = 1; //Set the queue occurence count to one.
        mOpperQue.push_back(fh); //and add the file handle to the event queue.
        this->tempCloseIfNeeded(); //Before opening the new file, make sure we don't exeed the maximum number of open files.
        node.preOpen(); //Do any high-level one-time initialization.
        node.lowLevelOpen(); //Do the low level file open.
        return fh; //Return our brand new high-level open file handle.
    }
    //Method for explicit high-level closing of a file node.
    void close(uint64_t fh) {
        //If the file is low-level open, close it first.
        if (mFullyOpen.count(fh)) {
            mFullyOpen.erase(fh);
            mCollection[fh].lowLevelClose();
        }
        //Perform any high-level cleanup tasks on the file node.
        mCollection[fh].postClose();
        //Drop from our high-level collection.
        mCollection.erase(fh);
    }
};
#endif
