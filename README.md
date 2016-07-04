# xdgurl

An install helper program for desktop stuff.


## How to install

For Ubuntu/Debian system,
you can use "install.sh" in the project to install the program.

    $ sh install.sh

And you can use "uninstall.sh" to uninstall the program.

    $ sh uninstall.sh


## How to use

#### From web browser

Open XDG-URL by click a links, or type XDG-URL in browser's address bar.

In firefox,
you can get a program selection window when first time to open XDG-URL,
and you can choose "xdgurl" as custom URL handler.

If you can not get program selection window,
you need set the xdgurl program as custom URL handler for xdg:// scheme in browser's settings.

#### From command-line terminal

Execute xdgurl program with XDG-URL as argument.

    $ xdgurl "XDG-URL"


## XDG-URL

XDG-URL is a custom URL that represent the install method for desktop stuff.

    [scheme]://[command]?[query string]

    For example:
    xdg://install?url=http%3A%2F%2Fexample.com%2Ficons.tar.gz&type=icons

#### Scheme

* xdg
* xdgs

For now, "xdgs" scheme is the same of the xdg scheme,
it's reserved name for secure method in the future.

#### Command

* download
* install

Command "download" will download the file from specified URL to local destination of specified install-type.

Command "install" will make file download and trigger install process.

#### Query string

XDG-URL's query string is an option of command.

*All query string values should be urlencoded.*

**url** = File URL of desktop stuff

Must be set.

**type** = Install-type

Optional.
Default is "downloads".

Available install-type:

Install-type | Local destination
-------------|------------------
downloads | ~/Downloads
documents | ~/Documents
pictures | ~/Pictures
music | ~/Music
videos | ~/Videos
wallpapers | ~/.local/share/wallpapers
fonts | ~/.fonts
cursors | ~/.icons
icons | ~/.local/share/icons
emoticons | ~/.local/share/emoticons
themes | ~/.themes
emerald_themes | ~/.emerald/themes
enlightenment_themes | ~/.e/e/themes
enlightenment_backgrounds | ~/.e/e/backgrounds
fluxbox_styles | ~/.fluxbox/styles
pekwm_themes | ~/.pekwm/themes
icewm_themes | ~/.icewm/themes
plasma_plasmoids | ~/.local/share/plasma/plasmoids
plasma_look_and_feel | ~/.local/share/plasma/look-and-feel
plasma_desktopthemes | ~/.local/share/plasma/desktoptheme
kwin_effects | ~/.local/share/kwin/effects
kwin_scripts | ~/.local/share/kwin/scripts
kwin_tabbox | ~/.local/share/kwin/tabbox
aurorae_themes | ~/.local/share/aurorae/themes
dekorator_themes | ~/.local/share/deKorator/themes
qtcurve | ~/.local/share/QtCurve
color_schemes | ~/.local/share/color-schemes
gnome_shell_extensions | ~/.local/share/gnome-shell/extensions
cinnamon_applets | ~/.local/share/cinnamon/applets
cinnamon_desklets | ~/.local/share/cinnamon/desklets
nautilus_scripts | ~/.local/share/nautilus/scripts
amarok_scripts | ~/.kde/share/apps/amarok/scripts
yakuake_skins | ~/.kde/share/apps/yakuake/skins
cairo_clock_themes | ~/.cairo-clock/themes

Available alias name of the install-type:

Alias | Install-type
------|-------------
gnome_shell_themes | themes
cinnamon_themes | themes
gtk2_themes | themes
gtk3_themes | themes
metacity_themes | themes
xfwm4_themes | themes
openbox_themes | themes
kvantum_themes | themes
compiz_themes | emerald_themes
beryl_themes | emerald_themes
plasma4_plasmoids | plasma_plasmoids
plasma5_plasmoids | plasma_plasmoids
plasma5_look_and_feel | plasma_look_and_feel
plasma5_desktopthemes | plasma_desktopthemes
plasma_color_schemes | color_schemes

**filename** = Alternative file name

Optional.
This option is useful if the download URL does not represent the file name.
