# ⌬ [ARCHON-PLAYER] - PKGBUILD PARA O ECOSSISTEMA ARCHIA
# Maintainer: luiskallak-design

pkgname=archonplayer-git
pkgver=1.0
pkgrel=1
pkgdesc="Player de áudio TUI forjado para o ecossistema Archon"
arch=('x86_64')
url="https://github.com"
license=('MIT')

# Dependências do Arsenal
depends=('sdl2' 'sdl2_mixer' 'ncurses')
makedepends=('git' 'gcc' 'make')
provides=('archonplayer')
conflicts=('archonplayer')

# Fonte Corrigida: Aponta para o repositório real
source=("git+${url}.git")
sha256sums=('SKIP')

build() {
  # O git clona para uma pasta com o nome do repositório
  cd "archonplayer-tui"
  
  echo -e "\033[0;36m⌬ INICIANDO FORJA DO BINÁRIO...\033[0m"
  make
}

package() {
  cd "archonplayer-tui"
  
  echo -e "\033[0;32m⌬ DISTRIBUINDO PODER AO SISTEMA...\033[0m"
  
  # Instalação padrão Arch Linux (sem sudo dentro do script)
  install -Dm755 bin/archonplayer "$pkgdir/usr/bin/archonplayer"
}
