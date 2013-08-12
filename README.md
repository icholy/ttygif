ttygif converts ttyrec files to gifs.

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

