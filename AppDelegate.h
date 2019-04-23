#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();
    /*This function is responsible for implementing an OpenGL (Open Graphics Library) context so you can use OpenGL
     to render your 2D and 3D graphic images on any platform*/


    virtual bool applicationDidFinishLaunching();
    /*  Launches and initializes your app and returns true if that launch was successful and false if it was not */


    virtual void applicationDidEnterBackground();
    /*  Tells the application what to do if it starts running in the background (is no longer on the screen).
     E.g. If the user gets a phone call or switches to a different app */

    virtual void applicationWillEnterForeground();
    /* Tells the application what to do if it starts running in the foreground again (is back on the screen).
     E.g. If the user returns to the app after a phone call */
};

#endif // _APP_DELEGATE_H_

