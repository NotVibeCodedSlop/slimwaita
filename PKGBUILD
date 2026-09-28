_pkgname="slimwaita"
pkgbase=slimwaita
pkgname=slimwaita
pkgver=1.10.0.r12.g0ffcd2c
pkgrel=1
pkgdesc="SlimWaita is a fork of libadwaita"
url="https://github.com/NotVibeCodedSlop/slimwaita"
arch=(x86_64 i686 pentium4 aarch64 armv7h)
license=(LGPL-2.1-or-later)
depends=(
  appstream
  fribidi
  glib2
  glibc
  graphene
  pango
  gtk4
)
makedepends=(
  git
  gtk4
  glib2-devel
  gobject-introspection
  meson
  sassc
  vala
)
provides=(libadwaita)
conflicts=(libadwaita libadwaita-git)
source=(git+https://github.com/NotVibeCodedSlop/slimwaita.git)
sha256sums=(SKIP)
pkgver() {
  cd slimwaita
  local n
  n=$(git rev-list --count HEAD)
  printf "1.0.%d" "$n"
}

build() {


  arch-meson slimwaita build
  meson compile -C build
}
package() {
  provides+=(libadwaita-1.so)

  meson install -C build --destdir "$pkgdir"

  cd "$pkgdir"

}
