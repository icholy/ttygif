# TTYGIF

> ttygif converts a ttyrec file into gif files.
> It's a stripped down version of ttyplay which calls `import` on every frame.

## Setup

``` sh
$ sudo apt-get install imagemagick ttyrec
$ git clone git@github.com:icholy/ttygif.git
$ cd ttygif
$ make
```

## Usage:

**1. Create ttyrec recording**

``` sh
$ ttyrec myrecording
```

* Hit CTRL-D or type `exit` when done recording.

**2. Create gif frames**

``` sh
$ ./ttygif myrecording
```

* Dumps a bunch of gif images into the current directory.
* File names have this pattern: `<zero_padded_index>_<delay_in_milliseconds>.gif`

**3. Create animated gif**

``` sh
$ ./concat.sh terminal.gif 
```

* Concatenates all the images in the current directory

## Demo:

![gif](http://i.imgur.com/kS18GFq.gif)

## Credits

The idea and approach was adapted from [tty2gif](https://bitbucket.org/antocuni/tty2gif)
