%define orgniname cmtk
%define version 1.0.3
%define unmangled_version 1.0.3
%define	_prefix /usr
%define release 1.el5

Summary:   command set of cmtkit
Name:      cmtk
Version:   %{version}
Release:   %{release}
License:   GPL
Group:     System
Prefix:    /usr
Source:    cmtk.tar.gz
Url:	http://jigang.bv2.mianfeidns.com
Vendor:	duanjigang1983 <duanjigang1983@gmail.com>

%description
cmtk is a system tool used to run commands and upload files to servers, multi-thread operations is supported by it.
for more functions and knowledge, please contact with the autor cme <duanjigang1983@gmail.com>.

%prep
%setup -c

%build
make

%install
cp cmtk /usr/bin/
mkdir -pv /etc/cmtk
cp cmtk_config.ini /etc/cmtk/
chmod a+r /etc/cmtk/  -R
%preun

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/cmtk
/etc/cmtk/cmtk_config.ini
%ifarch x86_64
/usr/local/Ice-3.3.1/lib64/libIce.so.3.3.1
/usr/local/Ice-3.3.1/lib64/libIceUtil.so.3.3.1
%else
/usr/local/Ice-3.3.1/lib/libIce.so.3.3.1
/usr/local/Ice-3.3.1/lib/libIceUtil.so.3.3.1
%endif
