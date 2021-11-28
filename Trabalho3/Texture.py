from OpenGL.GLUT import *
from OpenGL.GL import *
import numpy as np
from PIL import Image
import ctypes

class Texture:
    def __init__(self, image_path):        
        self.textureId = glGenTextures(1)
        self._load(image_path)

    def _load(self, image_path):
        img = Image.open(image_path)
        width, height = img.size
        image_data = np.array(list(img.getdata()), np.uint8)
        glBindTexture(GL_TEXTURE_2D, self.textureId)

        glTexImage2D(
            GL_TEXTURE_2D, 
            0,
            GL_RGB,
            width, height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            image_data
        )

        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

    def getTextureId(self):
        return self.textureId