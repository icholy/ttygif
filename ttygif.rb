require 'formula'

class Ttygif < Formula
  homepage 'https://github.com/icholy/ttygif'
  url 'https://github.com/icholy/ttygif/archive/1.2.1.zip'
  sha1 'ecddb9d05a8eda43f955c7f7877c03517cef939e'

  depends_on 'imagemagick'
  depends_on 'ttyrec'

  def install
    system 'make'
    bin.install('ttygif')
  end
end
