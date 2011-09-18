%define orgniname cmtkitd
%define version 1.0.1
%define unmangled_version 1.0.1
%define	_prefix /usr
%define release 1.el5

Summary:   command set of cmtkitd
Name:      cmtkd
Version:   %{version}
Release:   %{release}
License:   GPL
Group:     System
Prefix:    /usr
Source:    cmtkd.tar.gz
Url:	http://jigang.bv2.mianfeidns.com
Vendor:	duanjigang1983 <duanjigang1983@gmail.com>

%description
cmtkd is a system tool used to run commands and upload files to servers, multi-thread operations is supported by it.
for more functions and knowledge, please contact with the autor cme <duanjigang1983@gmail.com>.

%prep
%setup -c

%build
make

%install
cp cmtkd /usr/bin/
mkdir -pv /etc/cmtk
cp cmtkd_config.ini /etc/cmtk/
cp cmtkitd /etc/init.d/
chkconfig --add cmtkitd 
chmod a+x /etc/init.d/cmtkitd 
chmod a+r /etc/cmtk/  -R


%preun
service cmtkitd stop
chkconfig --del cmtkitd
unlink /etc/init.d/cmtkitd


%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/cmtkd
/etc/init.d/cmtkitd
/etc/cmtk/cmtkd_config.ini
%ifarch x86_64
/usr/local/lib64/libIce.so.3.3.1
/usr/local/lib64/libIceUtil.so.3.3.1
%else
/usr/local/lib/libIce.so.3.3.1
/usr/local/lib/libIceUtil.so.3.3.1
%endif
