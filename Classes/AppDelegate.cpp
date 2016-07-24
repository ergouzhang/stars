#include "AppDelegate.h"
#include "GameSceneManager.hpp"
#include "C2DXShareSDK.h"

using namespace cn::sharesdk;

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    
    
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    glview->setDesignResolutionSize(960, 640, ResolutionPolicy::EXACT_FIT);
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 30);

    register_all_packages();

    
  
    //auto sceneManger=new GameSceneManager();
    
   // sceneManger->createOpeningLayer();
    GameSceneManager::createOpeningLayer();
    //sdk
    this->initSdk();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

//sdk

void AppDelegate::initSdk()
{
    __Dictionary *totalDict=__Dictionary::create();
    
    __Dictionary* weichatConf=__Dictionary::create();
    weichatConf->setObject(__String::create("wx4868b35061f87885"), "app_id");
    weichatConf->setObject(__String::create("64020361b8ec4c99936c0e3999a9f249"), "app_secret");
    std::stringstream wechat;
    wechat<<cn::sharesdk::C2DXPlatTypeWechatPlatform;
    totalDict->setObject(weichatConf, wechat.str());
    
    cn::sharesdk::C2DXShareSDK::registerAppAndSetPlatformConfig("154878c495c14", totalDict);
    



}
