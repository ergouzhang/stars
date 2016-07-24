//
//  shopLayer.hpp
//  MyCppGame
//
//  Created by zwj on 16/7/4.
//
//

#ifndef shopLayer_hpp
#define shopLayer_hpp

#include <stdio.h>
#include    "cocos2d.h"
USING_NS_CC;

class shopLayer:public Layer
{

public:
    
//    shopLayer();
//    ~shopLayer();
    
    virtual bool init();
    CREATE_FUNC(shopLayer);
    
    
    bool onTouchBegan(Touch* touch,Event* event);
    
    //static shopLayer* create(const char* backGroundImage);
    
    //virtual void onEnter();
    virtual void onExit();
    
    //item回调
    void talkCallBack(Ref* pSender);
    void examCallBack(Ref* pSender);
    void friendsCallBack(Ref* pSender);
    void  footballCallBack(Ref* pSender);
    void familyCallBack(Ref* pSender);
    
    //提示
    Label* tipsLabel;
    void tipsLabelAction();
    
    EventListenerTouchOneByOne* lisetener;
    
    Sprite* bg;
    
    
   Label* pointLabel;
    
    

};

#endif /* shopLayer_hpp */
