#!/bin/bash

# This initialises UV4L

uv4l \
--driver raspicam \
--auto-video_nr \
--encoding h264 \
--bitrate 4000000 \
--rotation 180 \
--text-overlay \
--text-filename=/tmp/text.json \
--license-key #paste license key here# \
