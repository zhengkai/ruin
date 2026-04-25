{ pkgs ? import <nixpkgs> {} }:

(pkgs.buildFHSEnv {
  name = "ruin";
  targetPkgs = pkgs: with pkgs; [
    autoconf-archive
    libtool
    autoconf
    automake
    bash
    binutils
    cmake
    coreutils
    curl
    gcc
    git
    gnumake
    gnutar
    gzip
    linuxHeaders
    m4
    ninja
    perl
    pkg-config
    python3
    unzip
    zip

    glibc

    sdl3
    wayland
    wayland-protocols
    wayland-scanner
    libxkbcommon
    libdecor
    libffi
    mesa
    vulkan-loader
    libglvnd
    libdrm
    libX11
    libXcursor
    libXrandr
    libXi
    libXext
    libXfixes
    libXinerama
    libXScrnSaver
    libGL

    alsa-lib
    libpulseaudio
    udev
    dbus
  ];

  profile = ''
    export LD_LIBRARY_PATH=${pkgs.lib.makeLibraryPath [
      pkgs.glibc
      pkgs.sdl3
      pkgs.wayland
      pkgs.libxkbcommon
      pkgs.libdecor
      pkgs.mesa
      pkgs.vulkan-loader
      pkgs.udev
      pkgs.libglvnd
      pkgs.dbus
      pkgs.alsa-lib
      pkgs.libpulseaudio
    ]}
    export VCPKG_FORCE_SYSTEM_BINARIES=1
    export SDL_LOG_PRIORITY=verbose
    export SDL_VIDEODRIVER=wayland
  '';
  runScript = "zsh";
}).env
