require 'formula'

class Ttygif < Formula
  homepage 'https://github.com/icholy/ttygif'
  url 'https://github.com/icholy/ttygif/archive/1.3.0.zip'
  sha1 'f2412cf43d00f39555c6ee79c4ee6a469050de3b'

  depends_on 'imagemagick'
  depends_on 'ttyrec'

  def install
    system 'make'
    bin.install('ttygif')
  end
end
