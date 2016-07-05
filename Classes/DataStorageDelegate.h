//
//  DataStorageDelegate.h
//  Shino2048
//
//  Created by shino on 16/7/1.
//
//

#ifndef DataStorageDelegate_h
#define DataStorageDelegate_h

class DataStorageDelegate {
public:
    virtual ~DataStorageDelegate() {};
    virtual bool readData() = 0;
    virtual bool writeData(bool isGameOver) = 0;
};

#endif /* DataStorageDelegate_h */
