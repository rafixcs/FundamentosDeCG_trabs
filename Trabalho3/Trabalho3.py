from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from Ponto import Ponto
from Linha import Linha
from Texture import Texture
from Paredao import *
import time
from math import *
from Jogador import Jogador

P_WIDTH = 25
P_HEIGHT = 15
P_BRICKSIZE = 1

gblAngulo = 0.0
gblAnda = 0
gblAnda2 = 0
angle = 1.8
lx = 0.2
lz = 0.9
upY = 0
angleY = 0
gblPosAspectRatio = 0
gblTexturesId = {}
modoprojecao = False
paredao = None
jogador = None
jogadorPos = [0,0]

# **********************************************************************
#  init()
#  Inicializa os parÃ¢metros globais de OpenGL
#/ **********************************************************************
def init():
    global gblAnda
    global gblAnda2
    global angle
    global lx
    global lz
    global upY
    global paredao
    global angleY
    global jogador
    global jogadorPo

    # Define a cor do fundo da tela (BRANCO) 
    glClearColor(0.5, 0.5, 0.5, 1.0)
    
    glEnable(GL_DEPTH_TEST )
    glEnable(GL_TEXTURE_2D)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
   
    glEnable (GL_CULL_FACE )
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 )    

    global gblTexturesId
    texture = Texture('little_grass-tile.jpg')
    gblTexturesId['grass'] = texture.getTextureId()
    texture = Texture('little_brick-tile.jpg')
    gblTexturesId['brick'] = texture.getTextureId()
    paredao = Paredao(P_WIDTH, P_HEIGHT, P_BRICKSIZE, gblTexturesId['brick'])

    gblAnda = 0#11.8779#-19.11339#0.8053#-4.0 #-4.45
    gblAnda2 = 0#-35.033#-8.3941#0.7084#-1.0 #0.446
    angle = 1.7998#2.5999#2.8#1.8
    lx = 0.2271#0.8568#0.9422#0.2 #0.9899
    lz = 0.9738#0.5155#0.3349#0.9 #0.1411
    upY = 8#4 #0 #66
    angleY = 0
    jogadorPos = [0,0]

    jogador = Jogador(0,0)


# **********************************************************************
#  reshape( w: int, h: int )
#  trata o redimensionamento da janela OpenGL
#
# **********************************************************************
def reshape(w: int, h: int):
    global gblPosAspectRatio
	# Evita divisÃ£o por zero, no caso de uam janela com largura 0.
    if h == 0:
        h = 1
    # Ajusta a relaÃ§Ã£o entre largura e altura para evitar distorÃ§Ã£o na imagem.
    # Veja funÃ§Ã£o "PosicUser".
    gblPosAspectRatio = w / h
	# Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    # Seta a viewport para ocupar toda a janela
    glViewport(0, 0, w, h)
    
    PosicUser()
# **********************************************************************
def DefineLuz():
    # Define cores para um objeto dourado
    LuzAmbiente = [0.4, 0.4, 0.4] 
    LuzDifusa   = [0.7, 0.7, 0.7]
    LuzEspecular = [0.9, 0.9, 0.9]
    PosicaoLuz0  = [2.0, 3.0, 0.0 ]  # PosiÃ§Ã£o da Luz
    Especularidade = [1.0, 1.0, 1.0]

    # ****************  Fonte de Luz 0

    glEnable ( GL_COLOR_MATERIAL )

    #Habilita o uso de iluminaÃ§Ã£o
    glEnable(GL_LIGHTING)

    #Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente)
    # Define os parametros da luz nÃºmero Zero
    glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  )
    glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  )
    glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 )
    glEnable(GL_LIGHT0)

    # Ativa o "Color Tracking"
    glEnable(GL_COLOR_MATERIAL)

    # Define a reflectancia do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade)

    # Define a concentraÃ§Ã£oo do brilho.
    # Quanto maior o valor do Segundo parametro, mais
    # concentrado serÃ¡ o brilho. (Valores vÃ¡lidos: de 0 a 128)
    glMateriali(GL_FRONT,GL_SHININESS,51)

# **********************************************************************
# DesenhaCubos()
# Desenha o cenario
#
# **********************************************************************
def DesenhaCubo():
    glutSolidCube(1)
    
def PosicUser():
    global gblAnda
    global gblAnda2
    global lx
    global lz
    global upY
    global angleY

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    #print ("AspectRatio", AspectRatio)
    
    if modoprojecao:        
        glOrtho(-10, 10, 10, 0, 0.05, 100)
    else: 
        gluPerspective(40, gblPosAspectRatio, 0.05, 100) # Projecao perspectiva
    
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    gluLookAt(11.8779, 8, -35.033, 
              11.8779 + 0.2271, 8, -35.033 + 0.9738,
              0, 2 ,0)
    glRotatef(angleY, 1, 1, 0) 

# **********************************************************************
# void DesenhaLadrilho(int corBorda, int corDentro)
# Desenha uma cÃ©lula do piso.
# O ladrilho tem largula 1, centro no (0,0,0) e estÃ¡ sobre o plano XZ
# **********************************************************************
def DesenhaLadrilho():
    glColor3f(1,1,1) # desenha QUAD preenchido
    glBegin ( GL_QUADS )
    glNormal3f(0,1,0)
    glTexCoord2f(0.0, 0.0)
    glVertex3f(-0.5, 0.0, -0.5)
    glTexCoord2f(1.0, 0.0)
    glVertex3f(-0.5, 0.0, 0.5)
    glTexCoord2f(1.0, 1.0)
    glVertex3f( 0.5, 0.0, 0.5)
    glTexCoord2f(0.0, 1.0)
    glVertex3f( 0.5, 0.0, -0.5)
    glEnd()
    
    glColor3f(1,1,1) # desenha a borda da QUAD 
    glBegin ( GL_LINE_STRIP )
    glNormal3f(0,1,0)
    glVertex3f(-0.5,  0.0, -0.5)
    glVertex3f(-0.5,  0.0,  0.5)
    glVertex3f( 0.5,  0.0,  0.5)
    glVertex3f( 0.5,  0.0, -0.5)
    glEnd()
    
# **********************************************************************
def DesenhaPiso():
    global gblTexturesId
    glPushMatrix()    
    glTranslated(0, -1, -0)
    for x in range(0,50):
        glPushMatrix()
        glBindTexture(GL_TEXTURE_2D, gblTexturesId['grass'])
        for z in range(0, 25):
            DesenhaLadrilho()
            glTranslated(0, 0, 1)
        glPopMatrix()
        glTranslated(1, 0, 0)
    glPopMatrix()     
# **********************************************************************
# display()
# Funcao que exibe os desenhos na tela
#
# **********************************************************************
def display():
    animate()

    global gblAngulo
    global paredao

    # Limpa a tela com  a cor de fundo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    DefineLuz()
    PosicUser()

    glMatrixMode(GL_MODELVIEW)    
    
    DesenhaPiso()
    paredao.Draw()    
    jogador.draw()


    glutSwapBuffers()


# **********************************************************************
# animate()
# Funcao chama enquanto o programa esta ocioso
# Calcula o FPS e numero de interseccao detectadas, junto com outras informacoes
#
# **********************************************************************
# Variaveis Globais
nFrames, TempoTotal, AccumDeltaT = 0, 0, 0
oldTime = time.time()

def animate():
    global nFrames, TempoTotal, AccumDeltaT, oldTime, dt

    nowTime = time.time()
    dt = nowTime - oldTime
    oldTime = nowTime

    AccumDeltaT += dt
    TempoTotal += dt
    nFrames += 1
    
    if AccumDeltaT > 1.0/30:  # fixa a atualizaÃ§Ã£o da tela em 30
        AccumDeltaT = 0
        glutPostRedisplay()

    

# **********************************************************************
#  keyboard ( key: int, x: int, y: int )
#
# **********************************************************************
ESCAPE = b'\x1b'
def keyboard(*args):
    #print (args)
    # If escape is pressed, kill everything.
    global upY
    global angleY
    global dt

    if args[0] == ESCAPE:   # Termina o programa qdo
        os._exit(0)         # a tecla ESC for pressionada
    elif args[0] == b' ':
        init()
    elif args[0] == b'e':
        global modoprojecao
        modoprojecao = not modoprojecao
    elif args[0] == b'w':        
        upY += 2
    elif args[0] == b's':        
        upY -= 2
    elif args[0] == b'1':
        angleY = sin(angleY) + 1.5 * dt
    elif args[0] == b'2':
        angleY = cos(angleY) - 1.5 * dt
    # ForÃ§a o redesenho da tela
    glutPostRedisplay()

# **********************************************************************
#  arrow_keys ( a_keys: int, x: int, y: int )   
# **********************************************************************

def arrow_keys(a_keys: int, xx: int, yy: int):

    global gblAnda
    global gblAnda2
    global angle
    global lx
    global lz
    global jogador
    global jogadorPos

    fraction = 0.2
    
    if a_keys == GLUT_KEY_LEFT:       # Se pressionar LEFT
        '''angle += fraction
        lz = sin(angle)
        lx = -cos(angle)'''
        jogadorPos[1] -= 1
        jogador.movimenta(jogadorPos, (lx, lz), angle)
    elif a_keys == GLUT_KEY_RIGHT:      # Se pressionar RIGHT
        '''angle -= fraction
        lz = sin(angle)
        lx = -cos(angle)'''
        jogadorPos[1] += 1
        jogador.movimenta(jogadorPos, (lx, lz), angle) 
    elif a_keys == GLUT_KEY_UP:         # Se pressionar UP
        '''gblAnda += lx * fraction * 1.5
        gblAnda2 += lz * fraction * 1.5'''
        jogadorPos[0] += 1
        jogador.movimenta(jogadorPos, (lx, lz), angle)
    elif a_keys == GLUT_KEY_DOWN:       # Se pressionar DOWN        
        '''gblAnda -= lx * fraction * 1.5
        gblAnda2 -= lz * fraction * 1.5'''
        jogadorPos[0] -= 1
        jogador.movimenta( jogadorPos, (lx, lz), angle)

    glutPostRedisplay()


def mouse(button: int, state: int, x: int, y: int):
    glutPostRedisplay()

def mouseMove(x: int, y: int):
    glutPostRedisplay()

# ***********************************************************************************
# Programa Principal
# ***********************************************************************************

if __name__ == '__main__':        
    glutInit(sys.argv)
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB)
    glutInitWindowPosition(0, 0)
    glutInitWindowSize(1548, 755)
    glutInitWindowPosition(100, 100)
    
    wind = glutCreateWindow(b"Trabalho 3 Fundamentos de CG")
    
    init ()
    
    glutDisplayFunc(display)
    glutReshapeFunc(reshape)    
    glutKeyboardFunc(keyboard)
    glutSpecialFunc(arrow_keys)
    #glutMouseFunc(mouse)
    #glutMotionFunc(mouseMove)


    try:        
        glutMainLoop()
    except SystemExit:
        pass