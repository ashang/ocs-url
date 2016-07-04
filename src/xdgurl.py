#!/usr/bin/env python

import sys
import os
import json
import urllib
import urlparse
import tempfile
import mimetypes
import subprocess

import Tkinter
import tkMessageBox

class XdgUrl:
    """Core class of xdgurl"""

    def __init__(self, xdg_url=''):
        self.xdg_url = xdg_url
        self.meta = self.parse()

        self.temp_dir = tempfile.gettempdir()
        self.home_dir = os.path.expanduser('~')
        #self.config_dir = os.path.join(self.home_dir, '.config', 'xdgurl')
        self.data_dir = os.path.join(self.home_dir, '.local', 'share')
        self.kde_data_dir = os.path.join(self.home_dir, '.kde', 'share')

        #self.config = {}

        self.destinations = {
            'downloads': os.path.join(self.home_dir, 'Downloads'),
            'documents': os.path.join(self.home_dir, 'Documents'),
            'pictures': os.path.join(self.home_dir, 'Pictures'),
            'music': os.path.join(self.home_dir, 'Music'),
            'videos': os.path.join(self.home_dir, 'Videos'),
            'wallpapers': os.path.join(self.data_dir, 'wallpapers'),
            'fonts': os.path.join(self.home_dir, '.fonts'),
            'cursors': os.path.join(self.home_dir, '.icons'),
            'icons': os.path.join(self.data_dir, 'icons'),
            'emoticons': os.path.join(self.data_dir, 'emoticons'),
            'themes': os.path.join(self.home_dir, '.themes'),
            'emerald_themes': os.path.join(self.home_dir, '.emerald', 'themes'),
            'enlightenment_themes': os.path.join(self.home_dir, '.e', 'e', 'themes'),
            'enlightenment_backgrounds': os.path.join(self.home_dir, '.e', 'e', 'backgrounds'),
            'fluxbox_styles': os.path.join(self.home_dir, '.fluxbox', 'styles'),
            'pekwm_themes': os.path.join(self.home_dir, '.pekwm', 'themes'),
            'icewm_themes': os.path.join(self.home_dir, '.icewm', 'themes'),
            'plasma_plasmoids': os.path.join(self.data_dir, 'plasma', 'plasmoids'),
            'plasma_look_and_feel': os.path.join(self.data_dir, 'plasma', 'look-and-feel'),
            'plasma_desktopthemes': os.path.join(self.data_dir, 'plasma', 'desktoptheme'),
            'kwin_effects': os.path.join(self.data_dir, 'kwin', 'effects'),
            'kwin_scripts': os.path.join(self.data_dir, 'kwin', 'scripts'),
            'kwin_tabbox': os.path.join(self.data_dir, 'kwin', 'tabbox'),
            'aurorae_themes': os.path.join(self.data_dir, 'aurorae', 'themes'),
            'dekorator_themes': os.path.join(self.data_dir, 'deKorator', 'themes'),
            'qtcurve': os.path.join(self.data_dir, 'QtCurve'),
            'color_schemes': os.path.join(self.data_dir, 'color-schemes'),
            'gnome_shell_extensions': os.path.join(self.data_dir, 'gnome-shell', 'extensions'),
            'cinnamon_applets': os.path.join(self.data_dir, 'cinnamon', 'applets'),
            'cinnamon_desklets': os.path.join(self.data_dir, 'cinnamon', 'desklets'),
            'cinnamon_extensions': os.path.join(self.data_dir, 'cinnamon', 'extensions'),
            'nautilus_scripts': os.path.join(self.data_dir, 'nautilus', 'scripts'),
            'amarok_scripts': os.path.join(self.kde_data_dir, 'apps', 'amarok', 'scripts'),
            'yakuake_skins': os.path.join(self.kde_data_dir, 'apps', 'yakuake', 'skins'),
            'cairo_clock_themes': os.path.join(self.home_dir, '.cairo-clock', 'themes')
        }
        self.destinations_alias = {
            'gnome_shell_themes': self.destinations['themes'],
            'cinnamon_themes': self.destinations['themes'],
            'gtk2_themes': self.destinations['themes'],
            'gtk3_themes': self.destinations['themes'],
            'metacity_themes': self.destinations['themes'],
            'xfwm4_themes': self.destinations['themes'],
            'openbox_themes': self.destinations['themes'],
            'kvantum_themes': self.destinations['themes'],
            'compiz_themes': self.destinations['emerald_themes'],
            'beryl_themes': self.destinations['emerald_themes'],
            'plasma4_plasmoids': self.destinations['plasma_plasmoids'],
            'plasma5_plasmoids': self.destinations['plasma_plasmoids'],
            'plasma5_look_and_feel': self.destinations['plasma_look_and_feel'],
            'plasma5_desktopthemes': self.destinations['plasma_desktopthemes'],
            'plasma_color_schemes': self.destinations['color_schemes']
        }
        self.destinations.update(self.destinations_alias)

        self.archive_types = {
            'tar': [
                'application/x-tar',
                'application/x-gzip',
                'application/gzip',
                'application/x-bzip',
                'application/x-bzip2',
                'application/x-xz',
                'application/x-lzma',
                'application/x-lzip',
                'application/x-compressed-tar',
                'application/x-bzip-compressed-tar',
                'application/x-bzip2-compressed-tar',
                'application/x-xz-compressed-tar',
                'application/x-lzma-compressed-tar',
                'application/x-lzip-compressed-tar'
            ],
            'zip': ['application/zip'],
            '7z': ['application/x-7z-compressed'],
            'rar': [
                'application/x-rar',
                'application/x-rar-compressed'
            ]
        }

        #self.config.update(self.read_config('config'))
        #self.destinations.update(self.read_config('destinations'))

    def read_config(self, name):
        path = os.path.join(self.config_dir, name + '.json')
        data = {}
        if os.path.isfile(path):
            f = open(path, 'r')
            data = json.load(f)
            f.close()
        return data

    def write_config(self, name, data):
        if not os.path.isdir(self.config_dir):
            os.makedirs(self.config_dir)
        path = os.path.join(self.config_dir, name + '.json')
        f = open(path, 'w')
        json.dump(data, f)
        f.close()

    def parse(self):
        meta = {
            'command': 'download',
            'url': '',
            'type': 'downloads',
            'filename': ''
        }

        parse_result = urlparse.urlparse(self.xdg_url)
        query = urlparse.parse_qs(parse_result.query)

        if parse_result.netloc:
            meta['command'] = parse_result.netloc

        if 'url' in query and query['url'][0]:
            meta['url'] = urllib.unquote(query['url'][0])

        if 'type' in query and query['type'][0]:
            meta['type'] = urllib.unquote(query['type'][0])

        if 'filename' in query and query['filename'][0]:
            meta['filename'] = urllib.unquote(query['filename'][0]).split('?')[0]

        if meta['url'] and not meta['filename']:
            meta['filename'] = os.path.basename(meta['url']).split('?')[0]

        return meta

    """
    def detect_desktop_environment(self):
        desktop_environment = 'unknown'
        if os.environ.get('KDE_FULL_SESSION') == 'true':
            desktop_environment = 'kde'
        elif os.environ.get('GNOME_DESKTOP_SESSION_ID'):
            desktop_environment = 'gnome'
        return desktop_environment
    """

    def install_plasmapkg(self, path, type='plasmoid'):
        status = subprocess.call(['plasmapkg2', '-t', type, '-i', path])
        if status == 0:
            return True
        return False

    def uncompress_archive(self, path, target_dir):
        (mimetype, encoding) = mimetypes.guess_type(path)
        status = None

        if mimetype in self.archive_types['tar']:
            status = subprocess.call(['tar', '-xf', path, '-C', target_dir])
        elif mimetype in self.archive_types['zip']:
            status = subprocess.call(['unzip', '-o', path, '-d', target_dir])
        elif mimetype in self.archive_types['7z']:
            # No space between -o and directory
            status = subprocess.call(['7z', 'x', path, '-o' + target_dir])
        elif mimetype in self.archive_types['rar']:
            status = subprocess.call(['unrar', 'e', path, target_dir])

        if status == 0:
            return True
        return False

    def download(self):
        url = self.meta['url']
        type = self.meta['type']
        filename = self.meta['filename']
        destination = self.destinations[type]

        temp_path = os.path.join(self.temp_dir, filename)
        path = os.path.join(destination, filename)

        print('Retrieving a file from ' + url)
        urllib.urlretrieve(url, temp_path)

        print('Creating a directory ' + destination)
        if not os.path.isdir(destination):
            os.makedirs(destination)

        print('Saving a file to ' + path)
        os.rename(temp_path, path)

        print('Done')

    def install(self):
        url = self.meta['url']
        type = self.meta['type']
        filename = self.meta['filename']
        destination = self.destinations[type]

        temp_path = os.path.join(self.temp_dir, filename)
        path = os.path.join(destination, filename)

        print('Retrieving a file from ' + url)
        urllib.urlretrieve(url, temp_path)

        print('Creating a directory ' + destination)
        if not os.path.isdir(destination):
            os.makedirs(destination)

        print('Installing')
        if (type in ['plasma_plasmoids', 'plasma4_plasmoids', 'plasma5_plasmoids']
        and self.install_plasmapkg(temp_path, 'plasmoid')):
            print('The plasmoid has been installed')
            os.remove(temp_path)
        elif (type in ['plasma_look_and_feel', 'plasma5_look_and_feel']
        and self.install_plasmapkg(temp_path, 'lookandfeel')):
            print('The plasma look and feel has been installed')
            os.remove(temp_path)
        elif (type in ['plasma_desktopthemes', 'plasma5_desktopthemes']
        and self.install_plasmapkg(temp_path, 'theme')):
            print('The plasma desktop theme has been installed')
            os.remove(temp_path)
        elif (type == 'kwin_effects'
        and self.install_plasmapkg(temp_path, 'kwineffect')):
            print('The KWin effect has been installed')
            os.remove(temp_path)
        elif (type == 'kwin_scripts'
        and self.install_plasmapkg(temp_path, 'kwinscript')):
            print('The KWin script has been installed')
            os.remove(temp_path)
        elif (type == 'kwin_tabbox'
        and self.install_plasmapkg(temp_path, 'windowswitcher')):
            print('The KWin window switcher has been installed')
            os.remove(temp_path)
        elif self.uncompress_archive(temp_path, destination):
            print('The archive file has been uncompressed into ' + destination)
            os.remove(temp_path)
        else:
            print('Saving a file to ' + path)
            os.rename(temp_path, path)

        print('Done')

    def execute(self):
        if (self.meta['command'] in ['download', 'install']
        and self.meta['url']
        and self.meta['type'] in self.destinations
        and self.meta['filename']):
            if self.meta['command'] == 'download':
                self.download()
            elif self.meta['command'] == 'install':
                self.install()
        else:
            raise Exception('Incorrect XDG-URL ' + self.xdg_url)

'''
class XdgUrlApp(Tkinter.Frame):
    """Confirmation dialog for xdgurl"""

    def __init__(self, master=None, core=None):
        Tkinter.Frame.__init__(self, master)
        self.pack()
        self.core = core
        self.master.title('xdgurl')
        self.master.geometry('300x120')
        self.create_widget()

    def create_widget(self):
        execute_text = 'Download'
        if self.core.meta['command'] == 'install':
            execute_text = 'Install'

        info_label = Tkinter.Label(self, text=execute_text + ': ' + self.core.meta['filename'])
        info_label.pack(padx=10, pady=5, anchor=Tkinter.W)

        message_label = Tkinter.Label(self, text='Do you want to continue?')
        message_label.pack(padx=10, pady=5, anchor=Tkinter.W)

        execute_button = Tkinter.Button(self, text=execute_text, command=self.execute)
        execute_button.pack(padx=5, pady=10, side=Tkinter.RIGHT)

        quit_button = Tkinter.Button(self, text='Cancel', command=self.quit)
        quit_button.pack(padx=5, pady=10, side=Tkinter.RIGHT)

    def execute(self):
        self.core.execute()
        sys.exit()

    def quit(self):
        sys.exit()
'''

def main():
    if len(sys.argv) > 1:
        """
        window = Tkinter.Tk()
        core = XdgUrl(sys.argv[1])
        app = XdgUrlApp(window, core)
        app.mainloop()
        """

        core = XdgUrl(sys.argv[1])

        execute_text = 'Download'
        if core.meta['command'] == 'install':
            execute_text = 'Install'
        info_text = execute_text + ': ' + core.meta['filename'] + '\nFrom: ' + core.meta['url']

        window = Tkinter.Tk()
        window.withdraw()

        if tkMessageBox.askyesno('xdgurl', info_text + '\n\nDo you want to continue?'):
            try:
                core.execute();
            except Exception as e:
                message = ''
                if e.message:
                    message = e.message
                else:
                    message = str(e)
                tkMessageBox.showerror('xdgurl', info_text + '\n\n' + execute_text + ' failed\n' + message)
            else:
                tkMessageBox.showinfo('xdgurl', info_text + '\n\n' + execute_text + ' finished')
        sys.exit()
    else:
        print('xdgurl "XDG-URL"')
        sys.exit()

if __name__ == '__main__':
    main()
