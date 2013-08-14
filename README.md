
ttygif converts a ttyrec file into gif files. It's a stripped down version of ttyplay which calls `import` on every frame.

**Installation:**

``` sh
$ make
```

**Prerequisites:**

``` sh
$ sudo apt-get install imagemagick ttyrec
```

**Usage:**

Create gif frames

``` sh
$ ./ttygif [FILE]
```

Create animated gif

``` sh
$ ./concat.sh terminal.gif 
```

**Example:**

![gif](http://i.imgur.com/WyFoHXZ.gif)
