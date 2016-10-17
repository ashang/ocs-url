Summary: An install helper program for desktop stuff
Name: xdgurl
Version: 2.0.0-beta
Release: 1%{?dist}
License: GPLv3+
Group: Applications/Internet
URL: https://github.com/xdgurl/xdgurl

#Source0: https://github.com/xdgurl/xdgurl/archive/release-%{version}.tar.gz
Source0: %{name}.tar.gz

Requires: qt5-qtbase >= 5.3.0, qt5-qtsvg >= 5.3.0, qt5-qtdeclarative >= 5.3.0, qt5-qtquickcontrols >= 5.3.0

%description
An install helper program for desktop stuff.

%prep
#%%autosetup -n %{name}-release-%{version}
%autosetup -n %{name}

%build
%define debug_package %{nil}
qmake PREFIX="/usr"
make

%install
make INSTALL_ROOT="%{buildroot}" install

%files
%defattr(-,root,root)
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg

%clean
rm -rf %{buildroot}

%changelog
* Mon Oct 17 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 2.0.0-beta-1
- Re-implemented xdgurl as Qt program
- Download progress bar
- Add install-type "bin" and "appimage"

* Fri Jul 15 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 1.0.1-1
- Clean successfull message
- Return exit code

* Wed Jul 06 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 1.0.0-1
- Initial release
