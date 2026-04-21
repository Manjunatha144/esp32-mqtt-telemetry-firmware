ESP32 Bring-up Notes



COM port: COM6 (Silicon Labs CP210x)

Command: idf.py -p COM6 flash monitor



Observed:

\- Hello world!

\- chip info printed

\- reboot countdown printed

\- bootloader logs shown



Note:

\- Warning about flash size (4096k detected vs 2048k header) appeared but program runs OK.

