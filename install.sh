#!/bin/bash

systemctl --user enable domed
systemctl --user enable dome_core
systemctl --user enable dome_ep_dht22
systemctl --user enable dome_ep_openweather
systemctl --user enable dome_ep_telegram
systemctl --user enable dome_ep_ipcamera
systemctl --user enable dome_ep_s8
sudo systemctl enable pigpiod
systemctl --user daemon-reload



# systemctl --user status dome_ep_dht22

# systemctl --user stop dome_core && sleep 1 && \
systemctl --user stop dome_ep_dht22 && \
systemctl --user stop dome_ep_telegram && \
systemctl --user stop dome_ep_ipcamera && \
systemctl --user stop dome_ep_s8 && \
systemctl --user stop dome_ep_openweather
