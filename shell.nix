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

    fmt
    fmt.dev
    spdlog
    spdlog.dev
    cli11
    protobuf
    abseil-cpp_202505
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
  ];

  profile = ''
    export VCPKG_FORCE_SYSTEM_BINARIES=1
    export SDL_LOG_PRIORITY=verbose
    export SDL_VIDEODRIVER=wayland
  '';
  runScript = "zsh";
}).env
