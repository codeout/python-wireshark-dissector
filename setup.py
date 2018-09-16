from Cython.Distutils import build_ext
from distutils.core import setup
from distutils.extension import Extension
import os
import subprocess

WIRESHARK_VERSION = '2.4.5'

base_dir = os.path.dirname(os.path.abspath(__file__))
source_files = ['pywireshark.pyx', 'wireshark.c']

setup(
    cmdclass={'build_ext': build_ext},
    ext_modules=[
        Extension(
            'wireshark',
            [os.path.join(base_dir, i) for i in source_files],
            extra_compile_args=[
                # '-g', '-O3',
                *subprocess.check_output(('pkg-config', '--cflags', 'glib-2.0')).decode().split(),
                '-I%s' % base_dir,
                '-I%s/wireshark-%s' % (base_dir, WIRESHARK_VERSION),
                ],
            extra_link_args=['-lwireshark', '-lwiretap', '-lwsutil', '-lglib-2.0'],
        )
    ],
)
