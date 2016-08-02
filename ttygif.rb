require 'formula'

class Ttygif < Formula
  homepage 'https://github.com/icholy/ttygif'
  url 'https://github.com/icholy/ttygif/archive/1.3.0.zip'
  sha256 'a833cb7798b26ad0652658be24322b07629282452f822f659871e4f4bcdd8173'

  depends_on 'imagemagick'
  depends_on 'ttyrec'

  def install
    system 'make'
    bin.install('ttygif')
  end
end
