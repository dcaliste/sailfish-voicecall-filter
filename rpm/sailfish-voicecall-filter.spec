Name: sailfish-voicecall-filter
Version: 0.1
Release: 1
Summary: a voicecall filter plugin for Sailfish OS
License: GPLv2
URL:     https://github.com/dcaliste/sailfish-voicecall-filter
Source0: %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(Qt5Test)
BuildRequires: pkgconfig(ofono)
BuildRequires: pkgconfig(mlite5)
BuildRequires: pkgconfig(gio-2.0)
BuildRequires: pkgconfig(dbus-1)
BuildRequires: cmake

%description
Provides a plugin and a daemon to filter received voice
calls based on phone numbers.

%package devel
Summary: Headers for the voicecall filter library
Requires: %{name} = %{version}-%{release}

%description devel
Development headers and library for the voice call filter.

%prep
%setup -q -n %{name}-%{version}

%build
%cmake
make %{?_smp_mflags}

%install
make DESTDIR=%{buildroot} install
UNIT_DIR=%{buildroot}%{_userunitdir}/user-session.target.wants
mkdir -p "$UNIT_DIR"
ln -sf ../voicecallfilterd.service "$UNIT_DIR/voicecallfilterd.service"

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%license LICENSE
%{_libdir}/*.so.*
%{_datadir}/glib-2.0/schemas/*.xml
%{_libdir}/ofono/plugins/*.so
%config %{_sysconfdir}/dbus-1/system.d/*.conf
%{_bindir}/voicecallfilterd
%{_userunitdir}/*.service
%{_userunitdir}/user-session.target.wants/*.service

%files devel
%defattr(-,root,root,-)
%{_libdir}/*.so
%{_includedir}/voicecallfilter
