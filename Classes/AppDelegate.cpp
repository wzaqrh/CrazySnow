#include "AppDelegate.h"
#include "AppMacros.h"
#include "common/CommonDef.h"
#include "entity/WindowStageEntity.h"
#include "common/SoundPool.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Cpp Empty Test");
		//glview->setFrameSize(640,1150);
        director->setOpenGLView(glview);
    }

	Size frameSize = glview->getFrameSize();
    
#if 0
    if (DESIGN_RESOLUTION_WIDTH > frameSize.width / frameSize.height * DESIGN_RESOLUTION_HEIGHT) {
        glview->setDesignResolutionSize(DESIGN_RESOLUTION_WIDTH, DESIGN_RESOLUTION_HEIGHT, ResolutionPolicy::EXACT_FIT);
    }
    else
        glview->setDesignResolutionSize(DESIGN_RESOLUTION_WIDTH, DESIGN_RESOLUTION_HEIGHT, ResolutionPolicy::FIXED_HEIGHT);
#else
    if (DESIGN_RESOLUTION_HEIGHT > frameSize.height / frameSize.width * DESIGN_RESOLUTION_WIDTH) {
        glview->setDesignResolutionSize(DESIGN_RESOLUTION_WIDTH, DESIGN_RESOLUTION_HEIGHT, ResolutionPolicy::EXACT_FIT);
    }
    else
        glview->setDesignResolutionSize(DESIGN_RESOLUTION_WIDTH, DESIGN_RESOLUTION_HEIGHT, ResolutionPolicy::FIXED_WIDTH);
#endif
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.
    {
        searchPath.push_back(myResource.directory);
        //director->setContentScaleFactor(myResource.size.width/designResolutionSize.width);
    }
    
    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);
	
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    director->setDisplayStats(true);
#else
    director->setDisplayStats(false);
#endif
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);


    // create a scene. it's an autorelease object
    WindowStageEntity::Inst()->fisrtStage(CS_STAGE_BEGIN);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SoundPool::Inst()->pauseAll();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SoundPool::Inst()->resumeAll();
}
