# TTYGIF

> ttygif converts a ttyrec file into gif files.
> It's a stripped down version of ttyplay that captures every frame.

## Setup

### Debian/Ubuntu
``` sh
$ sudo apt-get install imagemagick ttyrec gcc
$ git clone https://github.com/icholy/ttygif.git
$ cd ttygif
$ make
```

### Fedora/Redhat
``` sh
$ sudo yum install ImageMagick gcc
$ # install ttyrec from source ~> https://github.com/mjording/ttyrec
$ git clone https://github.com/icholy/ttygif.git
$ cd ttygif
$ make
```

### OSX
``` sh
$ brew install imagemagick ttyrec
$ brew install https://raw.githubusercontent.com/icholy/ttygif/master/ttygif.rb
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

## Demo:

![gif](http://i.imgur.com/nvEHTgn.gif)

## Credits

The idea and approach was adapted from [tty2gif](https://bitbucket.org/antocuni/tty2gif)

![](http://i.imgur.com/9et8daN.jpg)

