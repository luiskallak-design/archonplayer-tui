# Mantido por: luiskallak-design
pkgname=archonplayer-git
pkgver=1.0
pkgrel=1
pkgdesc="Player de áudio TUI forjado para o ecossistema Archon"
arch=('x86_64')
url="https://github.com"
license=('GPL')
depends=('sdl2' 'sdl2_mixer' 'ncurses')
makedepends=('git' 'gcc' 'make')
provides=('archonplayer')
conflicts=('archonplayer')

# A URL correta para o source termina em .git
source=('git+https://github.com.git')
md5sums=('SKIP')

build() {
  cd "$srcdir/archonplayer-tui"
  # O build chama o seu Makefile (Missão Principal)
  make
}

package() {
  cd "$srcdir/archonplayer-tui"
  # Instala o binário na pasta padrão do sistema
  install -Dm755 bin/archonplayer "$pkgdir/usr/bin/archonplayer"
}
