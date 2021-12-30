# DesignLab

Command line:

```bash
../app/build_release_link/DesignLab_release_link.elf.app/Contents/MacOS/DesignLab_release_link.elf
```

With theme update:

```bash
../app/build_release_link/DesignLab_release_link.elf.app/Contents/MacOS/DesignLab_release_link.elf --theme=../designlab/themes/lvgl-small-light.json && ninja && ../app/build_release_link/DesignLab_release_link.elf.app/Contents/MacOS/DesignLab_release_link.elf
```


```bash
lv_font_conv --bpp=8 --size=64 --format=lvgl --output=../app/src/designlab/fontawesome_brands_regular.c --font=../designlab/fonts/fontawesome/"Font Awesome 5 Brands-Regular-400.otf" --range=0x0000-0xffff --no-compress
lv_font_conv --bpp=8 --size=64 --format=lvgl --output=../app/src/designlab/fontawesome_solid.c --font=../designlab/fonts/fontawesome/"Font Awesome 5 Free-Solid-900.otf" --range=0x0000-0xffff --no-compress  
lv_font_conv --bpp=8 --size=64 --format=lvgl --output=../app/src/designlab/fontawesome_regular.c --font=../designlab/fonts/fontawesome/"Font Awesome 5 Free-Regular-400.otf" --range=0x0000-0xffff --no-compress 
```