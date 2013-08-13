
ttygif converts a ttyrec file into gif files. It's a stripped down version of ttyplay which calls `import` on every frame.

**Installation:**

``` sh
$ make
```

**Prerequisites:** [ImageMagick](http://www.imagemagick.org/script/index.php)

``` sh
$ sudo apt-get install imagemagick
```

**Usage:**

Create gif frames

``` sh
$ ./ttygif [FILE]
```

Create animated gif

``` sh
$ convert -delay 25 -loop 0 *.gif terminal.gif
```

Set correct frame delays

``` sh
$ ./set_delays.sh terminal.gif
```

Optimize output

``` sh
$ gifsicle --batch --optimize=3 terminal.gif
```

**Example:**

![img](http://i.imgur.com/KTWIWQT.gif)
