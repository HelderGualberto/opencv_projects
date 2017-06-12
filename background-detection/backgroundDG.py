# Autor: Roberto Kenji Hiramatsu
# Teste local para verificar capacidade ocupada de processamento em maquina local e uso de modo de tracking para otimizar captura dos dados
# Data: 2016-09-05


import numpy as np
import cv2
from imutils import paths
from common import clock, draw_str
from datetime import datetime
import thread
import copy

def draw_rects(img, rects, color):
    for x1, y1, x2, y2 in rects:
        cv2.rectangle(img, (x1, y1), (x2, y2), color, 2)

#cv2.ocl.setUseOpenCL(False)
def detectaM(imag,sx,sy):
    somaf = cv2.integral(imag)
    h,w = imag.shape
    px = w / sx
    py = h / sy
    sqv = []
    bmax = float(w*h/(sx*sy))*255.0
    resp = False
    xmin = w
    ymin = h
    xmax = 0
    ymax = 0
    for ix in range(0,sx):
        for iy in range(0,sy):
            x1 = ix * px
            y1 = iy * py
            x2 = (ix + 1) * px 
            y2 = (iy + 1) * py 
            v = somaf[y2,x2]-somaf[y2,x1]-somaf[y1,x2]+somaf[y1,x1]
            print somaf[y2,x2], somaf[y2,x1], somaf[y1,x2], somaf[y1,x1]
            sqv.append(v)
            pp = float(v)/bmax
            #print ix,':',iy,'->',v," per:",pp
            if pp > 0.1:
                #print ix,':',iy,'(',bmax,')->',v," per:",pp
                xmin = x1 if x1 < xmin else xmin
                ymin = y1 if y1 < ymin else ymin
                xmax = x2 if x2 > xmax else xmax
                ymax = y2 if y2 > ymax else ymax
                #return True
                resp = True
                cv2.imshow('blackmask',imag)
                
    if resp:
       xmin = xmin - px if xmin >= px else xmin
       ymin = ymin - py if ymin >= py else ymin
       xmax = xmax + px if (xmax + px) <= w else xmax
       ymax = ymax + py if (ymax + py) <= h else ymax
       
       #print xmin,ymin,xmax,ymax
    return resp,np.array([xmin,ymin,xmax,ymax])

if __name__ == '__main__':
  try:
    #cap = cv2.VideoCapture(args.video)
    cap = cv2.VideoCapture("rtsp://viwer:foscam123@10.1.1.5:554/videoMain")
    print cap
    fgbg  = cv2.BackgroundSubtractorMOG()
    #(history=100,varThreshold=16)
    #(100,3,0.3)
    conta = 0
    baseframe = True 
    while(True):
        t = clock()
        ret, frame = cap.read()
        
        if ret:
            conta += 1
            reduzido = cv2.resize(frame,(0,0),fx=0.5,fy=0.5)
            fgm = fgbg.apply(reduzido,learningRate=0.1)
            resp,area = detectaM(fgm,16,6) 
            if resp:
              draw_rects(frame,[area],(0,0,255))
            cv2.imshow('frame',frame)
            cv2.waitKey(1)

  except Exception:
    pass
    # When everything done, release the capture
    #cv2.destroyAllWindows()
  cap.release()
