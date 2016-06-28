//
//  ResetGameDelegate.h
//  Shino2048
//
//  Created by shino on 16/5/11.
//
//

#ifndef ResetGameDelegate_h
#define ResetGameDelegate_h

class ResetGameDelegate {
public:
    virtual ~ResetGameDelegate() {};
    virtual void resetGame() = 0;
};

#endif /* ResetGameDelegate_h */
