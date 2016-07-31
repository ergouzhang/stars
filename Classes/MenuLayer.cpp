//
//  MenuLayer.cpp
//  MyCppGame
//
//  Created by zwj on 16/6/13.
//
//

#include "MenuLayer.hpp"
#include "randomStars.hpp"
#include <math.h>
#include "ConstellationSprite.hpp"
#include <string>
#include "C2DXShareSDK.h"
#include "Header.h"

//create()方法
MenuLayer* MenuLayer::create(int starNum,int shinStarNum,int collNum)
{

    MenuLayer* layer=new MenuLayer(starNum,shinStarNum,collNum);
    
    if (layer&&layer->init())
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

//构造
MenuLayer::MenuLayer(int starNum, int ShineStar,int collNum)
{
    this->collensation=collNum;
    this->starNum=starNum;
    this->shineStarNum=ShineStar;
    lineNum=0;
}

//init()方法

bool MenuLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
   
  
    
    //亲密值
    Relation= UserDefault::getInstance()->getIntegerForKey("RELATION", 0);

     //成长值
    growth= UserDefault::getInstance()->getIntegerForKey("GROWTH", 0);
    
    
    //星星的生成：starNum\shineStarNum\collensation
   stars =new randomStars(this,starNum,shineStarNum,collensation);
   size=Director::getInstance()->getVisibleSize();
    
    //背景
    Sprite* backGround=Sprite::create("bg_L.png");
    backGround->setPosition(size.width/2,size.height/2);
    this->addChild(backGround,0);
    
    
    backGround2=Sprite::create("bg_2.png");
    backGround2->setPosition(size.width/2,backGround2->getContentSize().height/2);
    // backGround->setScale(1.2);
    this->addChild(backGround2,1);

    //布置星星
    stars->randomPosition();
    
    //MAN
    Sprite* man=Sprite::create("man2.png");
    man->setPosition(100,105);
    man->setScale(0.8f);
    this->addChild(man,101,100);
    
 
    
    //boy
    Sprite* boy=Sprite::create("boy-hand.png");
    boy->setPosition(200,100);
    boy->setScale(0.8f);
    this->addChild(boy,102,101);
    
    //talkingBox
    talkingBox=Sprite::create("talkingBox_L.png");
    talkingBox->setAnchorPoint(Point(0.5,0.5));
    talkingBox->setPosition(370,80);
    this->addChild(talkingBox,4);

     talkingString="老爸，我要那个星星！";
    talkingLabel=Label::createWithTTF("", "fonts/china.ttf", 25);
    talkingLabel->setPosition(talkingBox->getContentSize().width/2,talkingBox->getContentSize().height/3);
    talkingLabel->setColor(Color3B::WHITE);
    talkingBox->addChild(talkingLabel,3);
    talkingLabel->setString(talkingString);
   
//    //schdule（）函数实现打字效果
//    this->schedule(schedule_selector(MenuLayer::talkingBoxFunc), 0.05f);
    
    //说完话隐藏
    talkingBox->runAction(Sequence::create(DelayTime::create(5),Hide::create(), NULL));
    
    
//暂停按钮
    auto pasueLabel=Label::createWithTTF("退出", "fonts/china.ttf", 40);
    pasueLabel->setColor(Color3B::RED);
    auto pasueButton=MenuItemLabel::create(pasueLabel,CC_CALLBACK_1(MenuLayer::goBackCallBack, this));
    pasueButton->setPosition(pasueButton->getContentSize().width/2+5,size.height-pasueButton->getContentSize().height);
    auto menu=Menu::create(pasueButton, NULL);
    menu->setPosition(0,0);
    this->addChild(menu);
 
    
    
    
    //hand
    pointer=Sprite::create("hand.png");
    pointer->setAnchorPoint(Point(0,0.5));
    pointer->setScale(1.5);
    pointer->setPosition(215,95);
    this->addChild(pointer,103,102);
    pointToStar();

    
    //timer
    auto moon=Sprite::create("moon_2.png");
    moon->setPosition(Point(moon->getContentSize().width,490));
    moon->setAnchorPoint(Point(0,0));
    moon->setTag(10);
    this->addChild(moon,0);

    //con0星座能力
    bool con0=UserDefault::getInstance()->getBoolForKey("CON0", false);
    if (con0==true)
    {
        this->schedule(schedule_selector(MenuLayer::timerFunc), 0.15f);
    }
    else
    {
        this->schedule(schedule_selector(MenuLayer::timerFunc), 0.1f);
    }
    
    //开启触摸
    setTouchEnabled(true);
    setSwallowsTouches(true);
    setTouchMode(kCCTouchesOneByOne);


   
   
    
    return true;
}

//talkingBox打字效果回调

//void MenuLayer::talkingBoxFunc(float dt)
//{
//    if (talkingString==talkingStr)
//    {
//        index=0;
//       
//       unschedule(schedule_selector(MenuLayer::talkingBoxFunc));
//    }
//    else
//    {
//    talkingStr=talkingString.substr(0,index);
//    index++;
//    talkingLabel->setString(talkingStr.c_str());
//    }
//}

//触摸监控
bool MenuLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
    
    //触摸点位置
    auto touchPoint=touch->getLocation();
    
    if (stars->starsVector.empty())
    {
        return false;
    }
    
    //判断触摸点是否在其他星星上
    for (int i=0; i<stars->starsVector.size(); i++)
    {
      //触摸点在星星上且不在正确的星星上
        if ((stars->starsVector.at(i)->getBoundingBox().containsPoint(touchPoint))&& stars->starsVector.at(i)!=stars->starsVector.back())
        {
            //提示线
            if (lineShow==false)
            {
                lineNum++;
            }
            
            //con1能力，2次提示
            if (UserDefault::getInstance()->getBoolForKey("CON1", false))
            {
                if (lineNum>=2&&lineShow==false)
                {
                    //提示线条
                    line=DrawNode::create();
                    line->drawLine(Point(215,95),  (stars->starsVector.back())->getPosition(), Color4F::WHITE);
                    this->addChild(line,2);
                    lineNum=0;
                    lineShow=true;
                }
            }
            else
            {
            if (lineNum>=3&&lineShow==false)
            {
                //提示线条
                line=DrawNode::create();
                line->drawLine(Point(215,95),  (stars->starsVector.back())->getPosition(), Color4F::WHITE);
                this->addChild(line,2);
                lineNum=0;
                lineShow=true;
            }
            }
            //错误提示效果
            talkingString="不是那个～";
            talkingLabel->setString(talkingString);
            talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(2),Hide::create(),NULL));
            auto act1=ScaleTo::create(0.3, 2.5);
            auto act2=ScaleTo::create(0.3, 1.5);
            stars->starsVector.at(i)->runAction(Sequence::create(act1,act2, NULL));
            
            //点错亲密值降低
            if (Relation>0)
            {
                --Relation;
                 UserDefault::getInstance()-> setIntegerForKey("RELATION", Relation);
                if (Relation>0)
                {
                    auto tipsLabel=Label::createWithTTF("亲密值降低1！", "fonts/china.ttf", 30);
                    tipsLabel->setColor(Color3B::YELLOW);
                    tipsLabel->setPosition(size.width/2,size.height-tipsLabel->getContentSize().height);
                    tipsLabel->runAction(Sequence::create(EaseSineIn::create( Spawn::create(MoveBy::create(1.0f, Point(0,-50)), FadeOut::create(1.0f),NULL)), RemoveSelf::create(),NULL));
                    this->addChild(tipsLabel,100);
                   
                    
                }
                
                
                if (Relation==0)
                {
                    auto tipsLabel=Label::createWithTTF("亲密值已经0了！", "fonts/china.ttf", 30);
                    tipsLabel->setColor(Color3B::YELLOW);
                    tipsLabel->setPosition(size.width/2,size.height-tipsLabel->getContentSize().height);
                    tipsLabel->runAction(Sequence::create(EaseSineIn::create( Spawn::create(MoveBy::create(1.0f, Point(0,-50)), FadeOut::create(1.0f),NULL)), RemoveSelf::create(),NULL));
                    this->addChild(tipsLabel,100);

                }
                
            }
            
            
        }
        
        
    }
    
    
    
    
    //触摸指向的星星back()
        if ((stars->starsVector.back())->getBoundingBox().containsPoint(touchPoint))
        {
           
            //若点击了星座
            if (stars->starsVector.back()==stars->constellation)
            {
               //写入plist
                
                writToPlist();
                conllGeted=true;
                
                //记录获取了的星座,以其序号为key
              
                
               
                
                switch (collensation)
                {
                    case 1:  //baiyang
                    {  //提示信息
                        
                        talkingString="yeap,白羊座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        
                        
                         //显示连线
                        stars->constellation->getChildByTag(1)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                      
                       
                        
                         stars->constellation->getChildByTag(2)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        
                        
                         stars->constellation->getChildByTag(3)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        break;
                        
                }
                    case 2:  //jinniu
                    {talkingString="yeap, 金牛座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        
                        
                        for (int i=1; i<=17; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                          
                        }
                        
                        
                        
                        
                        break;
                    }
                    case 3:  //shuangzi
                        talkingString="yeap, 双子座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        
                        for (int i=0; i<=15; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }
                        
                        break;
                        
                        
                    case 4:  //shuangyu
                        talkingString="yeap, 双鱼座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        
                        for (int i=0; i<=18; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }
                        break;
                        
                        
                    case 5:  //mojie
                        talkingString="yeap, 摩羯座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        
                        for (int i=0; i<=10; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }

                        
                        break;
                        
                        
                    case 6:  //chunv
                        talkingString="yeap, 处女座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        for (int i=0; i<=13; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }

                        
                        
                        break;
                        
                        
                        
                    case 7:  //sheshou
                        talkingString="yeap, 射手座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        
                        
                        for (int i=0; i<=21; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }
                        break;
                        
                        
                    case 8:  //shuiping
                        talkingString="yeap, 水瓶座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        
                        for (int i=0; i<=14; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }
                        break;
                        
                        
                    case 9:  //tianping
                        talkingString="yeap, 天平座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        for (int i=0; i<=7; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }
                        break;
                        
                        
                    case 10:  //shizi
                        talkingString="yeap, 狮子座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        
                        for (int i=0; i<=10; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }
                        break;
                        
                        
                    case 11:  //tianxie
                        talkingString="yeap, 天蝎座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        for (int i=0; i<=4; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }
                        break;
                        
                        
                    case 12:  //juxie
                        talkingString="yeap, 巨蟹座！";
                        talkingBox->runAction(Sequence::create(Show::create(),DelayTime::create(5),Hide::create(),NULL));
                        
                        
                        for (int i=0; i<=6; i++)
                        {
                            stars->constellation->getChildByTag(i)->runAction(Sequence::create(Show::create(),Blink::create(1.0f, 3), NULL));
                        }
                        break;
                        
                    default:
                        break;
                }
                //星座对话
                talkingLabel->setString(talkingString);
                
               
                auto collact1=DelayTime::create(2.0f);
                
                auto collact2=ScaleTo::create(2.0f, 1.5);
                
              
             //   stars->starsVector.back()->runAction(Sequence::create(Spawn::create(collact1, collact2,NULL), RemoveSelf::create(),NULL));
                
                
            }
            
            else
            {
           //若点在星星上，则移除此星星
            //移除特效
            auto act1=MoveTo::create(1.0f, Point(size.width/2,size.height/2));
            auto act2=ScaleTo::create(1.0f, 3.0f);
            
            //爆炸效果
            auto act3=ParticleExplosion::create();
            act3->setPosition(Point::ZERO);
            act3->setLife(1.0f);
            act3->setScale(0.2f);
       
          (stars->starsVector.back())->addChild(act3);
            
            //EASESINEOUT:正弦加速
            (stars->starsVector.back())->runAction(Sequence::create(EaseSineOut::create(Spawn::create(act1,act2, NULL)),RemoveSelf::create(), NULL));
            }
            
            
            //清除提示线条
            if (lineShow)
            {
                line->removeFromParent();
                lineShow=false;
                
            }
            //得分
            score++;
            //亲密值增加
            //CON9技能
            if (UserDefault::getInstance()->getBoolForKey("CON9", false))
            {
                //概率增加2
                int i=random(0, 2);
              
                if (i==1)
                {
                    Relation+=2;
                    auto tipsLabel=Label::createWithTTF("亲密值增加2！", "fonts/china.ttf", 30);
                    tipsLabel->setColor(Color3B::RED);
                    tipsLabel->setPosition(size.width/2,size.height-tipsLabel->getContentSize().height);
                    tipsLabel->runAction(Sequence::create(ScaleTo::create(0.1, 1.2),ScaleTo::create(0.1, 1.4),EaseSineIn::create( Spawn::create(MoveBy::create(3.0f, Point(0,-50)), FadeOut::create(3.0f),NULL)), RemoveSelf::create(),NULL));
                    this->addChild(tipsLabel,100);
                }
                else
                {
                //没双倍
                    Relation++;
                    auto tipsLabel=Label::createWithTTF("亲密值增加1！", "fonts/china.ttf", 30);
                    tipsLabel->setColor(Color3B::RED);
                    tipsLabel->setPosition(size.width/2,size.height-tipsLabel->getContentSize().height);
                    tipsLabel->runAction(Sequence::create(EaseSineIn::create( Spawn::create(MoveBy::create(1.5f, Point(0,-50)), FadeOut::create(1.5f),NULL)), RemoveSelf::create(),NULL));
                    this->addChild(tipsLabel,100);
                }
                
                
            }
            else
            {
                Relation++;
                auto tipsLabel=Label::createWithTTF("亲密值增加1！", "fonts/china.ttf", 30);
                tipsLabel->setColor(Color3B::RED);
                tipsLabel->setPosition(size.width/2,size.height-tipsLabel->getContentSize().height);
                tipsLabel->runAction(Sequence::create(EaseSineIn::create( Spawn::create(MoveBy::create(1.5f, Point(0,-50)), FadeOut::create(1.5f),NULL)), RemoveSelf::create(),NULL));
                this->addChild(tipsLabel,100);

            }
            UserDefault::getInstance()->setIntegerForKey("RELATION", Relation);
            
      
            
            //弹出点击正确的星星
            //stars->starsVector.pop_back();
            stars->starsVector.popBack();
            //错误计数清零
             lineNum=0;
            pointer->setRotation(0);
            // 若空，则胜利
            if (stars->starsVector.empty())
            {
                //成长值＋＋
                growth++;
                UserDefault::getInstance()->setIntegerForKey("GROWTH", growth);
                //设置age
                if (growth<=AGE10&& growth>=1)
                {
                    UserDefault::getInstance()->setIntegerForKey("age", 10);
                    
                }
                else if (growth>AGE10&&growth<AGE15)
                {
                 UserDefault::getInstance()->setIntegerForKey("age", 15);
                
                }
                else if (growth>AGE15&&growth<AGE20)
                {
                    UserDefault::getInstance()->setIntegerForKey("age", 20);
                    
                }
                
                
                else if(growth>AGE20)
                {
                    UserDefault::getInstance()->setIntegerForKey("age", 25);
                    
                }
                
                
                
                
                timeOver();
                
                
                
                
            }
            else
            {
            pointToStar();
            }
        }
  
    
    
    
    return false;
}

void MenuLayer::pointToStar()
{
    
    if (stars->starsVector.empty())
    {
        return ;
    }
    
    //星星的坐标
     //测试指向0号星星
    
    auto starToPoint= (stars->starsVector.back())->getPosition();
    log("x=%lf,y=%lf\n",starToPoint.x,starToPoint.y);
    //正切
    
    log("pointerX=%lf,pointerY=%lf\n",pointer->getPosition().x,pointer->getPosition().y);
    double tanAngle=(starToPoint.y-pointer->getPosition().y)/(starToPoint.x-pointer->getPosition().x);
    log("tanAngle=%lf\n",tanAngle);
    //反正切
    double Angle=atan(tanAngle)*180/3.141592653;
    log("angle=%lf\n",Angle);
   
        //正数顺时针旋转节点，负数逆时针旋转节点。
    if (Angle<=0)
    {
        
         pointer->setRotation(-(180-fabs(Angle)));
    }
    if (Angle>=0)
    {
         pointer->setRotation(-Angle);
    }
   
    }


//写入plist
void MenuLayer::writToPlist()
{
    __String ActionNum;
    
    
//    if (starNum<5)
//    {
//        ActionNum="A0";
//    }
//    
//    
//   else if (starNum>5&&starNum<=10)
//    {
//        ActionNum="A10";
//    }
//   else if (starNum>10&&starNum<=15)
//    {
//        ActionNum="A15";
//    }
//   else if (starNum>15&&starNum<=20)
//    {
//        ActionNum="A20";
//    }

    int age=UserDefault::getInstance()->getIntegerForKey("age", 0);
    
    if (age==0)
    {
        ActionNum="A0";
    }
    
    
    else if (age==10)
    {
        ActionNum="A10";
    }
    else if (age==15)
    {
        ActionNum="A15";
    }
    else if (age==20)
    {
        ActionNum="A20";
    }

    
    
    std::  string writaBle=FileUtils::getInstance()->getWritablePath();
    std::string path=writaBle+"List.plist";
    
     //跟目录是dic类型
    auto  root=FileUtils::getInstance()->getValueMapFromFile(path.c_str());
    
        //各个章节是arry
    auto arr=root[ActionNum.getCString()].asValueVector();
    arr[collensation-1]=1;
    root[ActionNum.getCString()]=arr;
    FileUtils::getInstance()->writeToFile(root, path.c_str());
   

}


//timer回调
void MenuLayer::timerFunc(float dt)
{
    Sprite* moon =(Sprite*)this->getChildByTag(10);
    
    //下降
    moon->runAction(MoveBy::create(0.1f, Point(0,-2)));
    
    //透明度
    moon->setOpacity(moon->getOpacity()-0.01);

    //达到海平面
    if (moon->getPositionY() <=(backGround2->getContentSize().height-moon->getContentSize().height))
    {
        growth++;
        UserDefault::getInstance()->setIntegerForKey("GROWTH", growth);
        //设置age
        if (growth<=AGE10&& growth>=1)
        {
            UserDefault::getInstance()->setIntegerForKey("age", 10);
            
        }
        else if (growth>AGE10&&growth<AGE15)
        {
            UserDefault::getInstance()->setIntegerForKey("age", 15);
            
        }
        else if (growth>AGE15&&growth<AGE20)
        {
            UserDefault::getInstance()->setIntegerForKey("age", 20);
            
        }
        else if(growth>AGE20)
        {
        UserDefault::getInstance()->setIntegerForKey("age", 25);
        
        }
        
        

        timeOver();
        unschedule(schedule_selector(MenuLayer::timerFunc));
        
    }
    
    

}





//时间到
void MenuLayer::timeOver()
{
    //移除人
    this->removeChildByTag(100);
    this->removeChildByTag(101);
    this->removeChildByTag(102);
    this->pause();
    
    
    //提示版
    auto bookSprite=Sprite::create("book_s.png");
    bookSprite->setPosition(size.width/2,size.height/2);
    bookSprite->setScale(0, 0);
    this->addChild(bookSprite,1000);
    
    //分值
    char scoreBuf[128];
    sprintf(scoreBuf, "摘到了 %d 颗星星!",score);
    
    auto scoreLabel=Label::createWithTTF(scoreBuf, "fonts/china.ttf", 30);
    scoreLabel->setColor(Color3B::RED);
    scoreLabel->setPosition(Point(bookSprite->getContentSize().width/4,bookSprite->getContentSize().height-50));
    bookSprite->addChild(scoreLabel);
    
    
     sprintf(scoreBuf, "亲密值： %d ",Relation);
    auto relationLabel=Label::createWithTTF(scoreBuf, "fonts/china.ttf", 30);
    relationLabel->setColor(Color3B::RED);
    relationLabel->setPosition(Point(bookSprite->getContentSize().width/4,bookSprite->getContentSize().height-50-50));
    bookSprite->addChild(relationLabel);
    
    sprintf(scoreBuf, "成长值： %d ",growth);
    auto growtLabel=Label::createWithTTF(scoreBuf, "fonts/china.ttf", 30);
    growtLabel->setColor(Color3B::RED);
    growtLabel->setPosition(Point(bookSprite->getContentSize().width/4,bookSprite->getContentSize().height-50-50-50));
    //bookSprite->addChild(growtLabel);
    
    
    
    
    //弹出效果
    bookSprite->runAction(Sequence::create(ScaleTo::create(0, 0),ScaleTo::create(0.1, 0.2),ScaleTo::create(0.1, 0.5),ScaleTo::create(0.1, 0.8), ScaleTo::create(0.1, 1),NULL));

   
    
  //确定按钮
    auto okLabel=Label::createWithSystemFont("OK", "Marker Felt.ttf", 40);
    okLabel->setColor(Color3B::RED);
    auto okButton=MenuItemLabel::create(okLabel,CC_CALLBACK_1(MenuLayer::goBackCallBack, this));
    okButton->setPosition(bookSprite->getContentSize().width/4,bookSprite->getContentSize().height/4);

    
    //分享按钮
    auto shareLabel=Label::createWithTTF("分享", "fonts/china.ttf", 40);
    shareLabel->setColor(Color3B::RED);
     shareButton=MenuItemLabel::create(shareLabel,CC_CALLBACK_0(MenuLayer::shareCallBack, this));
    shareButton->setPosition(3*bookSprite->getContentSize().width/4,bookSprite->getContentSize().height/4);
    
    
    auto menu=Menu::create(okButton,shareButton, NULL);
    menu->setPosition(0,0);
    bookSprite->addChild(menu);
    
    
    
    
    //如果出现了星座

    if (collensation!=0 && conllGeted==true)
    {
        auto boardSprite=Sprite::create("mark_2.png");
        boardSprite->setPosition(size.width/2+bookSprite->getContentSize().width/4,size.height/2);
        boardSprite->setScale(0, 0);
        boardSprite->setRotation(-10);
        this->addChild(boardSprite,1001);
        
        
        boardSprite->runAction(Sequence::create(DelayTime::create(1.0f),ScaleTo::create(0, 1),ScaleTo::create(0.1, 0.8),ScaleTo::create(0.1, 0.6),ScaleTo::create(0.1, 0.5), NULL));
    
        char conBuf[10];
        sprintf(conBuf, "C%d.png",collensation);
        auto conSprite=Sprite::create(conBuf);
        conSprite->setPosition(boardSprite->getContentSize().width/2,boardSprite->getContentSize().height/2);
        conSprite->setScale(2.0f);
        boardSprite->addChild(conSprite);
        
        
    }

}

//sdkcallback


 void shareContentResultHandler(int seqId, cn::sharesdk::C2DXResponseState state, cn::sharesdk::C2DXPlatType platType, __Dictionary *result)
{
    switch (state)
    {
        case cn::sharesdk::C2DXResponseStateSuccess:
        {
            log("Success");
        }
            break;
        case cn::sharesdk::C2DXResponseStateFail:
        {
            log("Fail");
            //回调错误信息
            __Array *allKeys = result->allKeys();
            allKeys->retain();
            for (int i = 0; i < allKeys-> count(); i++)
            {
                __String *key = (__String*)allKeys->getObjectAtIndex(i);
                Ref *obj = result->objectForKey(key->getCString());
                
                log("key = %s", key -> getCString());
                if (dynamic_cast<__String *>(obj))
                {
                    log("value = %s", dynamic_cast<__String *>(obj) -> getCString());
                }
                else if (dynamic_cast<__Integer *>(obj))
                {
                    log("value = %d", dynamic_cast<__Integer *>(obj) -> getValue());
                }
                else if (dynamic_cast<__Double *>(obj))
                {
                    log("value = %f", dynamic_cast<__Double *>(obj) -> getValue());
                }
            }
        }
            break;
        case cn::sharesdk::C2DXResponseStateCancel:
        {
            log("Cancel");
        }
            break;
        default:
            break;
    }
}


//sdk
void MenuLayer::shareCallBack()
{
    int picNum=UserDefault::getInstance()->getIntegerForKey("PIC",0);
    char buf[128];
    sprintf(buf, "%d.png",picNum);
    std::string stringBuf=buf;
    //savePic
    std::string path=FileUtils::getInstance()->getWritablePath();
    std::string fullPath=path+stringBuf;
    utils::captureScreen(CC_CALLBACK_0(MenuLayer::capCallBack, this), fullPath);
    

    
    

 

}
//capcall
void MenuLayer::capCallBack()
{
    
    
    int picNum=UserDefault::getInstance()->getIntegerForKey("PIC",0);
    char buf[128];
    sprintf(buf, "%d.png",picNum);
    std::string stringBuf=buf;
    //savePic
    std::string path=FileUtils::getInstance()->getWritablePath();
    std::string fullPath=path+stringBuf;
    
    picNum++;
    UserDefault::getInstance()->setIntegerForKey("PIC", picNum);

    //分享内容
    __Dictionary *content = __Dictionary::create();
    content -> setObject(__String::create("stars for u"), "text");
    content -> setObject(__String::create(fullPath), "image");
    content -> setObject(__String::create("摘星星"), "title");
    content -> setObject(__String::create("www.baidu.com"), "url");
    content -> setObject(__String::createWithFormat("%d", cn::sharesdk::C2DXContentTypeImage), "type");
    
    //坐标转换
    Point point=Director::getInstance()->getVisibleSize();
    //分享
    cn::sharesdk::C2DXShareSDK::showShareMenu(NULL,content,0,0,shareContentResultHandler);

}

//ok
void MenuLayer::goBackCallBack(cocos2d::Ref *pSender)
{

    gsm->goBookLayer();
    
    //测试用～～～～～
    growth++;
    UserDefault::getInstance()->setIntegerForKey("GROWTH", growth);
    //设置age
    if (growth<=AGE10&& growth>=1)
    {
        UserDefault::getInstance()->setIntegerForKey("age", 10);
        
    }
    else if (growth>AGE10&&growth<AGE15)
    {
        UserDefault::getInstance()->setIntegerForKey("age", 15);
        
    }
    else if (growth>AGE15&&growth<AGE20)
    {
        UserDefault::getInstance()->setIntegerForKey("age", 20);
        
    }
    else if(growth>AGE20)
    {
        UserDefault::getInstance()->setIntegerForKey("age", 25);
        
    }

}