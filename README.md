# TTYGIF

> ttygif converts a ttyrec file into gif files.
> It's a stripped down version of ttyplay which calls `import` on every frame.

## Setup

``` sh
$ sudo apt-get install imagemagick ttyrec
$ git clone https://github.com/icholy/ttygif.git
$ cd ttygif
$ git checkout ttyexec
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
$ ./ttyexec myrecording "import -window $WINDOWID {{index}}_{{delay}}.gif"
```

* The command will be executed for each frame

**3. Create animated gif**

``` sh
$ ./concat.sh terminal.gif 
```

* Concatenates all the images in the current directory

## Credits

The idea and approach was adapted from [tty2gif](https://bitbucket.org/antocuni/tty2gif)
