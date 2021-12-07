from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from Ponto import Ponto
from Trabalho3 import *
import random
from Jogador import *

surfaces = (
    (0,1,2,3),
    (3,2,7,6),
    (6,7,5,4),
    (4,5,1,0),
    (1,5,7,2),
    (4,0,3,6)
)

vertices = (
    (0.5, -0.5, -0.5),
    (0.5, 0.5, -0.5),
    (-0.5, 0.5, -0.5),
    (-0.5, -0.5, -0.5),
    (0.5, -0.5, 0.5),
    (0.5, 0.5, 0.5),
    (-0.5, -0.5, 0.5),
    (-0.5, 0.5, 0.5)
)

texCoords = (
    (0.0, 0.0),
    (1.0, 0.0),
    (1.0, 1.0),
    (0.0, 1.0)
)

class BrickCube:
    def __init__(self, pos, index, size, textureId):
        self.pos = pos
        self.index = index
        self.size = size
        self.enabled = True
        self.textureId = textureId

    def Draw(self):        
            glPushMatrix()
            glTranslated(self.pos[0], self.pos[1], self.pos[2])
            DesenhaCubo()

            # TOO SLOOOW
            '''glBegin(GL_QUADS)
            for surface in surfaces:
                cont = 0          
                for vertex in surface:
                    glTexCoord2d(random.uniform(0, 1.0), random.uniform(0, 1.0))
                    glVertex3fv(vertices[vertex])
                    cont += 1
            glEnd()'''

            glPopMatrix()

    def DestroyBrick(self):
        self.enabled = False
    def IsEnabled(self):
        return self.enabled

class Paredao:
    def __init__(self, width, height, brick_size, textureId):
        self.width = width
        self.height = height
        self.brick_size = brick_size
        self.bricks = []
        self.matrix = []
        self._build(textureId)

    def _build(self, textureId):
        index = 0
        for x in range(0, self.width):
            for y in range(0, self.height):
                __brick = BrickCube(numpy.array((15+x, y-0.5, 0)), index, self.brick_size, textureId)
                self.bricks.append(__brick)

    def Draw(self):
        random.seed(1)
        for brick in self.bricks:
            if brick.IsEnabled():
                glPushMatrix()
                glRotatef(90, 0.0, 1.0, 0.0)
                glTranslated(-39, 0, 24)
                matrix = numpy.array(numpy.zeros(16))
                glGetFloatv(GL_MODELVIEW_MATRIX, matrix)
                brick.matrix = matrix
                glColor3f(1.0, 1.0, 1.0)
                glEnable(GL_TEXTURE_GEN_S)
                glEnable(GL_TEXTURE_GEN_T)
                glBindTexture(GL_TEXTURE_2D, brick.textureId)
                brick.Draw()
                glDisable(GL_TEXTURE_GEN_S)
                glDisable(GL_TEXTURE_GEN_T)
                glPopMatrix()
    
    def Colision(self, jogador):
        for brick in self.bricks:            
            aux = brick.pos - jogador.tiro.pos_t
            if aux[0] < 1 and aux[1] < 1 and aux[2] < 1:
                self.bricks.remove(brick)
                jogador.tiro.enabled = False
