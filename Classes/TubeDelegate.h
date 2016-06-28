//
//  TubeDelegate.h
//  Shino2048
//
//  Created by shino on 16/6/29.
//
//

#ifndef TubeDelegate_h
#define TubeDelegate_h

class TubeDelegate {
public:
    virtual ~TubeDelegate() {}
    virtual void updateScore(int num) = 0;
    virtual void updateActionNum() = 0;
    virtual void updateIsFinished() = 0;
};

#endif /* TubeDelegate_h */
