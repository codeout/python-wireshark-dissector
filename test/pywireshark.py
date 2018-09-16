import os
import wireshark

base_dir = os.path.dirname(os.path.abspath(__file__))

with open(os.path.join(base_dir, 'raw_frame'), 'rb') as raw:
    wireshark.load()
    print(wireshark.dissect(raw.read()))
    wireshark.unload()
