import random
import time
import vlc
import sys

def speak_finished(event, data):
    print "Yayuhhh"
    #player.set_position(0.0)

single_dialogue = ["./res/bicyle-horn-sound-effect.mp3", "./res/single-late-for-work.mp3"]
multi_dialogue = ["./res/bicyle-horn-sound-effect.mp3", "./res/multi-do-you-mind-moving.mp3"]
dialogue_idx = random.randint(0, 1)
player = vlc.MediaPlayer(single_dialogue[dialogue_idx])
events = player.event_manager()
events.event_attach(vlc.EventType.MediaPlayerEndReached, speak_finished, 1)
player.play()
time.sleep(30)

player = vlc.MediaPlayer(single_dialogue[dialogue_idx])
events = player.event_manager()
events.event_attach(vlc.EventType.MediaPlayerEndReached, speak_finished, 1)
player.play()
time.sleep(10)