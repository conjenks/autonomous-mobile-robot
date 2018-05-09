import cv2
import imutils
from imutils.video import VideoStream
from imutils import face_utils
import random
import time
import threading
import vlc

from threading import Thread
import sys

# import the Queue class from Python 3
if sys.version_info >= (3, 0):
    from queue import Queue

else:
    from Queue import Queue

class VideoCamera(object):

    first_connection = True
    vs = None
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

    def __init__(self):
        # Using OpenCV to capture from device 0. If you have trouble capturing
        # from a webcam, comment the line below out and use a video file
        # instead.
        #self.video = cv2.VideoCapture(0)
        # If you decide to use video.mp4, you must have this file in the folder
        # as the main.py.
        # self.video = cv2.VideoCapture('video.mp4')

        print("[INFO] camera sensor warming up...")
        self.vs = VideoStream(usePiCamera=0, resolution=(100, 100), framerate=20).start()
        time.sleep(2.0)

        #self.stopped = False
        #self.Q = Queue(maxsize=128)

    def __del__(self):
        #self.video.release()
        #self.vs.stop()
        self.stop()

    def start(self):
        print "Started thread"
        # start a thread to read frames from the camera
        t = Thread(target=self.update, args=())
        t.daemon = True
        t.start()
        return self

    def update(self):
        #keep looping infinitely
        while True:
            # if the thread indicator variable is set, stop the thread
            if self.stopped:
                return

            # otherwise, ensure the queue has room in it
            if not self.Q.full():
                # read the next frame from the video feed
                frame = self.vs.read()

                if frame is None:
                    self.stop()
                    return

                # add the frame to the queue
                self.Q.put(frame)

    def more(self):
        # return True if there are still frames in the queue
        return self.Q.qsize() > 0

    def stop(self):
        # indicate that the thread should be stopped
        self.vs.stop()
        self.stopped = True

    def read_frame(self):
        # return next frame in the queue
        return self.Q.get()

    def get_frame(self, info):

        #if self.first_connection:
            #self.start()
            #self.first_connection = False

        #(success, image) = self.video.read()
        # We are using Motion JPEG, but OpenCV defaults to capture raw images,
        # so we must encode it into JPEG in order to correctly display the
        # video stream.

        frame = self.vs.read()
        ret, jpeg = cv2.imencode('.jpg', frame)
        return jpeg.tobytes()

        #if self.more():
            #frame = self.read_frame()
            #print "Read frame"
            ##frame = imutils.resize(frame, width=100)
            ##gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            ## convert the image to grayscale, load the face cascade detector,
            ## and detect faces in the image
            ##detector = cv2.CascadeClassifier("./haarcascade_frontalface_default.xml")
            ##rects = detector.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5,
                ##minSize=(30, 30), flags=cv2.CASCADE_SCALE_IMAGE)
            ##print "RECTS: " + str(len(rects))

            ##for (x, y, w, h) in rects:
                ##cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

            ## show the frame
            ##cv2.imshow("Frame", frame)

            #ret, jpeg = cv2.imencode('.jpg', frame)
            #return jpeg.tobytes()