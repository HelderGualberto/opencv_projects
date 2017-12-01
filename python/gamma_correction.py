import cv2, os
import numpy as np

def read_images_from_directory(path):
	files = os.listdir(path)
	imgs = []

	for name in files:
		filename = path + name

		mat = cv2.imread(filename)
		imgs.append(mat)

	print files
	return imgs

def adjust_gamma(image, gamma=1.0):
		# build a lookup table mapping the pixel values [0, 255] to
		# their adjusted gamma values
		invGamma = 1.0 / gamma
		table = np.array([((i / 255.0) ** invGamma) * 255
			for i in np.arange(0, 256)]).astype("uint8")
	 
		# apply gamma correction using the lookup table
		return cv2.LUT(image, table)




input_path = '../data/dataset/predic/'
output_path = '../data/dataset/output/'


imgs = read_images_from_directory(input_path)

c = 0
for img in imgs:
	correct_gamma(1.0, img)
	imname = output_path + 'img_' + str(c) + '.jpg'

	print "Saving: {}".format(imname)

	cv2.imwrite(imname,img)

	c+=1

