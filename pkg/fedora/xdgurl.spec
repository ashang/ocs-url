Summary: An install helper program for desktop stuff
Name: xdgurl
Version: 1.0.1
Release: 1%{?dist}
License: GPLv3+
Group: Applications/Internet
URL: https://github.com/xdgurl/xdgurl

#Source0: https://github.com/xdgurl/xdgurl/archive/release-%{version}.tar.gz
Source0: %{name}.tar.gz

Requires: tkinter, python3-tkinter

%description
An install helper program for desktop stuff.

%prep
#%%autosetup -n %{name}-release-%{version}
%autosetup -n %{name}

%build
%define debug_package %{nil}
make

%install
make DESTDIR="%{buildroot}" prefix="/usr" install

%files
%defattr(-,root,root)
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg

%clean
rm -rf %{buildroot}

%changelog
* Fri Jul 15 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 1.0.1-1
- Clean successfull message
- Return exit code

* Wed Jul 06 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 1.0.0-1
- Initial release
