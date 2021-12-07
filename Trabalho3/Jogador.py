
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from math import sin
from math import cos
import numpy 

class Tiro:
    def __init__(self, pos, jogador):
        self.pos_t = pos + 2
        self.jogador = jogador
        self.enabled = True

    def avanca(self):
        if (self.pos_t[0] - self.jogador.pos[0]) < 25:
            glPushMatrix()
            self.pos_t[0] += 1
            glTranslatef(self.pos_t[0], 0, self.pos_t[2])
            glutSolidCube(0.3)       
            glPopMatrix()
        else:
            self.enabled = False


class Jogador:
    def __init__(self, horiz:float, vert:float) -> None:
        self.pos = numpy.array((0.0, 0.0, 0.0))
        self.rotation = numpy.array((0.0, 0.0))
        self.andada = 1
        self.angle = 0
        self.tiro = None

    def movimenta(self, pos, posll, angle) -> None:
        self.pos[0] += pos[0] * 0.3
        self.pos[2] += pos[2] * 0.3
        self.rotation[0] = pos[0] + posll[0]
        self.rotation[1] = pos[2] + posll[0]
        self.angle += angle

    def draw(self):                
        glPushMatrix()
        glTranslatef(self.pos[0], 0, self.pos[2])
        glutSolidCube(1)       
        glPopMatrix()

        if self.tiro != None and self.tiro.enabled:
            self.tiro.avanca()
        

    def shot(self):
        position = self.pos
        self.tiro = Tiro(position, self)
    