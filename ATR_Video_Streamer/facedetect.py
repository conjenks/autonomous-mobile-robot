# -*- coding: utf-8 -*-
import argparse
import cv2
import imutils
from imutils.video import VideoStream
from imutils import face_utils
import random
import time
import threading
import vlc

speak_lock = threading.Lock()
finish = True



def speak_finished(event, data):
    global finish
    finish = True

def robot_speak(face_num):
    single_dialogue = ["./res/bicyle-horn-sound-effect.mp3", "./res/single-late-for-work.mp3"]
    multi_dialogue = ["./res/bicyle-horn-sound-effect.mp3", "./res/multi-do-you-mind-moving.mp3"]
    # with speak_lock:
    # print("Starting thread : {}".format(threading.current_thread().name))
    with speak_lock:
        global finish
        current_data = threading.local()
        current_data.face_num = face_num
        #current_data.is_speaking = is_speaking
        #print("THREADS INSIDE: %d", threading.active_count())
        dialogue_idx = random.randint(0, 1)
        if current_data.face_num == 1 and finish:
            print("BEEP BEEP. Hey! Move please. I'm driving here")
            #print(finish)
            finish = False
            player = vlc.MediaPlayer(single_dialogue[dialogue_idx])
            events = player.event_manager()
            events.event_attach(vlc.EventType.MediaPlayerEndReached, speak_finished, 1)
            player.play()
        elif current_data.face_num > 1 and finish:
            print("Hey guys! Please move. I'm late for work.")
            #global finish
            finish = False
            player = vlc.MediaPlayer(multi_dialogue[dialogue_idx])
            events = player.event_manager()
            events.event_attach(vlc.EventType.MediaPlayerEndReached, speak_finished, 1)
            player.play()

print("[INFO] camera sensor warming up...")
vs = VideoStream(usePiCamera=0).start()
# vs = VideoStream().start()
time.sleep(2.0)

# loop over the frames from the video stream
while True:
    # grab the frame from the threaded video stream, resize it to
    # have a maximum width of 400 pixels, and convert it to
    # grayscale
    frame = vs.read()
    frame = imutils.resize(frame, width=100)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # detect faces in the grayscale frame
    #rects = detector(gray, 0)

    # convert the image to grayscale, load the face cascade detector,
    # and detect faces in the image
    detector = cv2.CascadeClassifier("./haarcascade_frontalface_default.xml")
    rects = detector.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5,
        minSize=(30, 30), flags=cv2.CASCADE_SCALE_IMAGE)

    # construct a list of bounding boxes from the detection
    # rects = [(int(x), int(y), int(x + w), int(y + h)) for (x, y, w, h) in rects]

    if finish:
        #print "FINISH!!!!"
        t = threading.Thread(target=robot_speak(len(rects)))
        t.daemon = True
        t.start()


    for (x, y, w, h) in rects:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    # show the frame
    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF

    # if the `q` key was pressed, break from the loop
    if key == ord("q"):
        break
