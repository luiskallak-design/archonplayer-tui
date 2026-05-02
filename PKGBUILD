# Maintainer: Seu Nome <seu-email@provedor.com>
pkgname=archonplayer-git
pkgver=1.0
pkgrel=1
pkgdesc="Tocador de música TUI moderno com espectro de áudio"
arch=('x86_64')
url="https://github.com"
license=('MIT')
depends=('sdl2' 'sdl2_mixer' 'ncurses')
makedepends=('git' 'make' 'gcc')
source=("git+${url}.git")
sha256sums=('SKIP')

build() {
  cd "$pkgname"
  make
}

package() {
  cd "$pkgname"
  # Usa o seu comando de install que já tá no Makefile!
  DESTDIR="$pkgdir" make install
}
