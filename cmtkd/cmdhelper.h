// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `cmdhelper.ice'

#ifndef __cmdhelper_h__
#define __cmdhelper_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace cmdhelper
{

class CmdMessageHandler;

}

}

namespace cmdhelper
{

class CmdMessageHandler;
bool operator==(const CmdMessageHandler&, const CmdMessageHandler&);
bool operator<(const CmdMessageHandler&, const CmdMessageHandler&);

}

namespace IceInternal
{

::Ice::Object* upCast(::cmdhelper::CmdMessageHandler*);
::IceProxy::Ice::Object* upCast(::IceProxy::cmdhelper::CmdMessageHandler*);

}

namespace cmdhelper
{

typedef ::IceInternal::Handle< ::cmdhelper::CmdMessageHandler> CmdMessageHandlerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::cmdhelper::CmdMessageHandler> CmdMessageHandlerPrx;

void __read(::IceInternal::BasicStream*, CmdMessageHandlerPrx&);
void __patch__CmdMessageHandlerPtr(void*, ::Ice::ObjectPtr&);

}

namespace cmdhelper
{

typedef ::std::vector< ::std::string> StringArray;

typedef ::std::vector< ::cmdhelper::StringArray> StringArray2;
void __writeStringArray2(::IceInternal::BasicStream*, const ::cmdhelper::StringArray*, const ::cmdhelper::StringArray*);
void __readStringArray2(::IceInternal::BasicStream*, StringArray2&);

typedef ::std::vector< ::Ice::Byte> byteArray;

typedef ::std::vector< ::Ice::Long> ipArray;

struct msghead
{
    ::std::string index;
    ::Ice::Long hostaddr;
    ::cmdhelper::ipArray srchost;
    ::Ice::Int hostport;
    ::Ice::Int taskid;
    ::Ice::Int msgtype;
    ::Ice::Long commandid;
    ::Ice::Long timestamp;
    ::Ice::Long localaddr;
    ::std::string file;
    ::std::string dstfile;
    ::std::string filemd5;
    ::Ice::Long filesize;
    ::Ice::Int nret;
    ::Ice::Int runmode;
    ::Ice::Long stmode;
    ::Ice::Int uid;
    ::Ice::Int gid;
    ::std::string username;
    ::std::string login;
    ::std::string remotefile;
    ::std::string localfile;

    bool operator==(const msghead&) const;
    bool operator<(const msghead&) const;
    bool operator!=(const msghead& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const msghead& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const msghead& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const msghead& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct CommandMessage
{
    ::cmdhelper::msghead head;
    ::std::string cmd;
    ::cmdhelper::StringArray result;
    ::cmdhelper::byteArray filedata;
    ::cmdhelper::byteArray fileinfo;

    bool operator==(const CommandMessage&) const;
    bool operator<(const CommandMessage&) const;
    bool operator!=(const CommandMessage& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const CommandMessage& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const CommandMessage& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const CommandMessage& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct command
{
    ::Ice::Long host;
    ::Ice::Int port;
    ::Ice::Int cmdtype;
    ::Ice::Int cmdid;
    ::Ice::Int pid;
    ::Ice::Long clientaddr;
    ::Ice::Long clientid;
    ::cmdhelper::byteArray data;

    bool operator==(const command&) const;
    bool operator<(const command&) const;
    bool operator!=(const command& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const command& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const command& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const command& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct response
{
    ::std::string filename;
    ::cmdhelper::byteArray filedata;
    ::std::string infoname;
    ::cmdhelper::byteArray infodata;
    ::Ice::Int more;

    bool operator==(const response&) const;
    bool operator<(const response&) const;
    bool operator!=(const response& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const response& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const response& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const response& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct magicmsg
{
    ::Ice::Int type;
    ::cmdhelper::byteArray data;

    bool operator==(const magicmsg&) const;
    bool operator<(const magicmsg&) const;
    bool operator!=(const magicmsg& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const magicmsg& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const magicmsg& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const magicmsg& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

}

namespace IceProxy
{

namespace cmdhelper
{

class CmdMessageHandler : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::Int ProcessMagicMessage(const ::cmdhelper::magicmsg& msg)
    {
        return ProcessMagicMessage(msg, 0);
    }
    ::Ice::Int ProcessMagicMessage(const ::cmdhelper::magicmsg& msg, const ::Ice::Context& __ctx)
    {
        return ProcessMagicMessage(msg, &__ctx);
    }
    
private:

    ::Ice::Int ProcessMagicMessage(const ::cmdhelper::magicmsg&, const ::Ice::Context*);
    
public:

    ::cmdhelper::CommandMessage ProcessMessage(const ::cmdhelper::CommandMessage& msg)
    {
        return ProcessMessage(msg, 0);
    }
    ::cmdhelper::CommandMessage ProcessMessage(const ::cmdhelper::CommandMessage& msg, const ::Ice::Context& __ctx)
    {
        return ProcessMessage(msg, &__ctx);
    }
    
private:

    ::cmdhelper::CommandMessage ProcessMessage(const ::cmdhelper::CommandMessage&, const ::Ice::Context*);
    
public:

    ::cmdhelper::command ProcessCmd(const ::cmdhelper::command& cmd)
    {
        return ProcessCmd(cmd, 0);
    }
    ::cmdhelper::command ProcessCmd(const ::cmdhelper::command& cmd, const ::Ice::Context& __ctx)
    {
        return ProcessCmd(cmd, &__ctx);
    }
    
private:

    ::cmdhelper::command ProcessCmd(const ::cmdhelper::command&, const ::Ice::Context*);
    
public:

    ::cmdhelper::response ProcessNotify(const ::cmdhelper::command& cmd)
    {
        return ProcessNotify(cmd, 0);
    }
    ::cmdhelper::response ProcessNotify(const ::cmdhelper::command& cmd, const ::Ice::Context& __ctx)
    {
        return ProcessNotify(cmd, &__ctx);
    }
    
private:

    ::cmdhelper::response ProcessNotify(const ::cmdhelper::command&, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<CmdMessageHandler> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<CmdMessageHandler*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<CmdMessageHandler*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace cmdhelper
{

class CmdMessageHandler : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Ice::Int ProcessMagicMessage(const ::cmdhelper::magicmsg&, const ::Ice::Context*) = 0;

    virtual ::cmdhelper::CommandMessage ProcessMessage(const ::cmdhelper::CommandMessage&, const ::Ice::Context*) = 0;

    virtual ::cmdhelper::command ProcessCmd(const ::cmdhelper::command&, const ::Ice::Context*) = 0;

    virtual ::cmdhelper::response ProcessNotify(const ::cmdhelper::command&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace cmdhelper
{

class CmdMessageHandler : virtual public ::IceDelegate::cmdhelper::CmdMessageHandler,
                          virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Ice::Int ProcessMagicMessage(const ::cmdhelper::magicmsg&, const ::Ice::Context*);

    virtual ::cmdhelper::CommandMessage ProcessMessage(const ::cmdhelper::CommandMessage&, const ::Ice::Context*);

    virtual ::cmdhelper::command ProcessCmd(const ::cmdhelper::command&, const ::Ice::Context*);

    virtual ::cmdhelper::response ProcessNotify(const ::cmdhelper::command&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace cmdhelper
{

class CmdMessageHandler : virtual public ::IceDelegate::cmdhelper::CmdMessageHandler,
                          virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Ice::Int ProcessMagicMessage(const ::cmdhelper::magicmsg&, const ::Ice::Context*);

    virtual ::cmdhelper::CommandMessage ProcessMessage(const ::cmdhelper::CommandMessage&, const ::Ice::Context*);

    virtual ::cmdhelper::command ProcessCmd(const ::cmdhelper::command&, const ::Ice::Context*);

    virtual ::cmdhelper::response ProcessNotify(const ::cmdhelper::command&, const ::Ice::Context*);
};

}

}

namespace cmdhelper
{

class CmdMessageHandler : virtual public ::Ice::Object
{
public:

    typedef CmdMessageHandlerPrx ProxyType;
    typedef CmdMessageHandlerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::Int ProcessMagicMessage(const ::cmdhelper::magicmsg&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___ProcessMagicMessage(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::cmdhelper::CommandMessage ProcessMessage(const ::cmdhelper::CommandMessage&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___ProcessMessage(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::cmdhelper::command ProcessCmd(const ::cmdhelper::command&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___ProcessCmd(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::cmdhelper::response ProcessNotify(const ::cmdhelper::command&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___ProcessNotify(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
