
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from math import sin
from math import cos
import numpy 


class Jogador:
    def __init__(self, horiz:float, vert:float) -> None:
        self.pos = numpy.array((0.0, 0.0))
        self.rotation = numpy.array((0.0, 0.0))
        self.andada = 1

    def movimenta(self, pos, posll, angle) -> None:
        self.pos[0] = pos[0] * 0.3
        self.pos[1] = pos[1] * 0.3
        #self.rotation[0] = pos[0] + posll[0]
        #self.rotation[1] = pos[1] + posll[1]
        

    def draw(self):
        glPushMatrix()
        glTranslatef(self.pos[0], 0, self.pos[1])
        glRotatef(2, self.rotation[0], 0, self.rotation[1])
        glutSolidCube(1)       
        glPopMatrix()    
        

    