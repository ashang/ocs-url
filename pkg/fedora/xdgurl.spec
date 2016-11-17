Summary: An install helper program for desktop stuff
Name: xdgurl
Version: 2.0.2
Release: 1%{?dist}
License: GPLv3+
Group: Applications/Internet
URL: https://github.com/xdgurl/xdgurl

#Source0: https://github.com/xdgurl/xdgurl/archive/release-%{version}.tar.gz
Source0: %{name}.tar.gz

Requires: qt5-qtbase >= 5.3.0, qt5-qtbase-gui >= 5.3.0, qt5-qtsvg >= 5.3.0, qt5-qtdeclarative >= 5.3.0, qt5-qtquickcontrols >= 5.3.0
BuildRequires: make, automake, gcc, gcc-c++, libtool, qt5-qtbase-devel, qt5-qtsvg-devel, qt5-qtdeclarative-devel, rpm-build

%description
An install helper program for desktop stuff.

%prep
#%%autosetup -n %{name}-release-%{version}
%autosetup -n %{name}

%build
%define debug_package %{nil}
qmake-qt5 PREFIX='/usr'
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
* Thu Nov 17 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 2.0.2-1
- Update qtlibs
- Small fix

* Mon Nov 14 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 2.0.1-1
- Update library
- Fix download/installation process

* Fri Oct 28 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 2.0.0-1
- Re-implemented xdgurl as Qt program
- Download progress bar
- Add install-type "bin"

* Fri Jul 15 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 1.0.1-1
- Clean successfull message
- Return exit code

* Wed Jul 06 2016 Akira Ohgaki <akiraohgaki@gmail.com> - 1.0.0-1
- Initial release
