Summary: An install helper program for desktop stuff
Name: xdgurl
Version: 1.0.0
Release: 1%{?dist}
License: GPLv3+
Group: Applications/Internet
URL: https://github.com/xdgurl/xdgurl

Source0: xdgurl.tar.gz

Requires: tkinter, python3-tkinter

%description
An install helper program for desktop stuff.

%build
install -D -m 755 xdgurl/src/xdgurl.py $RPM_BUILD_ROOT/%{_bindir}/xdgurl
install -D -m 644 xdgurl/src/xdgurl.desktop $RPM_BUILD_ROOT/%{_datadir}/applications/xdgurl.desktop

%files
%defattr(-,root,root)
%{_bindir}/xdgurl
%{_datadir}/applications/xdgurl.desktop

%changelog
* Wed Jul 06 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 1.0.0-1
- Initial release
