#include "WindowLinux.h"

#include "Engine.h"
#include "utils/Logger.h"

#include <X11/Xatom.h>
#include <iostream>

using namespace v3d;
using namespace v3d::platform;
using namespace v3d::event;

CWindowLinux::CWindowLinux(const WindowParam& param)
    : CWindow(param)
    , m_hasFocus(false)
    , m_isMinimized(false)

    , m_close(false)
{
    m_platformType = EPlatformType::ePlatformLinux;
}

CWindowLinux::~CWindowLinux()
{
}

Window CWindowLinux::getWidow() const
{
    return m_window;
}
GLXFBConfig CWindowLinux::getFBConfig() const
{
    return m_glxFBConfigs;
}

Display* CWindowLinux::getDisplay() const
{
    return m_display;
}

XVisualInfo* CWindowLinux::getVisualInfo() const
{
    return m_visualInfo;
}

void CWindowLinux::minimize()
{
    XIconifyWindow(m_display, m_window, m_screen);
}

void CWindowLinux::maximize()
{
    XMapWindow(m_display, m_window);
}

void CWindowLinux::restore()
{
    XMapWindow(m_display, m_window);
}

void CWindowLinux::setFullScreen(bool value)
{
    if (m_param._isFullscreen == value)
    {
        return;
    }

    //TODO: fullsreen

    m_param._isFullscreen = value;
}


void CWindowLinux::setResizeble (bool value)
{
    if (m_param._isFullscreen || m_param._isResizeble == value)
    {
        return;
    }

    //TODO: resize window

    m_param._isResizeble = value;
}

void CWindowLinux::setCaption(const std::string& text)
{
    XTextProperty txt;

    c8* temp = const_cast<c8*>(text.c_str());
    if (Success == XmbTextListToTextProperty(m_display, &temp, 1, XStdICCTextStyle, &txt))
    {
        XSetWMName(m_display, m_window, &txt);
        XSetWMIconName(m_display, m_window, &txt);
        XFree(txt.value);
    }
}

void CWindowLinux::setPosition(const Dimension2D& pos)
{
    if (m_param._isFullscreen)
    {
        return;
    }

    XMoveWindow(m_display, m_window, pos.width,pos.height);

    m_param._position = pos;
}

bool CWindowLinux::isMaximized() const
{
    return !m_isMinimized;
}

bool CWindowLinux::isMinimized() const
{
    return m_isMinimized;
}

bool CWindowLinux::isActive() const
{
    return m_hasFocus && !m_isMinimized;
}

bool CWindowLinux::isFocused() const
{
    return m_hasFocus;
}

void CWindowLinux::create()
{
    LOG_INFO("Create Windows")

    m_display = XOpenDisplay(NULL);
    if (!m_display)
    {
        LOG_ERROR("Failed to open X display");
        return;
    }

    int major, minor;
    // FBConfigs were added in GLX version 1.3.
    if ( !glXQueryVersion(m_display, &major, &minor) || (( major == 1) && (minor < 3)) || (major < 1) )
    {
        LOG_ERROR("Invalid GLX version");
        return;
    }

    // Get a matching FB config
     static int fbAttribs[] =
    {
        GLX_X_RENDERABLE,       True,
        GLX_DRAWABLE_TYPE,      GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,        GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE,      GLX_TRUE_COLOR,
        GLX_RED_SIZE,           8,
        GLX_GREEN_SIZE,         8,
        GLX_BLUE_SIZE,          8,
        GLX_ALPHA_SIZE,         8,
        GLX_DEPTH_SIZE,         24,
        GLX_STENCIL_SIZE,       8,
        GLX_DOUBLEBUFFER,       True,
        //GLX_SAMPLE_BUFFERS,   True,
        //GLX_SAMPLES,          4,
        //GLX_SAMPLE_BUFFERS_ARB, True,
        //GLX_SAMPLES_ARB,        antiAlias,
        None
    };

    m_screen = DefaultScreen(m_display);
    int numConfigs = 0;

    GLXFBConfig *fbConfigs;
    fbConfigs = glXChooseFBConfig(m_display, m_screen, fbAttribs, &numConfigs);
    if (!fbConfigs)
    {
        LOG_ERROR("Failed to retrieve a framebuffer config");
        return;
    }

    // Pick the FB config/visual with the most samples per pixel
    int bestFbc = -1, worstFbc = -1, bestNumSamp = -1, worstNumSamp = 999;
    for (int i = 0; i < numConfigs; ++i)
    {
        XVisualInfo *visualInfo;
        visualInfo = glXGetVisualFromFBConfig(m_display, fbConfigs[i]);
        if (visualInfo)
        {
            int sampBuf, samples;
            glXGetFBConfigAttrib(m_display, fbConfigs[i], GLX_SAMPLE_BUFFERS, &sampBuf);
            glXGetFBConfigAttrib(m_display, fbConfigs[i], GLX_SAMPLES       , &samples);

            if ( bestFbc < 0 || sampBuf && samples > bestNumSamp )
            {
                bestFbc = i;
                bestNumSamp = samples;
            }

            if ( worstFbc < 0 || !sampBuf || samples < worstNumSamp )
            {
                worstFbc = i;
                worstNumSamp = samples;
            }
    }

        XFree(visualInfo);
    }

    m_glxFBConfigs = fbConfigs[bestFbc];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree( fbConfigs );

    // Get a visual
    XVisualInfo *visualInfo;
    visualInfo = glXGetVisualFromFBConfig(m_display, m_glxFBConfigs);

    m_visualInfo = visualInfo;

    XSetWindowAttributes winAttribs;

    Colormap colorMap;
    colorMap = XCreateColormap(m_display, RootWindow(m_display, visualInfo->screen), visualInfo->visual, AllocNone);

    winAttribs.colormap          = colorMap;
    winAttribs.background_pixmap = None ;
    winAttribs.border_pixel      = 0;
    winAttribs.event_mask        = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;
    //winAttribs.bit_gravity       = StaticGravity;
    //winAttribs.background_pixel  = WhitePixel(m_display, m_screen);
    //winAttribs.override_redirect = m_param.isFullscreen;

    m_window = XCreateWindow(m_display, RootWindow(m_display, visualInfo->screen),
                            0, 0, m_param._size.width, m_param._size.height, 0, visualInfo->depth, InputOutput,
                            visualInfo->visual, CWBorderPixel|CWColormap|CWEventMask, &winAttribs);
    if ( !m_window )
    {
        LOG_ERROR("Failed to create window");
        return;
    }

    XStoreName(m_display, m_window, "V3DLinux");

    LOG_INFO("Window Size (%d, %d)", m_param._size.width, m_param._size.height);

    XMapWindow(m_display, m_window);
    XFlush(m_display);

    CWindowLinux::addKeyCodes();
}

void CWindowLinux::close()
{
    glXMakeCurrent(m_display, 0, 0);
    glXDestroyContext(m_display, m_glxContext);

    XDestroyWindow(m_display, m_window);
    XCloseDisplay(m_display);

    m_close = true;
}

bool CWindowLinux::begin()
{
    if (m_close)
    {
        return false;
    }

    while (XPending(m_display) > 0)
    {
        XEvent xevent;
        XNextEvent(m_display, &xevent);

        switch (xevent.type)
        {

        case ConfigureNotify:
            //TODO: Change windows size
            break;

        case MapNotify:
            m_isMinimized = false;
            break;

        case UnmapNotify:
            m_isMinimized = true;
            break;

        case FocusIn:
            m_hasFocus = true;
            break;

        case FocusOut:
            m_hasFocus = false;
            break;

        case KeyPress:
        {
            KeySym mp;
            c8 buf;
            XLookupString(&xevent.xkey, &buf, sizeof(buf), &mp, NULL);

            SKeyCodes& keys = const_cast<SKeyCodes&>(WINDOW->getKeyCodes());

            SKeyboardInputEventPtr event = std::make_shared<SKeyboardInputEvent>();
            event->_event = eKeyboardPressDown;
            event->_key = keys.get(mp);
            event->_character = (c8)mp;

            INPUT_EVENTS->pushEvent(event);

            break;
        }

        case KeyRelease:
        {
            KeySym mp;
            c8 buf;
            XLookupString(&xevent.xkey, &buf, sizeof(buf), &mp, NULL);

            SKeyCodes& keys = const_cast<SKeyCodes&>(WINDOW->getKeyCodes());

            SKeyboardInputEventPtr event = std::make_shared<SKeyboardInputEvent>();
            event->_event = eKeyboardPressUp;
            event->_key = keys.get(mp);
            event->_character = (c8)mp;

            INPUT_EVENTS->pushEvent(event);

            break;
        }

        case MotionNotify:
        {
            SMouseInputEventPtr event = std::make_shared<SMouseInputEvent>();
            event->_event = eMouseMoved;
            event->_position.width = xevent.xbutton.x;
            event->_position.height = xevent.xbutton.y;

            INPUT_EVENTS->pushEvent(event);

            break;
        }

        case ButtonPress:
        case ButtonRelease:
        {
           SMouseInputEventPtr event = std::make_shared<SMouseInputEvent>();
           event->_position.width = xevent.xbutton.x;
           event->_position.height = xevent.xbutton.y;

           switch(xevent.xbutton.button)
           {
           case  Button1:
               event->_event = (xevent.type == ButtonPress) ? eLeftMousePressedDown : eLeftMousePressedUp;
               break;

           case  Button2:
               event->_event = (xevent.type == ButtonPress) ? eRightMousePressedDown : eRightMousePressedUp;
               break;

           case  Button3:
               event->_event = (xevent.type == ButtonPress) ? eMiddleMousePressedDown : eMiddleMousePressedUp;
               break;

           case  Button4:
               if (xevent.type == ButtonPress)
               {
                   event->_event = eMouseWheel;
                   event->_wheel = 1.0f;
               }
               break;

           case  Button5:
               if (xevent.type == ButtonPress)
               {
                   event->_event = eMouseWheel;
                   event->_wheel = -1.0f;
               }
               break;
           }

           INPUT_EVENTS->pushEvent(event);

           break;
        }

        case MappingNotify:
        {
            XRefreshKeyboardMapping (&xevent.xmapping);

            break;

        }

        case ClientMessage:
        {
            c8 *atom = XGetAtomName(m_display, xevent.xclient.message_type);
            //TODO: user messages

            break;
        }

        case SelectionRequest:
        {
//            XEvent respond;
//            XSelectionRequestEvent *req = &(xevent.xselectionrequest);
//            if (req->target == XA_STRING)
//            {
//                XChangeProperty (m_display, req->requestor, req->property, req->target, 8, // format
//                    PropModeReplace,(u8*)Clipboard.c_str(), Clipboard.size());
//                respond.xselection.property = req->property;
//            }
//            else if (req->target == X_ATOM_TARGETS)
//            {
//                long data[2];
//                data[0] = X_ATOM_TEXT;
//                data[1] = XA_STRING;

//                XChangeProperty (m_display, req->requestor, req->property, req->target, 8,
//                    PropModeReplace, (u8*)&data, sizeof(data));
//                respond.xselection.property = req->property;
//            }
//            else
//            {
//                respond.xselection.property= None;
//            }
//            respond.xselection.type= SelectionNotify;
//            respond.xselection.display= req->display;
//            respond.xselection.requestor= req->requestor;
//            respond.xselection.selection=req->selection;
//            respond.xselection.target= req->target;
//            respond.xselection.time = req->time;

//            XSendEvent (m_display, req->requestor, 0, 0, &respond);
//            XFlush (m_display);

            break;
        }

        default:
            break;
        }
    }

    return true;
}

bool CWindowLinux::end()
{
    glXSwapBuffers (m_display, m_window);

    return true;
}

void CWindowLinux::addKeyCodes()
{
    m_keys.add(eKeyUknown,   XK_Linefeed);
    m_keys.add(eKeyUknown,   XK_Sys_Req);
    m_keys.add(eKeyUknown,   XK_KP_Equal);
    m_keys.add(eKeyUknown,   XK_exclam);
    m_keys.add(eKeyUknown,   XK_quotedbl);
    m_keys.add(eKeyUknown,   XK_section);
    m_keys.add(eKeyUknown,   XK_numbersign);
    m_keys.add(eKeyUknown,   XK_dollar);
    m_keys.add(eKeyUknown,   XK_percent);
    m_keys.add(eKeyUknown,   XK_ampersand);
    m_keys.add(eKeyUknown,   XK_apostrophe);
    m_keys.add(eKeyUknown,   XK_parenleft);
    m_keys.add(eKeyUknown,   XK_parenright);
    m_keys.add(eKeyUknown,   XK_asterisk);
    m_keys.add(eKeyUknown,   XK_slash);
    m_keys.add(eKeyUknown,   XK_colon);
    m_keys.add(eKeyUknown,   XK_semicolon);
    m_keys.add(eKeyUknown,   XK_less);
    m_keys.add(eKeyUknown,   XK_equal);
    m_keys.add(eKeyUknown,   XK_greater);
    m_keys.add(eKeyUknown,   XK_question);
    m_keys.add(eKeyUknown,   XK_at);
    m_keys.add(eKeyUknown,   XK_mu);
    m_keys.add(eKeyUknown,   XK_EuroSign);
    m_keys.add(eKeyUknown,   XK_Adiaeresis);
    m_keys.add(eKeyUknown,   XK_Odiaeresis);
    m_keys.add(eKeyUknown,   XK_Udiaeresis);
    m_keys.add(eKeyUknown,   XK_bracketleft);
    m_keys.add(eKeyUknown,   XK_backslash);
    m_keys.add(eKeyUknown,   XK_bracketright);
    m_keys.add(eKeyUknown,   XK_asciicircum);
    m_keys.add(eKeyUknown,   XK_degree);
    m_keys.add(eKeyUknown,   XK_underscore);
    m_keys.add(eKeyUknown,   XK_grave);
    m_keys.add(eKeyUknown,   XK_acute);
    m_keys.add(eKeyUknown,   XK_quoteleft);
    m_keys.add(eKeyUknown,   XK_ssharp);
    m_keys.add(eKeyUknown,   XK_adiaeresis);
    m_keys.add(eKeyUknown,   XK_odiaeresis);
    m_keys.add(eKeyUknown,   XK_udiaeresis);
    m_keys.add(eKeyBack,     XK_BackSpace);
    m_keys.add(eKeyTab,      XK_Tab);
    m_keys.add(eKeyTab,      XK_ISO_Left_Tab);
    m_keys.add(eKeyTab,      XK_KP_Tab);
    m_keys.add(eKeyClear,    XK_Clear);
    m_keys.add(eKeyReturn,   XK_Return);
    m_keys.add(eKeyReturn,   XK_KP_Enter);
    m_keys.add(eKeyMenu,     XK_Menu);
    m_keys.add(eKeyPause,    XK_Pause);
    m_keys.add(eKeyCapital,  XK_Caps_Lock);
    m_keys.add(eKeyCapital,  XK_Shift_Lock);
    m_keys.add(eKeyEscape,   XK_Escape);
    m_keys.add(eKeySpace,    XK_KP_Space);
    m_keys.add(eKeySpace,    XK_space);
    m_keys.add(eKeyPrior,    XK_Prior);
    m_keys.add(eKeyPrior,    XK_KP_Prior);
    m_keys.add(eKeyPrior,    XK_Page_Up);
    m_keys.add(eKeyPrior,    XK_KP_Page_Up);
    m_keys.add(eKeyNext,     XK_Next);
    m_keys.add(eKeyNext,     XK_KP_Next);
    m_keys.add(eKeyNext,     XK_Page_Down);
    m_keys.add(eKeyNext,     XK_KP_Page_Down);
    m_keys.add(eKeyEnd,      XK_End);
    m_keys.add(eKeyEnd,      XK_KP_End);
    m_keys.add(eKeyHome,     XK_Begin);
    m_keys.add(eKeyHome,     XK_KP_Begin);
    m_keys.add(eKeyHome,     XK_Home);
    m_keys.add(eKeyHome,     XK_KP_Home);
    m_keys.add(eKeyLeft,     XK_Left);
    m_keys.add(eKeyLeft,     XK_KP_Left);
    m_keys.add(eKeyUp,       XK_Up);
    m_keys.add(eKeyUp,       XK_KP_Up);
    m_keys.add(eKeyRight,    XK_Right);
    m_keys.add(eKeyRight,    XK_KP_Right);
    m_keys.add(eKeyDown,     XK_Down);
    m_keys.add(eKeyDown,     XK_KP_Down);
    m_keys.add(eKeyPrint,    XK_Print);
    m_keys.add(eKeyInsert,   XK_Insert);
    m_keys.add(eKeyInsert,   XK_KP_Insert);
    m_keys.add(eKeyDelete,   XK_Delete);
    m_keys.add(eKeyDelete,   XK_KP_Delete);
    m_keys.add(eKeyKey_0,    XK_KP_0);
    m_keys.add(eKeyKey_1,    XK_KP_1);
    m_keys.add(eKeyKey_2,    XK_KP_2);
    m_keys.add(eKeyKey_3,    XK_KP_3);
    m_keys.add(eKeyKey_4,    XK_KP_4);
    m_keys.add(eKeyKey_5,    XK_KP_5);
    m_keys.add(eKeyKey_6,    XK_KP_6);
    m_keys.add(eKeyKey_7,    XK_KP_7);
    m_keys.add(eKeyKey_8,    XK_KP_8);
    m_keys.add(eKeyKey_9,    XK_KP_9);
    m_keys.add(eKeyKey_0,    XK_0);
    m_keys.add(eKeyKey_1,    XK_1);
    m_keys.add(eKeyKey_2,    XK_2);
    m_keys.add(eKeyKey_3,    XK_3);
    m_keys.add(eKeyKey_4,    XK_4);
    m_keys.add(eKeyKey_5,    XK_5);
    m_keys.add(eKeyKey_6,    XK_6);
    m_keys.add(eKeyKey_7,    XK_7);
    m_keys.add(eKeyKey_8,    XK_8);
    m_keys.add(eKeyKey_9,    XK_9);
    m_keys.add(eKeyKey_A,    XK_A);
    m_keys.add(eKeyKey_B,    XK_B);
    m_keys.add(eKeyKey_C,    XK_C);
    m_keys.add(eKeyKey_D,    XK_D);
    m_keys.add(eKeyKey_E,    XK_E);
    m_keys.add(eKeyKey_F,    XK_F);
    m_keys.add(eKeyKey_G,    XK_G);
    m_keys.add(eKeyKey_H,    XK_H);
    m_keys.add(eKeyKey_I,    XK_I);
    m_keys.add(eKeyKey_J,    XK_J);
    m_keys.add(eKeyKey_K,    XK_K);
    m_keys.add(eKeyKey_L,    XK_L);
    m_keys.add(eKeyKey_M,    XK_M);
    m_keys.add(eKeyKey_N,    XK_N);
    m_keys.add(eKeyKey_O,    XK_O);
    m_keys.add(eKeyKey_P,    XK_P);
    m_keys.add(eKeyKey_Q,    XK_Q);
    m_keys.add(eKeyKey_R,    XK_R);
    m_keys.add(eKeyKey_S,    XK_S);
    m_keys.add(eKeyKey_T,    XK_T);
    m_keys.add(eKeyKey_U,    XK_U);
    m_keys.add(eKeyKey_V,    XK_V);
    m_keys.add(eKeyKey_W,    XK_W);
    m_keys.add(eKeyKey_X,    XK_X);
    m_keys.add(eKeyKey_Y,    XK_Y);
    m_keys.add(eKeyKey_Z,    XK_Z);
    m_keys.add(eKeyKey_A,    XK_a);
    m_keys.add(eKeyKey_B,    XK_b);
    m_keys.add(eKeyKey_C,    XK_c);
    m_keys.add(eKeyKey_D,    XK_d);
    m_keys.add(eKeyKey_E,    XK_e);
    m_keys.add(eKeyKey_F,    XK_f);
    m_keys.add(eKeyKey_G,    XK_g);
    m_keys.add(eKeyKey_H,    XK_h);
    m_keys.add(eKeyKey_I,    XK_i);
    m_keys.add(eKeyKey_J,    XK_j);
    m_keys.add(eKeyKey_K,    XK_k);
    m_keys.add(eKeyKey_L,    XK_l);
    m_keys.add(eKeyKey_M,    XK_m);
    m_keys.add(eKeyKey_N,    XK_n);
    m_keys.add(eKeyKey_O,    XK_o);
    m_keys.add(eKeyKey_P,    XK_p);
    m_keys.add(eKeyKey_Q,    XK_q);
    m_keys.add(eKeyKey_R,    XK_r);
    m_keys.add(eKeyKey_S,    XK_s);
    m_keys.add(eKeyKey_T,    XK_t);
    m_keys.add(eKeyKey_U,    XK_u);
    m_keys.add(eKeyKey_V,    XK_v);
    m_keys.add(eKeyKey_W,    XK_w);
    m_keys.add(eKeyKey_X,    XK_x);
    m_keys.add(eKeyKey_Y,    XK_y);
    m_keys.add(eKeyKey_Z,    XK_z);
    m_keys.add(eKeyLWin,     XK_Meta_L);
    m_keys.add(eKeyRWin,     XK_Meta_R);
    m_keys.add(eKeyMultiply, XK_KP_Multiply);
    m_keys.add(eKeyAdd,      XK_KP_Add);
    m_keys.add(eKeySeparator,XK_KP_Separator);
    m_keys.add(eKeySubtract, XK_KP_Subtract);
    m_keys.add(eKeyDecimal,  XK_KP_Decimal);
    m_keys.add(eKeyDivide,   XK_KP_Divide);
    m_keys.add(eKeyF1,       XK_KP_F1);
    m_keys.add(eKeyF2,       XK_KP_F2);
    m_keys.add(eKeyF3,       XK_KP_F3);
    m_keys.add(eKeyF4,       XK_KP_F4);
    m_keys.add(eKeyF1,       XK_F1);
    m_keys.add(eKeyF2,       XK_F2);
    m_keys.add(eKeyF3,       XK_F3);
    m_keys.add(eKeyF4,       XK_F4);
    m_keys.add(eKeyF5,       XK_F5);
    m_keys.add(eKeyF6,       XK_F6);
    m_keys.add(eKeyF7,       XK_F7);
    m_keys.add(eKeyF8,       XK_F8);
    m_keys.add(eKeyF9,       XK_F9);
    m_keys.add(eKeyF10,      XK_F10);
    m_keys.add(eKeyF11,      XK_F11);
    m_keys.add(eKeyF12,      XK_F12);
    m_keys.add(eKeyNumlock,  XK_Num_Lock);
    m_keys.add(eKeyScroll,   XK_Scroll_Lock);
    m_keys.add(eKeyLShift,   XK_Shift_L);
    m_keys.add(eKeyRShift,   XK_Shift_R);
    m_keys.add(eKeyLControl, XK_Control_L);
    m_keys.add(eKeyRControl, XK_Control_R);
    m_keys.add(eKeyLMenu,    XK_Alt_L);
    m_keys.add(eKeyRMenu,    XK_Alt_R);
    m_keys.add(eKeyRMenu,    XK_ISO_Level3_Shift);
    m_keys.add(eKeyPlus,     XK_plus);
    m_keys.add(eKeyComma,    XK_comma);
    m_keys.add(eKeyMinus,    XK_minus);
    m_keys.add(eKeyPeriod,   XK_period);
}
