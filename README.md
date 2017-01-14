![](ttygif.png)

> ttygif converts a ttyrec file into gif files.
> It's a stripped down version of ttyplay that screenshots every frame.

## Demo

![gif](http://i.imgur.com/nvEHTgn.gif)

## Setup

### Debian/Ubuntu
``` sh
$ sudo apt-get install imagemagick ttyrec gcc x11-apps
$ git clone https://github.com/icholy/ttygif.git
$ cd ttygif
$ make
$ sudo make install
```

### Fedora/Redhat
``` sh
$ sudo dnf install ImageMagick gcc
$ # install ttyrec from source patched ~> https://github.com/mattwilmott/ttyrec
$ git clone https://github.com/icholy/ttygif.git
$ cd ttygif
$ make
$ sudo make install
```

### Arch Linux
``` sh
$ git clone https://aur.archlinux.org/ttyrec.git
$ cd ttyrec
$ makepkg -i
$ cd ..
$ git clone https://aur.archlinux.org/ttygif.git
$ cd ttygif
$ makepkg -is
```

### macOS
``` sh
$ brew install ttygif
```

## Usage:

**1. Create ttyrec recording**

``` sh
$ ttyrec myrecording
```

* Hit CTRL-D or type `exit` when done recording.

**2. Convert to gif**

``` sh
$ ttygif myrecording
```

On OSX optionally you can set a -f flag which will bypass cropping which is needed for terminal apps which aren't full screen. 
Both standard Terminal and iTerm apps are supported.

``` sh
$ ttygif myrecording -f
```

## Additional Notes

If you're getting `Error: WINDOWID environment variable was empty.`, then you need to manually set `WINDOWID`.

``` sh
export WINDOWID=23068679
```

If you're on Ubuntu, you can use `xdotool` to find the WINDOWID
``` sh
$ sudo apt-get install xdotool
$ export WINDOWID=$(xdotool getwindowfocus)
```

## Debugging

If you're having issues, then export the `TTYGIF_DEBUG` env variable.

``` sh
export TTYGIF_DEBUG=1
```

This will print out all the commands it's trying to run.

## Credits

The idea and approach was adapted from [tty2gif](https://bitbucket.org/antocuni/tty2gif)

![](http://i.imgur.com/9et8daN.jpg)

