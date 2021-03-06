# -*- coding: utf-8 -*-
#!/usr/bin/env python
#
# Project: Video Streaming with Flask
# Author: Log0 <im [dot] ckieric [at] gmail [dot] com>
# Date: 2014/12/21
# Website: http://www.chioka.in/
# Description:
# Modified to support streaming out with webcams, and not just raw JPEGs.
# Most of the code credits to Miguel Grinberg, except that I made a small tweak. Thanks!
# Credits: http://blog.miguelgrinberg.com/post/video-streaming-with-flask
#
# Usage:
# 1. Install Python dependencies: cv2, flask. (wish that pip install works like a charm)
# 2. Run "python main.py".
# 3. Navigate the browser to the local webpage.
#
#
# - ATR Lab at Kent State University -
# - Kim added some text on videostream


from flask import Flask, render_template, Response
from camera import VideoCamera
import socket
import sys
import time

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

def gen(camera):
    host = "127.0.0.1"
    port = "1234"
    client_socket = socket.socket()

    try:
        client_socket.connect((host, int(port)))
    except socket.error as msg:
        sys.stderr.write('WARNING: {}\n'.format(msg))
        time.sleep(5)  # intentional delay on reconnection as client
        print("Socket Error... exiting")
        sys.exit(0)
    odometer = "0"
    speed = "0"
    while True:
        info = client_socket.recv(1024)
        oOpen, oClose = info.find("<O"), info.find("O>")
        sOpen, sClose = info.find("<S"), info.find("S>")
        if (oOpen != -1) and (oClose != -1):
            odometer = str(info[oOpen+2:oClose])
            #print(odometer)
        if sOpen != -1 and sClose != -1:
            speed = str(info[sOpen+2:sClose])
            #print(speed)
        message = "odometer: " + odometer + ", speed: " + speed
        frame = camera.get_frame(message)
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

@app.route('/video_feed')
def video_feed():
    return Response(gen(VideoCamera()),
                    mimetype='multipart/x-mixed-replace; boundary=frame')


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
