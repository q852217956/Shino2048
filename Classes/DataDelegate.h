//
//  DataDelegate.h
//  Shino2048
//
//  Created by shino on 16/5/10.
//
//

#ifndef DataDelegate_h
#define DataDelegate_h

class DataDelegate {
public:
    virtual ~DataDelegate() {};
    virtual bool readData() = 0;
    virtual bool writeData(bool isGameOver) = 0;
};

#endif /* DataDelegate_h */
