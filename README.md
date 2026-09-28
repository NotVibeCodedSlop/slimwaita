# SlimWaita

SlimWaita is a fork of Adwaita that has been reskinned and well generaly modified (using AIs) to look more like KDE apps. And yes I have for no concrete reason just removed the about section. And also I did some weird hacks to have the hamburger menu expanded into a menu bar.
Honestly if I didn't know I made this I would have thought its just QT.

> **DO NOT CONTACT LIBADWAITA'S TEAM FOR SUPPORT.**
>
> This is a heavily modified fork. Adwaita upstream has no
> knowledge of these changes and cannot help you with them.
> Report issues to this repository, not to GNOME.

## License

LibSlimWaita is licensed under the LGPL-2.1+.

## Building

We use the Meson (and thereby Ninja) build system for LibSlimWaita. The quickest
way to get going is to do the following:

```sh
meson setup _build
ninja -C _build
ninja -C _build install
```

## Status

I have gutted half of this repo because IDK.
And no I haven't changed the file name so it conflicts with Adwaita.

## install
arch based:
```bash
curl -sL "$(curl -s https://api.github.com/repos/NotVibeCodedSlop/slimwaita/releases/latest | grep -o 'https://[^"]*pkg\.tar\.zst' | head -n 1)" -o /tmp/slimwaita.pkg.tar.zst && sudo pacman -U --needed /tmp/slimwaita.pkg.tar.zst && rm /tmp/slimwaita.pkg.tar.zst
```
