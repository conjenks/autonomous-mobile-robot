import cv2
import imutils
from imutils.video import VideoStream
from imutils import face_utils
import time
import vlc
import random


class VideoCamera(object):

    finish = True
    time_start = time.time()
    time_now = time.time()

    def speaking_finished(self, event, data):
        self.finish = True

    def robot_speak(self, face_num):
        single_dialogue = ["./res/bicyle-horn-sound-effect.mp3", "./res/single-late-for-work.mp3"]
        multi_dialogue = ["./res/bicyle-horn-sound-effect.mp3", "./res/multi-do-you-mind-moving.mp3"]
        dialogue_idx = random.randint(0, 1)
        if face_num == 1 and self.finish:
            print("BEEP BEEP. Hey! Move please. I'm driving here")
            #print(finish)
            self.finish = False
            player = vlc.MediaPlayer(single_dialogue[dialogue_idx])
            events = player.event_manager()
            events.event_attach(vlc.EventType.MediaPlayerEndReached, self.speaking_finished, 1)
            player.play()
        elif face_num > 1 and self.finish:
            print("Hey guys! Please move. I'm late for work.")
            #global finish
            self.finish = False
            player = vlc.MediaPlayer(multi_dialogue[dialogue_idx])
            events = player.event_manager()
            events.event_attach(vlc.EventType.MediaPlayerEndReached, self.speaking_finished, 1)
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

    def __del__(self):
        #self.video.release()
        self.vs.stop()

    def get_frame(self, info):

        self.time_now = time.time()
        #success, image = self.video.read()

        frame = self.vs.read()
        frame = imutils.resize(frame, width=200)

        # convert the image to grayscale, load the face cascade detector,
        # and detect faces in the image
        if self.finish:
            if (self.time_now - self.time_start) > 5:
                self.time_start = time.time()
                gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
                detector = cv2.CascadeClassifier("./haarcascade_frontalface_default.xml")
                rects = detector.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5,
                    minSize=(30, 30), flags=cv2.CASCADE_SCALE_IMAGE)

                for (x, y, w, h) in rects:
                    cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

                if len(rects) > 0:
                    self.robot_speak(len(rects))
        else:
            print "NOT done speaking!"
        #for (x, y, w, h) in rects:
            #cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)


        # We are using Motion JPEG, but OpenCV defaults to capture raw images,
        # so we must encode it into JPEG in order to correctly display the
        # video stream.
        font = cv2.FONT_HERSHEY_SIMPLEX
        #image = cv2.putText(frame,'Kent State University', (10,40),font, 1, (255,255,255), 2, cv2.LINE_AA)
        #image = cv2.putText(frame,'Software Development for Robotics', (10,100),font, 1, (255,255,255), 2, cv2.LINE_AA)
        image = cv2.putText(frame, info, (10,400),font, 1, (255,255,255), 2, cv2.LINE_AA)
        ret, jpeg = cv2.imencode('.jpg', frame)
        #jpeg = cv2.line(jpeg, (100,0), 63, (0,0,255), -1)
        return jpeg.tobytes()