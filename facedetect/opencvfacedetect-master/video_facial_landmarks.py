# Tutorial from PyImageSearch :
# https://www.pyimagesearch.com/2017/04/17/real-time-facial-landmark-detection-opencv-python-dlib/

import argparse
import cv2
import dlib
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


def robot_speak(face_num, is_speaking):
    single_dialogue = ["./res/bicyle-horn-sound-effect.mp3", "./res/single-late-for-work.mp3"]
    multi_dialogue = ["./res/bicyle-horn-sound-effect.mp3", "./res/multi-do-you-mind-moving.mp3"]
    # with speak_lock:
    # print("Starting thread : {}".format(threading.current_thread().name))
    with speak_lock:
        current_data = threading.local()
        current_data.face_num = face_num
        current_data.is_speaking = is_speaking
        print("THREADS INSIDE: %d", threading.active_count())
        dialogue_idx = random.randint(0, 1)
        if current_data.face_num == 1 and not current_data.is_speaking:
            print("BEEP BEEP. Hey! Move please. I'm driving here")
            print(finish)
            global finish
            finish = False
            player = vlc.MediaPlayer(single_dialogue[dialogue_idx])
            events = player.event_manager()
            events.event_attach(vlc.EventType.MediaPlayerEndReached, speak_finished, 1)
            player.play()
        elif current_data.face_num > 1 and not current_data.is_speaking:
            print("Hey guys! Please move. I'm late for work.")
            global finish
            finish = False
            player = vlc.MediaPlayer(multi_dialogue[dialogue_idx])
            events = player.event_manager()
            events.event_attach(vlc.EventType.MediaPlayerEndReached, speak_finished, 1)
            player.play()

    # with speak_lock:
    # print("Finished thread: {}".format(threading.current_thread().name))


# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-p", "--shape-predictor", required=True, help="path to facial landmark predictor")
ap.add_argument("-r", "--picamera", type=int, default=1,
                help="whether or not the Raspberry Pi camera should be used")
args = vars(ap.parse_args())

# Initialize dlib's face detector (HOG-based) and then create
# the facial landmark peredictor
print("[INFO] loading facial landmark predictor...")
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(args["shape_predictor"])

print("[INFO] camera sensor warming up...")
vs = VideoStream(usePiCamera=args["picamera"] > 0).start()
# vs = VideoStream().start()
time.sleep(2.0)

prev_rects = 0
is_speaking = True

# loop over the frames from the video stream
while True:
    # grab the frame from the threaded video stream, resize it to
    # have a maximum width of 400 pixels, and convert it to
    # grayscale
    frame = vs.read()
    frame = imutils.resize(frame, width=400)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # detect faces in the grayscale frame
    rects = detector(gray, 0)

    if prev_rects != rects:
        prev_rects = rects
        is_speaking = False
        print("THREADS: %d", threading.active_count())
        if finish:
            t = threading.Thread(target=robot_speak(len(rects), is_speaking))
            t.daemon = True
            t.start()

    for rect in rects:
        prev_rects = rects
        # determine the facial landmarks for the face region, then
        # convert the facial landmark (x, y)-cordinates ot a NumPy
        # array
        shape = predictor(gray, rect)
        shape = face_utils.shape_to_np(shape)

        # loop over the (x, y)-coordinates for the facial landmarks
        # and draw them on the image
        for (x, y) in shape:
            cv2.circle(frame, (x, y), 1, (0, 0, 255), -1)

    # show the frame
    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF

    # if the `q` key was pressed, break from the loop
    if key == ord("q"):
        break

cv2.destroyAllWindows()
vs.stop()

# t = threading.Thread(target=detect_face())
# t.daemon = True
# t.start()
