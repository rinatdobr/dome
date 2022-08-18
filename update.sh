#!/bin/bash

systemctl --user daemon-reload
systemctl daemon-reload

systemctl --user restart dome_statistic
systemctl --user restart dome_client
sudo systemctl restart dome

systemctl --user status dome_statistic
systemctl --user status dome_client
systemctl status dome
