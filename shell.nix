{ pkgs ? import <nixpkgs> {} }:

(pkgs.buildFHSEnv {
  name = "ruin";
  targetPkgs = pkgs: with pkgs; [
    autoconf
    autoconf-archive
    automake
    bash
    binutils
    ccache
    clang
    clang-tools
    cmake
    coreutils
    curl
    gcc
    git
    gnumake
    gnutar
    gzip
    libtool
    linuxHeaders
    m4
    ninja
    nodejs
    perl
    pkg-config
    python3
    unzip
    zip

    fmt
    fmt.dev
    spdlog
    spdlog.dev
    cli11
    protobuf
    abseil-cpp.dev
    zlib
    zlib.dev
    sdl3
    sdl3.dev
    sdl3-image
    sdl3-image.dev
    sdl3-ttf
    entt
    freetype
    freetype.dev

    wayland
    wayland.dev
    wayland-protocols
    wayland-scanner

    alsa-lib
    alsa-lib.dev
    libpulseaudio
    libpulseaudio.dev
    udev
    udev.dev
    dbus
    dbus.dev

    zsh
    ncurses
  ];

  profile = ''
    export VCPKG_FORCE_SYSTEM_BINARIES=1
    export SDL_LOG_PRIORITY=verbose
    export SDL_VIDEODRIVER=wayland
  '';
  runScript = "zsh";
}).env
