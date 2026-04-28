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
    gdb
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

    fmt.dev
    spdlog.dev
    cli11
    protobuf
    abseil-cpp.dev
    zlib.dev
    sdl3.dev
    sdl3-image.dev
    sdl3-ttf
    entt
    freetype.dev

    wayland.dev
    wayland-protocols
    wayland-scanner

    alsa-lib.dev
    libpulseaudio.dev
    udev.dev
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
