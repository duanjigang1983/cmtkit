// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `cmtkp.ice'

#include "cmtkp.h"
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

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

static const ::std::string __cmtkp__CmdMessageHandler__ProcessMagicMessage_name = "ProcessMagicMessage";

static const ::std::string __cmtkp__CmdMessageHandler__ProcessMessage_name = "ProcessMessage";

::Ice::Object* IceInternal::upCast(::cmtkp::CmdMessageHandler* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::cmtkp::CmdMessageHandler* p) { return p; }

void
cmtkp::__read(::IceInternal::BasicStream* __is, ::cmtkp::CmdMessageHandlerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::cmtkp::CmdMessageHandler;
        v->__copyFrom(proxy);
    }
}

void
cmtkp::__writeStringArray2(::IceInternal::BasicStream* __os, const ::cmtkp::StringArray* begin, const ::cmtkp::StringArray* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        if(begin[i].size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            __os->write(&begin[i][0], &begin[i][0] + begin[i].size());
        }
    }
}

void
cmtkp::__readStringArray2(::IceInternal::BasicStream* __is, ::cmtkp::StringArray2& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        __is->read(v[i]);
        __is->endElement();
    }
    __is->endSeq(sz);
}

bool
cmtkp::msghead::operator==(const msghead& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(index != __rhs.index)
    {
        return false;
    }
    if(hostaddr != __rhs.hostaddr)
    {
        return false;
    }
    if(srchost != __rhs.srchost)
    {
        return false;
    }
    if(hostport != __rhs.hostport)
    {
        return false;
    }
    if(taskid != __rhs.taskid)
    {
        return false;
    }
    if(msgtype != __rhs.msgtype)
    {
        return false;
    }
    if(commandid != __rhs.commandid)
    {
        return false;
    }
    if(timestamp != __rhs.timestamp)
    {
        return false;
    }
    if(file != __rhs.file)
    {
        return false;
    }
    if(dstfile != __rhs.dstfile)
    {
        return false;
    }
    if(filemd5 != __rhs.filemd5)
    {
        return false;
    }
    if(filesize != __rhs.filesize)
    {
        return false;
    }
    if(nret != __rhs.nret)
    {
        return false;
    }
    if(runmode != __rhs.runmode)
    {
        return false;
    }
    if(remotefile != __rhs.remotefile)
    {
        return false;
    }
    if(localfile != __rhs.localfile)
    {
        return false;
    }
    return true;
}

bool
cmtkp::msghead::operator<(const msghead& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(index < __rhs.index)
    {
        return true;
    }
    else if(__rhs.index < index)
    {
        return false;
    }
    if(hostaddr < __rhs.hostaddr)
    {
        return true;
    }
    else if(__rhs.hostaddr < hostaddr)
    {
        return false;
    }
    if(srchost < __rhs.srchost)
    {
        return true;
    }
    else if(__rhs.srchost < srchost)
    {
        return false;
    }
    if(hostport < __rhs.hostport)
    {
        return true;
    }
    else if(__rhs.hostport < hostport)
    {
        return false;
    }
    if(taskid < __rhs.taskid)
    {
        return true;
    }
    else if(__rhs.taskid < taskid)
    {
        return false;
    }
    if(msgtype < __rhs.msgtype)
    {
        return true;
    }
    else if(__rhs.msgtype < msgtype)
    {
        return false;
    }
    if(commandid < __rhs.commandid)
    {
        return true;
    }
    else if(__rhs.commandid < commandid)
    {
        return false;
    }
    if(timestamp < __rhs.timestamp)
    {
        return true;
    }
    else if(__rhs.timestamp < timestamp)
    {
        return false;
    }
    if(file < __rhs.file)
    {
        return true;
    }
    else if(__rhs.file < file)
    {
        return false;
    }
    if(dstfile < __rhs.dstfile)
    {
        return true;
    }
    else if(__rhs.dstfile < dstfile)
    {
        return false;
    }
    if(filemd5 < __rhs.filemd5)
    {
        return true;
    }
    else if(__rhs.filemd5 < filemd5)
    {
        return false;
    }
    if(filesize < __rhs.filesize)
    {
        return true;
    }
    else if(__rhs.filesize < filesize)
    {
        return false;
    }
    if(nret < __rhs.nret)
    {
        return true;
    }
    else if(__rhs.nret < nret)
    {
        return false;
    }
    if(runmode < __rhs.runmode)
    {
        return true;
    }
    else if(__rhs.runmode < runmode)
    {
        return false;
    }
    if(remotefile < __rhs.remotefile)
    {
        return true;
    }
    else if(__rhs.remotefile < remotefile)
    {
        return false;
    }
    if(localfile < __rhs.localfile)
    {
        return true;
    }
    else if(__rhs.localfile < localfile)
    {
        return false;
    }
    return false;
}

void
cmtkp::msghead::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(index);
    __os->write(hostaddr);
    if(srchost.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&srchost[0], &srchost[0] + srchost.size());
    }
    __os->write(hostport);
    __os->write(taskid);
    __os->write(msgtype);
    __os->write(commandid);
    __os->write(timestamp);
    __os->write(file);
    __os->write(dstfile);
    __os->write(filemd5);
    __os->write(filesize);
    __os->write(nret);
    __os->write(runmode);
    __os->write(remotefile);
    __os->write(localfile);
}

void
cmtkp::msghead::__read(::IceInternal::BasicStream* __is)
{
    __is->read(index);
    __is->read(hostaddr);
    __is->read(srchost);
    __is->read(hostport);
    __is->read(taskid);
    __is->read(msgtype);
    __is->read(commandid);
    __is->read(timestamp);
    __is->read(file);
    __is->read(dstfile);
    __is->read(filemd5);
    __is->read(filesize);
    __is->read(nret);
    __is->read(runmode);
    __is->read(remotefile);
    __is->read(localfile);
}

bool
cmtkp::CommandMessage::operator==(const CommandMessage& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(head != __rhs.head)
    {
        return false;
    }
    if(cmd != __rhs.cmd)
    {
        return false;
    }
    if(result != __rhs.result)
    {
        return false;
    }
    if(filedata != __rhs.filedata)
    {
        return false;
    }
    if(fileinfo != __rhs.fileinfo)
    {
        return false;
    }
    return true;
}

bool
cmtkp::CommandMessage::operator<(const CommandMessage& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(head < __rhs.head)
    {
        return true;
    }
    else if(__rhs.head < head)
    {
        return false;
    }
    if(cmd < __rhs.cmd)
    {
        return true;
    }
    else if(__rhs.cmd < cmd)
    {
        return false;
    }
    if(result < __rhs.result)
    {
        return true;
    }
    else if(__rhs.result < result)
    {
        return false;
    }
    if(filedata < __rhs.filedata)
    {
        return true;
    }
    else if(__rhs.filedata < filedata)
    {
        return false;
    }
    if(fileinfo < __rhs.fileinfo)
    {
        return true;
    }
    else if(__rhs.fileinfo < fileinfo)
    {
        return false;
    }
    return false;
}

void
cmtkp::CommandMessage::__write(::IceInternal::BasicStream* __os) const
{
    head.__write(__os);
    __os->write(cmd);
    if(result.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&result[0], &result[0] + result.size());
    }
    if(filedata.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&filedata[0], &filedata[0] + filedata.size());
    }
    if(fileinfo.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&fileinfo[0], &fileinfo[0] + fileinfo.size());
    }
}

void
cmtkp::CommandMessage::__read(::IceInternal::BasicStream* __is)
{
    head.__read(__is);
    __is->read(cmd);
    __is->read(result);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___filedata;
    __is->read(___filedata);
    ::std::vector< ::Ice::Byte>(___filedata.first, ___filedata.second).swap(filedata);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___fileinfo;
    __is->read(___fileinfo);
    ::std::vector< ::Ice::Byte>(___fileinfo.first, ___fileinfo.second).swap(fileinfo);
}

bool
cmtkp::command::operator==(const command& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(host != __rhs.host)
    {
        return false;
    }
    if(port != __rhs.port)
    {
        return false;
    }
    if(cmdtype != __rhs.cmdtype)
    {
        return false;
    }
    if(cmdid != __rhs.cmdid)
    {
        return false;
    }
    if(pid != __rhs.pid)
    {
        return false;
    }
    if(clientaddr != __rhs.clientaddr)
    {
        return false;
    }
    if(clientid != __rhs.clientid)
    {
        return false;
    }
    if(data != __rhs.data)
    {
        return false;
    }
    return true;
}

bool
cmtkp::command::operator<(const command& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(host < __rhs.host)
    {
        return true;
    }
    else if(__rhs.host < host)
    {
        return false;
    }
    if(port < __rhs.port)
    {
        return true;
    }
    else if(__rhs.port < port)
    {
        return false;
    }
    if(cmdtype < __rhs.cmdtype)
    {
        return true;
    }
    else if(__rhs.cmdtype < cmdtype)
    {
        return false;
    }
    if(cmdid < __rhs.cmdid)
    {
        return true;
    }
    else if(__rhs.cmdid < cmdid)
    {
        return false;
    }
    if(pid < __rhs.pid)
    {
        return true;
    }
    else if(__rhs.pid < pid)
    {
        return false;
    }
    if(clientaddr < __rhs.clientaddr)
    {
        return true;
    }
    else if(__rhs.clientaddr < clientaddr)
    {
        return false;
    }
    if(clientid < __rhs.clientid)
    {
        return true;
    }
    else if(__rhs.clientid < clientid)
    {
        return false;
    }
    if(data < __rhs.data)
    {
        return true;
    }
    else if(__rhs.data < data)
    {
        return false;
    }
    return false;
}

void
cmtkp::command::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(host);
    __os->write(port);
    __os->write(cmdtype);
    __os->write(cmdid);
    __os->write(pid);
    __os->write(clientaddr);
    __os->write(clientid);
    if(data.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&data[0], &data[0] + data.size());
    }
}

void
cmtkp::command::__read(::IceInternal::BasicStream* __is)
{
    __is->read(host);
    __is->read(port);
    __is->read(cmdtype);
    __is->read(cmdid);
    __is->read(pid);
    __is->read(clientaddr);
    __is->read(clientid);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___data;
    __is->read(___data);
    ::std::vector< ::Ice::Byte>(___data.first, ___data.second).swap(data);
}

bool
cmtkp::response::operator==(const response& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(filename != __rhs.filename)
    {
        return false;
    }
    if(filedata != __rhs.filedata)
    {
        return false;
    }
    if(infoname != __rhs.infoname)
    {
        return false;
    }
    if(infodata != __rhs.infodata)
    {
        return false;
    }
    if(more != __rhs.more)
    {
        return false;
    }
    return true;
}

bool
cmtkp::response::operator<(const response& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(filename < __rhs.filename)
    {
        return true;
    }
    else if(__rhs.filename < filename)
    {
        return false;
    }
    if(filedata < __rhs.filedata)
    {
        return true;
    }
    else if(__rhs.filedata < filedata)
    {
        return false;
    }
    if(infoname < __rhs.infoname)
    {
        return true;
    }
    else if(__rhs.infoname < infoname)
    {
        return false;
    }
    if(infodata < __rhs.infodata)
    {
        return true;
    }
    else if(__rhs.infodata < infodata)
    {
        return false;
    }
    if(more < __rhs.more)
    {
        return true;
    }
    else if(__rhs.more < more)
    {
        return false;
    }
    return false;
}

void
cmtkp::response::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(filename);
    if(filedata.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&filedata[0], &filedata[0] + filedata.size());
    }
    __os->write(infoname);
    if(infodata.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&infodata[0], &infodata[0] + infodata.size());
    }
    __os->write(more);
}

void
cmtkp::response::__read(::IceInternal::BasicStream* __is)
{
    __is->read(filename);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___filedata;
    __is->read(___filedata);
    ::std::vector< ::Ice::Byte>(___filedata.first, ___filedata.second).swap(filedata);
    __is->read(infoname);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___infodata;
    __is->read(___infodata);
    ::std::vector< ::Ice::Byte>(___infodata.first, ___infodata.second).swap(infodata);
    __is->read(more);
}

bool
cmtkp::magicmsg::operator==(const magicmsg& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(type != __rhs.type)
    {
        return false;
    }
    if(data != __rhs.data)
    {
        return false;
    }
    return true;
}

bool
cmtkp::magicmsg::operator<(const magicmsg& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(type < __rhs.type)
    {
        return true;
    }
    else if(__rhs.type < type)
    {
        return false;
    }
    if(data < __rhs.data)
    {
        return true;
    }
    else if(__rhs.data < data)
    {
        return false;
    }
    return false;
}

void
cmtkp::magicmsg::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(type);
    if(data.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&data[0], &data[0] + data.size());
    }
}

void
cmtkp::magicmsg::__read(::IceInternal::BasicStream* __is)
{
    __is->read(type);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___data;
    __is->read(___data);
    ::std::vector< ::Ice::Byte>(___data.first, ___data.second).swap(data);
}

::Ice::Int
IceProxy::cmtkp::CmdMessageHandler::ProcessMagicMessage(const ::cmtkp::magicmsg& msg, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__cmtkp__CmdMessageHandler__ProcessMagicMessage_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::cmtkp::CmdMessageHandler* __del = dynamic_cast< ::IceDelegate::cmtkp::CmdMessageHandler*>(__delBase.get());
            return __del->ProcessMagicMessage(msg, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::cmtkp::CommandMessage
IceProxy::cmtkp::CmdMessageHandler::ProcessMessage(const ::cmtkp::CommandMessage& msg, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__cmtkp__CmdMessageHandler__ProcessMessage_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::cmtkp::CmdMessageHandler* __del = dynamic_cast< ::IceDelegate::cmtkp::CmdMessageHandler*>(__delBase.get());
            return __del->ProcessMessage(msg, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::cmtkp::CmdMessageHandler::ice_staticId()
{
    return ::cmtkp::CmdMessageHandler::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::cmtkp::CmdMessageHandler::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::cmtkp::CmdMessageHandler);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::cmtkp::CmdMessageHandler::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::cmtkp::CmdMessageHandler);
}

::IceProxy::Ice::Object*
IceProxy::cmtkp::CmdMessageHandler::__newInstance() const
{
    return new CmdMessageHandler;
}

::Ice::Int
IceDelegateM::cmtkp::CmdMessageHandler::ProcessMagicMessage(const ::cmtkp::magicmsg& msg, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __cmtkp__CmdMessageHandler__ProcessMagicMessage_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        msg.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::Ice::Int __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::cmtkp::CommandMessage
IceDelegateM::cmtkp::CmdMessageHandler::ProcessMessage(const ::cmtkp::CommandMessage& msg, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __cmtkp__CmdMessageHandler__ProcessMessage_name, ::Ice::Idempotent, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        msg.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::cmtkp::CommandMessage __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __ret.__read(__is);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateD::cmtkp::CmdMessageHandler::ProcessMagicMessage(const ::cmtkp::magicmsg& msg, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::cmtkp::magicmsg& msg, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_msg(msg)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::cmtkp::CmdMessageHandler* servant = dynamic_cast< ::cmtkp::CmdMessageHandler*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->ProcessMagicMessage(_m_msg, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int& _result;
        const ::cmtkp::magicmsg& _m_msg;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __cmtkp__CmdMessageHandler__ProcessMagicMessage_name, ::Ice::Idempotent, __context);
    ::Ice::Int __result;
    try
    {
        _DirectI __direct(__result, msg, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::cmtkp::CommandMessage
IceDelegateD::cmtkp::CmdMessageHandler::ProcessMessage(const ::cmtkp::CommandMessage& msg, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::cmtkp::CommandMessage& __result, const ::cmtkp::CommandMessage& msg, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_msg(msg)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::cmtkp::CmdMessageHandler* servant = dynamic_cast< ::cmtkp::CmdMessageHandler*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->ProcessMessage(_m_msg, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::cmtkp::CommandMessage& _result;
        const ::cmtkp::CommandMessage& _m_msg;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __cmtkp__CmdMessageHandler__ProcessMessage_name, ::Ice::Idempotent, __context);
    ::cmtkp::CommandMessage __result;
    try
    {
        _DirectI __direct(__result, msg, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::ObjectPtr
cmtkp::CmdMessageHandler::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __cmtkp__CmdMessageHandler_ids[2] =
{
    "::Ice::Object",
    "::cmtkp::CmdMessageHandler"
};

bool
cmtkp::CmdMessageHandler::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__cmtkp__CmdMessageHandler_ids, __cmtkp__CmdMessageHandler_ids + 2, _s);
}

::std::vector< ::std::string>
cmtkp::CmdMessageHandler::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__cmtkp__CmdMessageHandler_ids[0], &__cmtkp__CmdMessageHandler_ids[2]);
}

const ::std::string&
cmtkp::CmdMessageHandler::ice_id(const ::Ice::Current&) const
{
    return __cmtkp__CmdMessageHandler_ids[1];
}

const ::std::string&
cmtkp::CmdMessageHandler::ice_staticId()
{
    return __cmtkp__CmdMessageHandler_ids[1];
}

::Ice::DispatchStatus
cmtkp::CmdMessageHandler::___ProcessMagicMessage(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::cmtkp::magicmsg msg;
    msg.__read(__is);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = ProcessMagicMessage(msg, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
cmtkp::CmdMessageHandler::___ProcessMessage(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::cmtkp::CommandMessage msg;
    msg.__read(__is);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::cmtkp::CommandMessage __ret = ProcessMessage(msg, __current);
    __ret.__write(__os);
    return ::Ice::DispatchOK;
}

static ::std::string __cmtkp__CmdMessageHandler_all[] =
{
    "ProcessMagicMessage",
    "ProcessMessage",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
cmtkp::CmdMessageHandler::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__cmtkp__CmdMessageHandler_all, __cmtkp__CmdMessageHandler_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __cmtkp__CmdMessageHandler_all)
    {
        case 0:
        {
            return ___ProcessMagicMessage(in, current);
        }
        case 1:
        {
            return ___ProcessMessage(in, current);
        }
        case 2:
        {
            return ___ice_id(in, current);
        }
        case 3:
        {
            return ___ice_ids(in, current);
        }
        case 4:
        {
            return ___ice_isA(in, current);
        }
        case 5:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
cmtkp::CmdMessageHandler::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
cmtkp::CmdMessageHandler::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
cmtkp::CmdMessageHandler::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type cmtkp::CmdMessageHandler was not generated with stream support";
    throw ex;
}

void
cmtkp::CmdMessageHandler::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type cmtkp::CmdMessageHandler was not generated with stream support";
    throw ex;
}

void 
cmtkp::__patch__CmdMessageHandlerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::cmtkp::CmdMessageHandlerPtr* p = static_cast< ::cmtkp::CmdMessageHandlerPtr*>(__addr);
    assert(p);
    *p = ::cmtkp::CmdMessageHandlerPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::cmtkp::CmdMessageHandler::ice_staticId(), v->ice_id());
    }
}

bool
cmtkp::operator==(const ::cmtkp::CmdMessageHandler& l, const ::cmtkp::CmdMessageHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
cmtkp::operator<(const ::cmtkp::CmdMessageHandler& l, const ::cmtkp::CmdMessageHandler& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
