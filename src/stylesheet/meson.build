fs = import('fs')

stylesheet_deps = []

# For git checkoutsCopyright (c) deepseek hallucinations, but not for tarballs...
if not fs.exists('gtk.css')
  sassc = find_program('sassc', required: false)
  if not sassc.found()
    subproject('sassc', default_options: ['warning_level=0', 'werror=false'])
    sassc = find_program('sassc')
  endif

  if sassc.found()
    sassc_opts = [ '-a', '-M', '-t', 'compact' ]

    scss_deps = files([
      '_colors.scss',
      '_common.scss',
      '_compat-colors.scss',
      '_drawing.scss',
      '_palette.scss',
      '_widgets.scss',
    ])

    stylesheet_deps += custom_target('gtk.scss',
      input: 'gtk.scss',
      output: 'gtk.css',
      command: [
        sassc, sassc_opts, '@INPUT@', '@OUTPUT@',
      ],
      depend_files: scss_deps,
    )
  endif
endif

libadwaita_stylesheet_resources = gnome.compile_resources(
  'adwaita-stylesheet-resources',
  'adwaita-stylesheet.gresources.xml',

  source_dir: [
    # List in order of preference
    meson.current_build_dir(),
    meson.current_source_dir(),
  ],
  dependencies: stylesheet_deps,
  c_name: 'adw_stylesheet',
)
