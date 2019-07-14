
import cv2
import pyzbar.pyzbar as pyzbar

def qrdetect():
    def decodeDisplay(image):
        global detected
        barcodes = pyzbar.decode(image)
        for barcode in barcodes:
            # 提取二维码的边界框的位置
            # 画出图像中条形码的边界框
            (x, y, w, h) = barcode.rect
            cv2.rectangle(image, (x, y), (x + w, y + h), (0, 0, 255), 2)
 
            # 提取二维码数据为字节对象，所以如果我们想在输出图像上
            # 画出来，就需要先将它转换成字符串
            barcodeData = barcode.data.decode("utf-8")
            barcodeType = barcode.type
 
            # 绘出图像上条形码的数据和条形码类型
            text = "{} ({})".format(barcodeData, barcodeType)
            cv2.putText(image, text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX,
                    .5, (0, 0, 125), 2)
            # 向终端打印条形码数据和条形码类型
            data=open('barresult.txt','w+')
            print(barcodeData,file=data)
            data.close()
        
            detected=True
        return image
    video="http://admin:admin@192.168,43.229:8081/"
    camera = cv2.VideoCapture(video)
    while not detected:
        with open('switch.txt','r') as fr:
        #    for line in fr:
        #       print(line)
            data=fr.read()
            if data[0]=='1':
                # 读取当前帧
                ret, frame = camera.read()
                # 转为灰度图像
                gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
                im = decodeDisplay(gray)

                cv2.waitKey(5)
                cv2.imshow("camera", im)
            else:
                break
    
    camera.release()
    cv2.destroyAllWindows()

while True:
    with open('switch.txt','r') as fr:
        #    for line in fr:
        #       print(line)
            data=fr.read()
            if data[0]=='1':
                detected=False
                qrdetect()

