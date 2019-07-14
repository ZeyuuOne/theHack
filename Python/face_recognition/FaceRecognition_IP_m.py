import cv2

def recognition():
    recognizer = cv2.face.LBPHFaceRecognizer_create()
    recognizer.read('face_trainer/trainer.yml')
    cascadePath = r'C:\Users\ZeyuuOne\AppData\Local\Programs\Python\Python36\Lib\site-packages\cv2\data\haarcascade_frontalface_default.xml'
    faceCascade = cv2.CascadeClassifier(cascadePath)
    font = cv2.FONT_HERSHEY_SIMPLEX

    idnum = 0

    names = ['Yushi Xia','Xianglin Li']
    vote=[]

    video="http://admin:admin@192.168.43.38:8082/"   #此处@后的ipv4 地址需要改为app提供的地址
    cam =cv2.VideoCapture(video)

    minW = 0.1*cam.get(3)
    minH = 0.1*cam.get(4)

    finish=0
    prediction='Unknown'

    while finish==0:
        with open('switch.txt','r') as fr:
        #    for line in fr:
        #       print(line)
            data=fr.read()
            print(data)
            if data[0]=='1':
                ret, img = cam.read()
                gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

                faces = faceCascade.detectMultiScale(
                    gray,
                    scaleFactor=1.2,
                    minNeighbors=5,
                    minSize=(int(minW), int(minH))
                )

                for (x, y, w, h) in faces:
                    cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 2)
                    idnum, confidence = recognizer.predict(gray[y:y+h, x:x+w])

                    if confidence < 100:
                        vote.append(idnum)
                        idname = names[idnum]
                        confidence = "{0}%".format(round(100 - confidence))
                    else:
                        idname = "unknown"
                        confidence = "{0}%".format(round(100 - confidence))

                    if len(vote)>10:
                        for p in range(len(names)):
                            rate=vote.count(p)/len(vote)
                            if rate>0.5:
                                finish=1
                                prediction=names[p]
                                break
                    
                    cv2.putText(img, str(idname), (x+5, y-5), font, 1, (0, 0, 255), 1)
                    cv2.putText(img, str(confidence), (x+5, y+h-5), font, 1, (0, 0, 0), 1)

                cv2.imshow('camera', img)
                k = cv2.waitKey(10)
                if k == 27:
                    break
            else:
                break

    #print到文件中
    data=open("faceresult.txt",'w+') 
    print(prediction,file=data)
    data.close()

    cam.release()
    cv2.destroyAllWindows()

while True:
    with open('switch.txt','r') as fr:
    #    for line in fr:
    #       print(line)
        data=fr.read()
        if data=='1':
            recognition()
