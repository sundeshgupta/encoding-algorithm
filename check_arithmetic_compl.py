import cv2

x = int(input())
img = cv2.imread('0.jpg')

chk = cv2.resize(img, (x, x))

cv2.imwrite('chk.jpg', chk)