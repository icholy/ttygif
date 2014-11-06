require 'formula'

class Ttygif < Formula
    homepage 'https://github.com/icholy/ttygif'
    url 'https://github.com/icholy/ttygif/archive/1.0.8.zip'
    sha1 'f8d0a56af11d3ae8e2d5e64a4f6aceccb8338414'

    depends_on 'imagemagick'
    depends_on 'ttyrec'

    def install
        system "make"
        bin.install('ttygif')
    end
end
