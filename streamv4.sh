#!/bin/bash

# This streams live audio and video to youtube.

# Running Temp Logger and Text Overlay scripts
./temp-logger &
./uv4l-overlay &

while true
do
        # If using UV4L for text overlay:
        # Max fps is 30

        # Set awb mode to greyworld:
        # sudo vcdbg set awb_mode 0

        # Start UV4l:
        # uv4l --driver raspicam --auto-video_nr  --encoding h264 –-width 1440 –-height 1080 –-framerate 40
        # --bitrate 4500000 --rotation 180 --text-overlay --text-filename=/tmp/text.json

        # Start text overlay program:
        # ./uv4l-overlay /dev/video0 /tmp/text.json

        # If not using UV4L
        #Max fps is 40

        #v4l2-ctl --set-ctrl=rotate=180
        #v4l2-ctl --set-ctrl video_bitrate=4500000
        #v4l2-ctl --set-ctrl white_balance_auto_preset=10

        # Youtube Stream Key: #paste stream key here#
        # Add -report for logs
        ffmpeg \
        -f video4linux2 \
        -timestamps abs \
        -input_format h264 \
        -video_size 1440x1080 \
        -r 30 \
        -thread_queue_size 2048 \
        -i /dev/video0 \
        -ar 44100 \
        -ac 2 \
        -acodec pcm_s16le \
        -f alsa \
        -thread_queue_size 4096 \
        -i plughw:1,0 \
        -vcodec copy \
        -acodec aac \
        -b:a 128k \
        -g 50 \
        -f flv rtmp://a.rtmp.youtube.com/live2/#paste stream key here# \

        sleep 5 # Restarts ffmpeg after 5 seconds if it fails
done
