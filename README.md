# autonomous-mobile-robot
Project for Software Development for Robotics course at Kent State


To open communication with Arduino: 
python2 SDR_tcp2serial.py -P 1234 /dev/ttyACM0

Then, in separate terminal window:
telnet { ip of pi } { specified port in above command }
i.e. "telnet 10.14.220.129 1234"




To start web video server with speed/odometer info:
python2 SDR_tcp2serial.py -P 5555 /dev/ttyACM0
  - NOTE: port 5555 is required and hard coded in the main.py web server file
  
Then, in separate terminal window, in ATR_Video_Streamer directory:
python2 main.py

Navigate to localhost:5000 to open the web page's comm with the Arduino's serial.


Path to project:
/home/ubuntu/SDR_project/SDR_SUGV

Path to video streamer:
/home/ubuntu/SDR_project/ATR_Video_Streamer
