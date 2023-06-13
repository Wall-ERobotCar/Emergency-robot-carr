import cv2
import matplotlib.pyplot as plt
import cvlib as cv
import urllib.request
import numpy as np
from cvlib.object_detection import draw_bbox
import concurrent.futures

url = 'http://192.168.1.6/cam-hi.jpg'
im = None


def run1():
    cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)
    while True:
        img_resp = urllib.request.urlopen(url)
        imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        im = cv2.imdecode(imgnp, -1)

        cv2.imshow('live transmission', im)
        key = cv2.waitKey(5)
        if key == ord('q'):
            break
    cv2.destroyAllWindows()


def run2():
    cv2.namedWindow("detection", cv2.WINDOW_AUTOSIZE)
    flag = True  # add a flag variable to control loop exit
    while flag:  # use the flag variable in the loop condition
        img_resp = urllib.request.urlopen(url)
        imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        im = cv2.imdecode(imgnp, -1)

        bbox, label, conf = cv.detect_common_objects(im)
        for i, lab in enumerate(label):
            if lab != 'person':
                bbox[i] = [0, 0, 0, 0]
        im = draw_bbox(im, bbox, label, conf)

        cv2.imshow('detection', im)
        key = cv2.waitKey(5)
        if key == ord('q'):
            flag = False  # set the flag to False to exit the loop

    cv2.destroyAllWindows()


if __name__ == '__main__':
    print("started")
    with concurrent.futures.ProcessPoolExecutor() as executor:
        f1 = executor.submit(run1)
        f2 = executor.submit(run2)
