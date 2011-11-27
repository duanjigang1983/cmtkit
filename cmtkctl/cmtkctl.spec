%define orgniname cmtkctl
%define version 1.0.2
%define unmangled_version 1.0.2
%define	_prefix /usr
%define release 1.el5

Summary:   command set of cmtkit
Name:      cmtkctl
Version:   %{version}
Release:   %{release}
License:   GPL
Group:     System
Prefix:    /usr
Source:    cmtkctl.tar.gz
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
cp cmtkctl /usr/bin/
%preun

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin/cmtkctl
