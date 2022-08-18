#!/bin/bash

systemctl --user daemon-reload
sudo systemctl daemon-reload

systemctl --user restart dome_statistic
systemctl --user status dome_statistic

systemctl --user restart dome_client
systemctl --user status dome_client

sudo systemctl restart domed
systemctl status domed
