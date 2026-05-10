# ⌬ [ARCHON-PLAYER] - PKGBUILD PARA O ECOSSISTEMA ARCHIA
# Maintainer: luiskallak-design

pkgname=archonplayer-git
pkgver=2.0
pkgrel=1
pkgdesc="Player de áudio TUI moderno com espectro em tempo real e estética cyberpunk"
arch=('x86_64')
url="https://github.com/luiskallak-design/archonplayer-tui"
license=('MIT')

# Arsenal de Dependências (Necessárias para o Arsenal SDL2 e Ncurses)
depends=('sdl2' 'sdl2_mixer' 'ncurses')
makedepends=('git' 'gcc' 'make')
provides=('archonplayer')
conflicts=('archonplayer')

# Fonte Corrigida (Agora apontando para o seu repositório oficial)
source=("git+${url}.git")
sha256sums=('SKIP')

build() {
  # O segredo é entrar na subpasta 'archonplayer' após entrar no repo
  cd "$srcdir/archonplayer-tui/archonplayer"
  
  echo -e "\033[0;36m⌬ INICIANDO FORJA DO BINÁRIO NO OLIMPO...\033[0m"
  make
}

package() {
  # Entra na mesma subpasta para achar o binário que o Makefile gerou
  cd "$srcdir/archonplayer-tui/archonplayer"
  
  echo -e "\033[0;32m⌬ DISTRIBUINDO PODER AO SISTEMA...\033[0m"
  install -Dm755 bin/archonplayer "$pkgdir/usr/bin/archonplayer"
}
