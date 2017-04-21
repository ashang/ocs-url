Summary: OCS-URL
Name: ocs-url
Version: 3.0.0
Release: 1%{?dist}
License: GPLv3+
Group: Applications/Internet
URL: https://github.com/opendesktop/ocs-url

#Source0: https://github.com/opendesktop/ocs-url/archive/release-%{version}.tar.gz
Source0: %{name}.tar.gz

Requires: libQt5Svg5 >= 5.2.0, libqt5-qtquickcontrols >= 5.2.0
BuildRequires: make, automake, gcc, gcc-c++, libtool, libqt5-qtbase-devel >= 5.2.0, libqt5-qtsvg-devel >= 5.2.0, libqt5-qtdeclarative-devel >= 5.2.0, git, rpm-build

%description
An install helper program for items served on OpenCollaborationServices (OCS).

%prep
#%%autosetup -n %{name}-release-%{version}
%autosetup -n %{name}
sh scripts/import.sh

%build
%define debug_package %{nil}
qmake-qt5 PREFIX="/usr"
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
* Sun Mar 05 2017 Akira Ohgaki <akiraohgaki@gmail.com> - 3.0.0-1
- Rename xdgurl to ocs-url
- Add ocs:// and ocss:// schemes
- xdg:// and xdgs:// schemes still available for backward compatibile
- New app icon
- New dialog window
- Labels and messages are translatable
- Qt 5.2 support
- Use the same configs of install-types with ocsstore
- Add en_US translation
- Add ja_JP translation
- Add tr_TR translation (by tarakbumba)

* Wed Jan 25 2017 Akira Ohgaki <akiraohgaki@gmail.com> - 2.0.3-1
- Change installation destination of type bin
- Update qtlib
- Fix for dialog

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
