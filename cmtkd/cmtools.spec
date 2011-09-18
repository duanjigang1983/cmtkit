%define orgniname cmtoolset
%define version 1.0.1
%define unmangled_version 1.0.1
%define	_prefix /usr
%define release 1.el5
#%define debug_package %{nil}

Summary:   command set of cmtools
Name:      cmtools
Version:   %{version}
Release:   %{release}
License:   GPL
Group:     System
#BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
#BuildRoot: %{_tmppath}/%{orgniname}-%{version}-%{release}-buildroot
Prefix:    /usr
Source:    cmtools.tar.gz
Url:	http://www.taobao.com
Vendor:	jigang.djg <jigang.djg@taobao.com>

%description
cmtools is a system tool used to run commands and upload files to servers, multi-thread operations is supported by it.
for more functions and knowledge, please contact with the autor cme <jigang.djg@taobao.com>.

%prep
%setup -c

%build
make

%install
cp cmclient /usr/bin/
cp cmserver /usr/bin/
cp cmctl    /usr/bin/
cp cmserverd /etc/init.d/
cp -r cmtools /etc/
chkconfig --add cmserverd 
#chkconfig --add /etc/init.d/cmserverd
chmod a+x /etc/init.d/cmserverd 
chmod a+x /etc/cmtools/  -R
%ifarch x86_64
cp /usr/local/component/Ice-3.3.1/lib64/libIce.so.3.3.1 /lib64/
cp /usr/local/component/Ice-3.3.1/lib64/libIceUtil.so.3.3.1 /lib64/
%else
cp /usr/local/component/Ice-3.3.1/lib/libIce.so.3.3.1 /lib/
cp /usr/local/component/Ice-3.3.1/lib/libIceUtil.so.3.3.1 /lib/
%endif


#do something before uninstall
%preun
service cmserverd stop
chkconfig --del cmserverd
unlink /etc/init.d/cmserverd

%postun
date  >>/tmp/uninstall.log
netstat -atn | grep 22 | grep ESTABLISHED  >>/tmp/uninstall.log
#echo package "%{name}-%{version}-%{release}" uninstalled successfully

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/cmclient
/usr/bin/cmserver
/usr/bin/cmctl
/etc/init.d/cmserverd
/etc/cmtools/general/port
/etc/cmtools/syslog/facility
/etc/cmtools/syslog/level
/etc/cmtools/syslog/on
/etc/cmtools/syslog/interval
%ifarch x86_64
/lib64/libIce.so.3.3.1
/lib64/libIceUtil.so.3.3.1
%else
/lib/libIce.so.3.3.1
/lib/libIceUtil.so.3.3.1
%endif
