import cv2
import urllib.request
import numpy as np
import concurrent.futures

# URLs for the Haar Cascades
face_cascade_url = 'https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_frontalface_default.xml'
leg_cascade_url = 'https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_lowerbody.xml'

# Load the Haar Cascades
face_cascade = cv2.CascadeClassifier()
face_cascade.load(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

leg_cascade = cv2.CascadeClassifier()
leg_cascade.load(cv2.data.haarcascades + 'haarcascade_lowerbody.xml')

# URL for the camera
url='http://192.168.1.6/cam-hi.jpg'
im=None
 
def run1():
    cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)
    while True:
        img_resp=urllib.request.urlopen(url)
        imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
        im = cv2.imdecode(imgnp,-1)
 
        cv2.imshow('live transmission',im)
        key=cv2.waitKey(5)
        if key==ord('q'):
            break
            
    cv2.destroyAllWindows()

def run2():
    cv2.namedWindow("detection", cv2.WINDOW_AUTOSIZE)
    while True:
        img_resp=urllib.request.urlopen(url)
        imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
        im = cv2.imdecode(imgnp,-1)
 
        # Convert to grayscale
        gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
 
        # Detect faces
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
 
        # Draw bounding boxes around the faces
        for (x, y, w, h) in faces:
            cv2.rectangle(im, (x, y), (x+w, y+h), (0, 255, 0), 2)
 
        # Detect legs
        legs = leg_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
 
        # Draw bounding boxes around the legs
        for (x, y, w, h) in legs:
            cv2.rectangle(im, (x, y), (x+w, y+h), (255, 0, 0), 2)
 
        cv2.imshow('detection',im)
        key=cv2.waitKey(5)
        if key==ord('q'):
            break
            
    cv2.destroyAllWindows()
 
 
if __name__ == '__main__':
    print("started")
    with concurrent.futures.ProcessPoolExecutor() as executer:
            f1= executer.submit(run1)
            f2= executer.submit(run2)
