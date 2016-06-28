//
//  ScoreDelegate.h
//  Shino2048
//
//  Created by shino on 16/5/8.
//
//

#ifndef ScoreDelegate_h
#define ScoreDelegate_h

class ScoreDelegate {
    
public:
    
    virtual ~ScoreDelegate() {}
    
    virtual void updateScore(int num) = 0;
    
    virtual void updateActionNum() = 0;
};

#endif /* ScoreDelegate_h */
