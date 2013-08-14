# TTYGIF

> ttygif converts a ttyrec file into gif files.
> It's a stripped down version of ttyplay which calls `import` on every frame.

## Installation:

``` sh
$ make
```

## Prerequisites:

``` sh
$ sudo apt-get install imagemagick ttyrec
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

* dumps a bunch of gif images into the current directory.
* File names have this pattern: `<zero_padded_index>_<delay_in_milliseconds>.gif`

**3. Create animated gif**

``` sh
$ ./concat.sh terminal.gif 
```

* concatenates all the images in the current directory

**Example:**

![gif](http://i.imgur.com/WyFoHXZ.gif)
