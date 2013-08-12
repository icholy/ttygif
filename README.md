
ttygif converts a ttyrec file into gif files. It's a stripped down version of ttyplay which called `import` on every frame.

**Installation:**

``` sh
$ make
```

**Prerequisites:** [ImageMagick](http://www.imagemagick.org/script/index.php)

``` sh
$ sudo apt-get install imagemagick
```

**Usage:**

``` sh
$ ttygif [FILE]
$ convert -delay 25 -loop 0 *.gif terminal.gif
```

The output can get a little big. I use gifsicle to cut down on the size.

``` sh
$ gifsicle --batch --optimize=3 terminal.gif
```

**Example:**

![img](http://i.imgur.com/KTWIWQT.gif)
